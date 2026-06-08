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

	// STRUCTURE DIFF[target 0x779e50 | base 0x57b370]: target 5 / base 5 stmts
	//   1: 0x013 <0x25> | 0x013 <0x12> | ASSERT( prefix );   SIZE
	// .. same ..
	// ; aligned 4, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the ASSERT lowering (empty_stub) byte-size, non-structural.
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
	// STRUCTURE DIFF[target 0x779ff0 | base 0x57bda0]: target 0 / base 0 stmts
	// ; aligned 0, size-diffs 0, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - all inits attributed to the ctor decl line; no body statements; residual is byte-level only.
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

// STATE[79%|PARTIAL]: LOG_ERROR / err.message() inline differently; control structure matches.
void http_client::on_error( boost::system::error_code const& err )
{
	LOG_ERROR( "http_client error: %s", err.message().c_str() );
	close_connection( );
	if ( m_on_error )
		m_on_error( err );

	// STRUCTURE DIFF[target 0x77a110 | base 0x57b6b0]: target 4 / base 4 stmts
	//   1: 0x017 <0xbd> | 0x017 <0xba> | LOG_ERROR( "http_client error: %s", err.message().c_str() );   SIZE
	// .. same ..
	//   3: 0x0df <0x22> | 0x0dc <0x21> | if ( m_on_error )   SIZE
	// .. same ..
	// ; aligned 2, size-diffs 2, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - both SIZE diffs are LOG_ERROR/message() inline-vs-call byte size; no control-structure divergence.
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

	// STRUCTURE DIFF[target 0x77aa50 | base 0x57c0f0]: target 5 / base 5 stmts
	// .. same ..
	// ; aligned 5, size-diffs 0, quantity-diffs 0, blank-gaps 1
	// VERDICT: STRUCTURE MATCH (shape ok) - statements, blocks and sizes all aligned; objdiff scores None (unit pairing), structure is clean.
}

// STATE[97%|PARTIAL]: one SIZE on the bind() arg; control structure matches.
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

	// STRUCTURE DIFF[target 0x77a820 | base 0x57bec0]: target 8 / base 8 stmts
	// .. same ..
	//   6: 0x11d <0xce> | 0x11d <0xd2> | boost::bind( &http_client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator ) );   SIZE
	// .. same ..
	// ; aligned 7, size-diffs 1, quantity-diffs 0, blank-gaps 1
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the bind/async_connect call byte size; no control-structure divergence.
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

	// STRUCTURE DIFF[target 0x77a780 | base 0x57bd00]: target 4 / base 4 stmts
	// .. same ..
	// ; aligned 4, size-diffs 0, quantity-diffs 0, blank-gaps 1
	// VERDICT: STRUCTURE MATCH (shape ok) - statements and sizes aligned; objdiff scores None (unit pairing), structure is clean.
}

// STATE[78%|PARTIAL]: read_lines_from_stream + async_read inline boundaries shuffle line attribution; LOG_ERROR/find inline byte sizes differ.
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

	// STRUCTURE DIFF[target 0x77a3e0 | base 0x57b970]: target 16 / base 16 stmts
	// .. same ..
	//   5: 0x0d6 <0x16> | 0x0d6 <0x23> | s32	found = status_message.find( "HTTP/" );   SIZE
	// .. same ..
	//   7: 0x110 <0x8d> | 0x11d <0x86> | LOG_ERROR( "http_client: Invalid response" );   SIZE
	//   8: 0x19d <0x18> | 0x1a3 <0x5> | } else   SIZE
	// .. same ..
	//   9: 0x1b5 <0x16> | 0x1a8 <0x23> | found = status_message.find( "200" );   SIZE
	// .. same ..
	//  11: 0x1d5 <0x9c> | 0x1d5 <0x96> | LOG_ERROR( "http_client: Response returned with status code %s", status_message.c_str( ) );   SIZE
	//  12: 0x271 <0x18> | 0x26b <0x5> | } else   SIZE
	//  13: --          | 0x270 <0x1a> | read_lines_from_stream( "read_status_line", m_response_buff );   ONLY base
	// .. same ..
	//  14: 0x289 <0x19> | 0x28a <0xd1> | );   SIZE
	// .. same ..
	//  15: 0x2a2 <0xd2> | --          | L151   ONLY target
	// .. same ..
	// ; aligned 8, size-diffs 7, quantity-diffs 2, blank-gaps 2
	// VERDICT: STRUCTURE MISMATCH (size) - same 16 statements + braces; the ONLY base/target pair is a line-attribution shuffle of read_lines_from_stream + the async_read `);` (different inline boundary), not an added/removed statement. All SIZE diffs are LOG_ERROR/string::find inline byte sizes. No source restructure needed.
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

	// STRUCTURE DIFF[target 0x779ca0 | base 0x57b4a0]: target 6 / base 6 stmts
	// .. same ..
	// ; aligned 6, size-diffs 0, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - statements, loop block and sizes aligned; objdiff scores None (unit pairing), structure is clean.
}

// STATE[99%|PARTIAL]: target used a different register; structure clean.
void http_client::close_connection( )
{
	if ( m_socket.is_open( ) )
		m_socket.close( );

	m_on_content_downloaded( );

	// STRUCTURE DIFF[target 0x779f90 | base 0x57b650]: target 3 / base 3 stmts
	// .. same ..
	// ; aligned 3, size-diffs 0, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - statements and sizes aligned; residual is the register-choice byte noise noted on the STATE line.
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

	// STRUCTURE DIFF[target 0x77a240 | base 0x57b7d0]: target 10 / base 10 stmts
	// .. same ..
	// ; aligned 10, size-diffs 0, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - statements, nested if/else-if blocks and sizes all aligned; objdiff scores None (unit pairing), structure is clean.
}

} // namespace network_core
} // namespace vostok
