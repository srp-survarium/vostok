////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_HTTP_CLIENT_H_INCLUDED
#define NETWORK_CORE_HTTP_CLIENT_H_INCLUDED

#include <boost/function.hpp>

#include <boost/asio.hpp>

namespace vostok {
namespace network_core {

class http_client {
public:
			explicit				http_client				( boost::asio::io_service& io_service );

			void					get						( pcstr server, pcstr path, boost::function< void() > const& callback );

	// STATE[STUB]
	inline	void					set_on_error			( boost::function< void( boost::system::error_code ) > const& callback )
	{
		// FUNCTION BODY[0x59070]
		// <0x59070>|0x000|      :'22'	{
		// ******
	}

	inline	std::string const&		result_content			( ) const { return m_result_content; }

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

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_HTTP_CLIENT_H_INCLUDED
