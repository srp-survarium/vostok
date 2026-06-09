////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_NETWORK_HTTP_CLIENT_H_INCLUDED
#define VOSTOK_NETWORK_HTTP_CLIENT_H_INCLUDED

#include <vostok/network/api.h>

namespace vostok {

namespace network_core {
	class http_client;
} // namespace network_core

namespace network {

struct world;
class network_world;

class VOSTOK_NETWORK_API http_client : public core::noncopyable {
public:
			explicit	http_client					( world& world );
						~http_client				( );

	// STATE[STUB]
	inline	void		set_on_error				( boost::function< void ( boost::system::error_code ) > const& on_error )
	{
		m_on_error			= on_error;
	}

			void		get							( pcstr server, pcstr path, boost::function< void ( pcstr ) > const& callback );

	// STATE[STUB]: trivial getter (buildability return)
	inline	bool		busy						( ) const { return m_busy; }

			void		get_impl					( pcstr server, pcstr path );
			void		create_client_impl			( );

			void		on_content_downloaded		( );
			void		on_content_downloaded_impl	( pcstr content );
			void		on_error					( boost::system::error_code error_code );
			void		on_error_impl				( boost::system::error_code error_code );

private:
	network_world&						m_world;
	boost::function< void ( pcstr ) >	m_on_content_downloaded;
	boost::function< void ( boost::system::error_code ) >	m_on_error;
	boost::function< void ( boost::system::error_code ) >	m_client_on_error;
	network_core::http_client*			m_client;
	bool								m_busy;
}; // class http_client

STATIC_SIZE_ASSERT(http_client, 0x70);

} // namespace network
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_HTTP_CLIENT_H_INCLUDED
