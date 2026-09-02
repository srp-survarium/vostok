// SPDX-License-Identifier: GPL-3.0-or-later
 ////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/network_core/http_client.h>

using boost::asio::ip::tcp;


namespace vostok {
namespace network_core {

// claude@NOTE: structure matches (5 stmts / 2 locals). Residual is the ASSERT: the target's
// plain ASSERT emits a single-arg expression_eater( prefix ) guarded block, but our MASTER_GOLD
// ASSERT_T -> VOSTOK_EMPTY_EXPRESSION_VA_ARGS emits no eater (just the identity(false) guard).
// The original game's VOSTOK_EMPTY_EXPRESSION_VA_ARGS expanded to expression_eater(__VA_ARGS__);
// recovering it is a shared debug_macros.h change affecting every plain-ASSERT site, out of
// scope for this TU. ASSERT_U( prefix ) overshoots (pushes prefix AND assert_untyped).
void read_lines_from_stream( pcstr prefix, boost::asio::streambuf& buff )
{
	VOSTOK_UNREFERENCED_PARAMETERS( prefix );
	std::istream response_stream( &buff );
	std::string str;

	while ( std::getline( response_stream, str ) && str != "\r" )
	{
	}
}

http_client::http_client( boost::asio::io_service& io_service ) :
	m_resolver				( io_service ),
	m_socket				( io_service ),
	m_request_buff			( ),
	m_response_buff			( ),
	m_result_content		( ),
	m_on_content_downloaded	( ),
	m_on_error				( )
{
}

void http_client::get( pcstr server, pcstr path, boost::function<void()> const& callback )
{
	m_result_content = "";
	m_on_content_downloaded = callback;
	std::ostream request_stream ( &m_request_buff );
	request_stream << "GET " << path << " HTTP/1.0\r\n";
	request_stream << "Host: " << server << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: close\r\n\r\n";
	tcp::resolver::query query(
		server,
		"http",
		tcp::resolver::query::address_configured );

	m_resolver.async_resolve(
		query,
		boost::bind( &http_client::handle_resolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator )
	);
}

void http_client::on_error( boost::system::error_code const& err )
{
	LOG_ERROR( "http_client error: %s", err.message().c_str() );
	close_connection( );
	if ( m_on_error )
		m_on_error( err );
}

void http_client::handle_resolve( boost::system::error_code const& err, tcp::resolver::iterator endpoint_iterator )
{
	if ( !err )
	{
		tcp::endpoint endpoint = *endpoint_iterator;
		m_socket.async_connect(
			endpoint,
			boost::bind( &http_client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator ) );
	}
	else
	{
		on_error( err );
	}
}

void http_client::handle_connect( boost::system::error_code const& err, tcp::resolver::iterator endpoint_iterator )
{
	if ( !err )
	{
		boost::asio::async_write(
			m_socket,
			m_request_buff,
			boost::bind( &http_client::handle_write_request, this, boost::asio::placeholders::error ) );
	}
	else if ( endpoint_iterator != tcp::resolver::iterator( ) )
	{
		m_socket.close();
		tcp::endpoint endpoint = *endpoint_iterator;
		boost::asio::async_connect(
			m_socket,
			&endpoint,
			boost::bind( &http_client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator ) );
	}
	else
	{
		on_error( err );
	}
}

void http_client::handle_write_request( boost::system::error_code const& err )
{
	if ( !err )
		boost::asio::async_read_until(
			m_socket,
			m_response_buff,
			"\r\n",
			boost::bind( &http_client::handle_read_status_line, this, boost::asio::placeholders::error )
		);
	else
		on_error( err );
}

// claude@NOTE: structure + local set (3) match. Residual is StlPort basic_string::find overload
// resolution: status_message.find( "HTTP/" ) / "200" - the target binds the 2-arg
// find( const char* s, size_type pos ) which computes traits::length internally, our StlPort
// headers bind the path that pre-computes char_traits::length( s ) before the call (+0xd each).
// Library/header version wall, not steerable from this source. The async_read( ... ) tail is the
// usual boost::bind / read_streambuf_op completion-handler inline-vs-call.
void http_client::handle_read_status_line( boost::system::error_code const& err )
{
	if ( !err )
	{
		std::istream response_stream( &m_response_buff );

		std::string status_message;
		std::getline( response_stream, status_message );
		s32	found = status_message.find( "HTTP/" );	// @TODO: std::string::size_type
		if ( !response_stream || found != 0 )
		{
			LOG_ERROR( "http_client: Invalid response" );
		} else
		{
			found = status_message.find( "200" );
			if ( found == status_message.npos )
			{
				LOG_ERROR( "http_client: Response returned with status code %s", status_message.c_str( ) );
			} else
			{
				read_lines_from_stream( "read_status_line", m_response_buff );

				boost::asio::async_read(
					m_socket,
					m_response_buff,
					boost::asio::transfer_at_least( 1 ),
					boost::bind( &http_client::handle_read_content, this, boost::asio::placeholders::error )
				);
			}
		}
	} else
		on_error( err );
}

bool http_client::add_result_content( )
{
	std::istream response_stream( &m_response_buff );
	std::string str;

	for ( ; std::getline( response_stream, str ) && str != "\r" ; )
	{
		m_result_content.append( str );
	}

	return m_result_content.size( ) < 1024;
}

void http_client::close_connection( )
{
	if ( m_socket.is_open( ) )
		m_socket.close( );

	m_on_content_downloaded( );
}

void http_client::handle_read_content( boost::system::error_code const& err )
{
	if ( !err )
	{
		if ( add_result_content( ) )
		{
			boost::asio::async_read(
				m_socket,
				m_response_buff,
				boost::asio::transfer_at_least( 1 ),
				boost::bind( &http_client::handle_read_content, this, boost::asio::placeholders::error )
			);
		} else
			close_connection( );
	} else if ( err != boost::asio::error::eof )
	{
		on_error( err );
	} else if ( err == boost::asio::error::eof )
	{
		add_result_content( );
		close_connection( );
	}
}

} // namespace network_core
} // namespace vostok
