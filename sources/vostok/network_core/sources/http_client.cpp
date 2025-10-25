////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/network_core/http_client.h>

using boost::asio::ip::tcp;


namespace vostok {
namespace network_core {

// STATE[88%]: ASSERT is completely different, functionally everything else is the same
void read_lines_from_stream( pcstr prefix, boost::asio::streambuf& buff )
{
	ASSERT( prefix );												// <0x789e63>|0x000|0x000:'15'
	std::istream response_stream( &buff );							// <0x789e88>|0x025|0x025:'16'
	std::string str;												// <0x789ed6>|0x073|0x04e:'17'
																	// 1
	while ( std::getline( response_stream, str ) && str != "\r" )	// <0x789ef8>|0x095|0x022:'19'
	{	// 1
		// 2
	}																// <0x789f72>|0x10f|0x07a:'22'
}

// STATE[100%]
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

// STATE[82%]
void http_client::get( pcstr server, pcstr path, boost::function<void()> const& callback )
{
	m_result_content = "";								// <0x78abcf>|0x000|0x000:'32' // seems like something is different there
	m_on_content_downloaded = callback;					// <0x78abf8>|0x029|0x029:'33'
	// 1
	// 2
	// 3
	// 4
	// 5
	std::ostream request_stream ( &m_request_buff );	// <0x78ac0d>|0x03e|0x015:'39'
	request_stream << "GET ";							// <0x78ac21>|0x052|0x014:'40'
	request_stream << path;								// <0x78ac47>|0x078|0x026:'41'
	request_stream << " HTTP/1.0\r\n";					// <0x78ac6d>|0x09e|0x026:'42'
	request_stream << "Connection: close\r\n\r\n";		// <0x78ac7a>|0x0ab|0x00d:'43'
	tcp::resolver::query query(						// 1
		server,															// 2
		"http",															// 3
		tcp::resolver::query::address_configured );		// <0x78ac87>|0x0b8|0x00d:'47'
																		// 1
	m_resolver.async_resolve(											// 2
		query,															// 3
		boost::bind( &http_client::handle_resolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator )		// 4
	);													// <0x78ad08>|0x139|0x081:'52'
	// 1
}

// STATE[79%]
void http_client::on_error( boost::system::error_code const& err )
{
	LOG_ERROR( "http_client error: %s", err.message().c_str() );	// <0x78a127>|0x000|0x000:'58'
	close_connection( );											// <0x78a1e4>|0x0bd|0x0bd:'59'
	if ( m_on_error )												// <0x78a1ef>|0x0c8|0x00b:'60'
		m_on_error( err );											// <0x78a211>|0x0ea|0x022:'61'
}

// STATE[84%]: Logic is incorrect
void http_client::handle_resolve( boost::system::error_code const& err, tcp::resolver::iterator endpoint_iterator )
{
	if ( !err )																			// <0x78aa61>|0x000|0x000:'66'
	{																					// 1
																						// 2
																						// 3
		tcp::endpoint endpoint = *endpoint_iterator++;					// <0x78aa77>|0x016|0x016|[1]:'70'
		m_socket.async_connect(															// 1
			endpoint,																	// 2
			boost::bind( &http_client::handle_connect, this, boost::asio::placeholders::error, endpoint_iterator ) );	// <0x78aaab>|0x04a|0x034:'73'
	}																					// <0x78ab7a>|0x119|0x0cf:'74'
	else																				// 1
		on_error( err );																// <0x78ab7c>|0x11b|0x002:'76'
																						// 1
}

// STATE[98%]:
void http_client::handle_connect( boost::system::error_code const& err, tcp::resolver::iterator endpoint_iterator )
{
	if ( !err )																												// <0x78a831>|0x000|0x000:'82'
	{
		boost::asio::async_write(
			m_socket,
			m_request_buff,
			boost::bind( &http_client::handle_write_request, this, boost::asio::placeholders::error ) );					// <0x78a843>|0x012|0x012:'87'
	}
	else if ( endpoint_iterator != tcp::resolver::iterator( ) )																// <0x78a886>|0x055|0x043:'89'
	{
		m_socket.close();																									// <0x78a8e9>|0x0b8|0x063|[1]:'92'
		tcp::endpoint endpoint = *endpoint_iterator;																		// <0x78a8f7>|0x0c6|0x00e:'93'
		boost::asio::async_connect(
			m_socket,
			&endpoint,
			boost::bind( &http_client::handle_connect, this, boost::asio::placeholders::error, ++endpoint_iterator ) );		// <0x78a93d>|0x10c|0x046:'96'
	} 																														// <0x78aa0b>|0x1da|0x0ce:'97'
	else
	{
		on_error( err );																									// <0x78aa0d>|0x1dc|0x002:'99'
	}
}

// STATE[100%]
void http_client::handle_write_request( boost::system::error_code const& err )
{
	if ( !err )																								// <0x78a789>|0x000|0x000:'105'
		boost::asio::async_read_until(
			m_socket,
			m_response_buff,
			"\r\n",
			boost::bind( &http_client::handle_read_status_line, this, boost::asio::placeholders::error )	// <0x78a79b>|0x012|0x012:'110'
		);																									// <0x78a7fd>|0x074|0x062:'111'
	else
		on_error( err );																					// <0x78a7ff>|0x076|0x002:'113'

}

// STATE[78%]: Lot's of stuff got inlined differently. Functionally feels the same but I am not sure
void http_client::handle_read_status_line( boost::system::error_code const& err )
{
	if ( !err )																									// <0x78a3fa>|0x000|0x000:'119'
	{
		std::istream response_stream( &m_response_buff );														// <0x78a410>|0x016|0x016|[1]:'122'

		std::string status_message;																				// <0x78a466>|0x06c|0x056:'127'
		std::getline( response_stream, status_message );														// <0x78a4a1>|0x0a7|0x03b:'128'
		s32	found = status_message.find( "HTTP/" );																// <0x78a4b6>|0x0bc|0x015:'129' // @TODO: std::string::size_type
		if ( !response_stream || found != 0 )																	// <0x78a4cc>|0x0d2|0x016:'130'
		{
			LOG_ERROR( "http_client: Invalid response" );														// <0x78a4f0>|0x0f6|0x024:'132'
		} else																									// <0x78a57d>|0x183|0x08d:'133'
		{
			found = status_message.find( "200" );																// <0x78a595>|0x19b|0x018:'135'
			if ( found == status_message.npos )																	// <0x78a5ab>|0x1b1|0x016:'136'
			{
				LOG_ERROR( "http_client: Response returned with status code %s", status_message.c_str( ) );		// <0x78a5b5>|0x1bb|0x00a:'138'
			} else																								// <0x78a651>|0x257|0x09c:'139'
			{
				read_lines_from_stream( "read_status_line", m_response_buff );									// <0x78a669>|0x26f|0x018:'141'

				boost::asio::async_read(
					m_socket,
					m_response_buff,
					boost::asio::transfer_at_least( 1 ),
					boost::bind( &http_client::handle_read_content, this, boost::asio::placeholders::error )	// <0x78a682>|0x288|0x019:'151'
				);																								// <0x78a754>|0x35a|0x0d2:'152'
			}
		}
	} else
		on_error( err );																						// <0x78a769>|0x36f|0x015:'154'
}

// STATE[100%]
bool http_client::add_result_content( )
{
	std::istream response_stream( &m_response_buff );				// <0x789cb9>|0x000|0x000:'160'
	std::string str;												// <0x789d0f>|0x056|0x056:'161'
																	// 1
	for ( ; std::getline( response_stream, str ) && str != "\r" ; )	// <0x789d31>|0x078|0x022:'163'
	{																// 1
		m_result_content.append( str );								// <0x789dab>|0x0f2|0x07a:'165'
																	// 1
	}																// <0x789ddc>|0x123|0x031:'167'
																	// 1
	return m_result_content.size( ) < 1024;							// <0x789de1>|0x128|0x005:'169'
}

// STATE[99%]. Different register used once
void http_client::close_connection( )
{
	if ( m_socket.is_open( ) )	// <0x789f9f>|0x000|0x000:'174'
		m_socket.close( );		// <0x789fc6>|0x027|0x027:'175'

	m_on_content_downloaded( );	// <0x789fd4>|0x035|0x00e:'177'
}

// STATE[100%]
void http_client::handle_read_content( boost::system::error_code const& err )
{
	if ( !err )																								// <0x78a249>|0x000|0x000:'182'
	{																		// 1
		if ( add_result_content( ) )																		// <0x78a25f>|0x016|0x016:'184'
		{
			boost::asio::async_read(
				m_socket,
				m_response_buff,
				boost::asio::transfer_at_least( 1 ),
				boost::bind( &http_client::handle_read_content, this, boost::asio::placeholders::error )	// <0x78a272>|0x029|0x013:'190'
			);																								// <0x78a2fc>|0x0b3|0x08a:'191'
		} else
			close_connection( );																			// <0x78a2fe>|0x0b5|0x002:'192'
	} else if ( err != boost::asio::error::eof )															// <0x78a30b>|0x0c2|0x00d:'193'
	{
		on_error( err );																					// <0x78a363>|0x11a|0x058:'195'
	} else if ( err == boost::asio::error::eof )															// <0x78a371>|0x128|0x00e:'196'
	{
		add_result_content( );																				// <0x78a3bf>|0x176|0x04e:'198'
		close_connection( );																				// <0x78a3c7>|0x17e|0x008:'199'
	}
}

} // namespace network_core
} // namespace vostok