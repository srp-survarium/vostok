 ////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/http_client.h>

using boost::asio::ip::tcp;


namespace vostok {
namespace network_core {

// STATE[88.69%|PARTIAL]: ASSERT lowers differently; control structure matches the target.
void read_lines_from_stream( pcstr prefix, boost::asio::streambuf& buff )
{
	ASSERT( prefix );
	std::istream response_stream( &buff );
	std::string str;

	while ( std::getline( response_stream, str ) && str != "\r" )
	{
	}

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// SIZE -0x13|0| ASSERT( prefix );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the gold-assert machinery lowering
	// (target 0x25: bool temp + helper call + guarded handler call vs our 0x12 eater),
	// non-structural.
}

// STATE[99.85%|PARTIAL]: member-init list, structure clean; residual is LTCG byte noise.
http_client::http_client( boost::asio::io_service& io_service ) :
	m_resolver				( io_service ),
	m_socket				( io_service ),
	m_request_buff			( ),
	m_response_buff			( ),
	m_result_content		( ),
	m_on_content_downloaded	( ),
	m_on_error				( )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts - no diverging rows
	// VERDICT: STRUCTURE MATCH (shape ok) - all inits attributed to the ctor decl line;
	// no body statements; the 0.15% residual is byte-level LTCG noise only.
}

// STATE[100%|DONE]: LTCG for copying `m_on_content_downloaded = callback`
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

// STATE[95.36%|PARTIAL]: LOG_ERROR / err.message() inline differently; control structure matches.
void http_client::on_error( boost::system::error_code const& err )
{
	LOG_ERROR( "http_client error: %s", err.message().c_str() );
	close_connection( );
	if ( m_on_error )
		m_on_error( err );

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE -0x3|0 | LOG_ERROR( "http_client error: %s", err.message().c_str() );
	// SIZE -0x1|+2| if ( m_on_error )
	// VERDICT: STRUCTURE MATCH (shape ok) - both SIZE rows are LOG_ERROR/message()
	// inline byte size (+ __LINE__ value noise); no control-structure divergence.
}

// STATE[100%|DONE]
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

// STATE[97.98%|PARTIAL]: one SIZE on the bind() arg; control structure matches.
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

	// STRUCTURE DIFF: target 8 stmts / base 8 stmts
	// SIZE +0x4|+14| boost::bind( &http_client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the bind/async_connect call byte
	// size; no control-structure divergence.
}

// STATE[100%|DONE]
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

// STATE[88.01%|PARTIAL]: read_lines_from_stream + async_read line attribution shuffles
// (target wraps async_read over more lines); LOG_ERROR/find/else inline byte sizes differ.
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

	// STRUCTURE DIFF: target 16 stmts / base 16 stmts
	// SIZE +0xd |+6 | s32	found = status_message.find( "HTTP/" );	// @TODO: std::string::size_type
	// SIZE -0x7 |+9 | LOG_ERROR( "http_client: Invalid response" );
	// SIZE -0x13|+10| } else
	// SIZE +0xd |+12| found = status_message.find( "200" );
	// SIZE -0x6 |+15| LOG_ERROR( "http_client: Response returned with status code %s", status_message.c_str( ) );
	// SIZE -0x13|+16| } else
	// BASE_ONLY |+18| read_lines_from_stream( "read_status_line", m_response_buff );
	// SIZE +0xb8|+25| );
	// TRGT_ONLY |t+32| --
	// VERDICT: STRUCTURE MATCH (shape ok) - 16/16; the ONLY pair is the SAME read_lines
	// (t 0x19 vs b 0x1a) / async_read tail (t 0xd2 vs b 0xd1) statements mis-paired
	// because the target wraps async_read over ~10 lines (anchor drifts to t+32); SIZE
	// rows are string::find / LOG_ERROR / else-row lowering, non-steerable.
}

// STATE[100%|DONE]
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

// STATE[100%|DONE]
void http_client::close_connection( )
{
	if ( m_socket.is_open( ) )
		m_socket.close( );

	m_on_content_downloaded( );
}

// STATE[100%|DONE]
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
