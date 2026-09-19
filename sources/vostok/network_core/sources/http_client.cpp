// SPDX-License-Identifier: GPL-3.0-or-later
 ////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/network_core/http_client.h>

using boost::asio::ip::tcp;


namespace vostok {
namespace network_core {

void read_lines_from_stream( pcstr prefix, boost::asio::streambuf& buff )
#line 14
{
	VOSTOK_UNREFERENCED_PARAMETERS( prefix );
	std::istream response_stream( &buff );
	std::string str;

	while ( std::getline( response_stream, str ) && str != "\r" )
	{
#line 22
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
#line 28
{
}

void http_client::get( pcstr server, pcstr path, boost::function<void()> const& callback )
#line 31
{
	m_result_content = "";
	m_on_content_downloaded = callback;
#line 39
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
#line 54
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
#line 70
		tcp::endpoint endpoint = *endpoint_iterator;
		m_socket.async_connect(
			endpoint,
			boost::bind( &http_client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator ) );
	}
#line 74
	else
	{
		on_error( err );
	}
}
#line 78

void http_client::handle_connect( boost::system::error_code const& err, tcp::resolver::iterator endpoint_iterator )
#line 81
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
#line 92
		m_socket.close();
		tcp::endpoint endpoint = *endpoint_iterator;
		m_socket.async_connect(
			endpoint,
			boost::bind( &http_client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator ) );
	}
#line 97
	else
	{
		on_error( err );
	}
}
#line 101

void http_client::handle_write_request( boost::system::error_code const& err )
#line 104
{
	if ( !err )
		boost::asio::async_read_until(
			m_socket,
			m_response_buff,
			"\r\n",
			boost::bind( &http_client::handle_read_status_line, this, boost::asio::placeholders::error )
#line 110
		);
	else
#line 113
		on_error( err );
#line 115
}
#line 114

void http_client::handle_read_status_line( boost::system::error_code const& err )
#line 118
{
	if ( !err )
	{
#line 122
		std::istream response_stream( &m_response_buff );

#line 127
		std::string status_message;
		std::getline( response_stream, status_message );
		s32	found = status_message.find( "HTTP/" );
		if ( !response_stream || found != 0 )
		{
			LOG_ERROR( "http_client: Invalid response" );
			return;
		}

#line 135
		found = status_message.find( "200" );
		if ( found == status_message.npos )
		{
			LOG_ERROR( "http_client: Response returned with status code %s", status_message.c_str( ) );
			return;
		}

#line 141
		read_lines_from_stream( "read_status_line", m_response_buff );

		boost::asio::async_read(
			m_socket,
			m_response_buff,
			boost::asio::transfer_at_least( 1 ),
			boost::bind( &http_client::handle_read_content, this, boost::asio::placeholders::error )
#line 151
		);
	} else
#line 154
		on_error( err );
#line 156
}
#line 148

bool http_client::add_result_content( )
#line 159
{
	std::istream response_stream( &m_response_buff );
	std::string str;

	for ( ; std::getline( response_stream, str ) && str != "\r" ; )
	{
		m_result_content.append( str );
#line 167
	}

	return m_result_content.size( ) < 1024;
}
#line 161

void http_client::close_connection( )
{
	if ( m_socket.is_open( ) )
		m_socket.close( );

	m_on_content_downloaded( );
}

void http_client::handle_read_content( boost::system::error_code const& err )
#line 181
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
#line 190
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
#line 193

} // namespace network_core
} // namespace vostok
