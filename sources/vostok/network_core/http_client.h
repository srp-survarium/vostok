////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_HTTP_CLIENT_H_INCLUDED
#define NETWORK_CORE_HTTP_CLIENT_H_INCLUDED

#include <vostok/network_core/api.h>
#include <boost/function.hpp>

// clear the engine's Windows macros before boost/asio re-includes the WinSDK headers,
// so this header survives being force-included outside the module pch (e.g. game_core's
// temp_include_all anchor), where _WIN32_WINNT/APIENTRY/etc. would otherwise collide.
#	undef BOOL
#	undef APIENTRY
#	undef HMODULE
#	undef HWND
#include <boost/asio.hpp>

namespace vostok {
namespace network_core {

class http_client {
public:
			explicit				http_client				( boost::asio::io_service& io_service );

			void					get						( pcstr server, pcstr path, boost::function< void() > const& callback );

	inline	void					set_on_error			( boost::function< void( boost::system::error_code ) > const& callback ) { m_on_error = callback; }

	inline	std::string const&		result_content			( ) const { return m_result_content; }

private:
			void					handle_resolve			(
										boost::system::error_code const&	err,
										boost::asio::ip::tcp::resolver::iterator	endpoint_iterator
									);
			void					handle_connect			(
										boost::system::error_code const&	err,
										boost::asio::ip::tcp::resolver::iterator	endpoint_iterator
									);
			void					handle_write_request	( boost::system::error_code const& err );
			void					handle_read_status_line	( boost::system::error_code const& err );
			void					handle_read_content		( boost::system::error_code const& err );

			void					on_error				( boost::system::error_code const& err );

			bool					add_result_content		( );

			void					close_connection		( );

public:
	inline							~http_client			( ) { /* no source */ }

private:
	/* 0x0000 */	boost::asio::ip::tcp::resolver		m_resolver;
	/* 0x000c */	boost::asio::ip::tcp::socket		m_socket;
	/* 0x0050 */	boost::asio::streambuf				m_request_buff;
	/* 0x0080 */	boost::asio::streambuf				m_response_buff;
	/* 0x00b0 */	std::string							m_result_content;
	/* 0x00c8 */	boost::function< void() >			m_on_content_downloaded;
	/* 0x00e8 */	boost::function< void( boost::system::error_code ) >	m_on_error;
}; // class http_client

STATIC_SIZE_ASSERT(http_client, 0x108);

// free helper defined in http_client.cpp; declared so the game_core anchor can call it
VOSTOK_NETWORK_CORE_API void read_lines_from_stream( pcstr prefix, boost::asio::streambuf& buff );

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_HTTP_CLIENT_H_INCLUDED
