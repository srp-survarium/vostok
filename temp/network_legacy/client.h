////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_NETWORK_CLIENT_H_INCLUDED
#define VOSTOK_NETWORK_CLIENT_H_INCLUDED

#include <vostok/network/api.h>

namespace vostok {
namespace network {

class packet;
class packet_reader;
struct world;
class network_world;
class client_impl;

class VOSTOK_NETWORK_API client : private core::noncopyable {
public:
	typedef boost::function< void (packet_reader& ) >	client_on_packet_received_type;
	typedef boost::function< void ( ) >					client_on_connected_type;
	typedef boost::function< void ( ) >					client_on_disconnected_type;

public:
					client					( world& world );
					~client					( );
			void	connect					( pcstr host, u16 port );
			void	disconnect				( );
			void	send					( packet const* packet );
			void	set_on_packet_received	( client_on_packet_received_type const& on_packet_received );
			void	set_on_connected		( client_on_connected_type const& on_connected );
			void	set_on_disconnected		( client_on_disconnected_type const& on_disconnected );

private:
			void	on_packet_received		( vostok::network::packet const& packet );
			void	on_packet_received_impl	( vostok::network::packet const& packet );
			void	on_connected			( );
			void	on_connected_impl		( );
			void	on_disconnected			( );
			void	on_disconnected_impl	( );
			void	create_client			( );
	static	void	destroy_client			( client_impl* client );
			void	connect_impl			( pcstr host, u16 port );

private:
	client_on_packet_received_type	m_on_packet_received;
	client_on_connected_type		m_on_connected;
	client_on_disconnected_type		m_on_disconnected;
	network_world&					m_world;
	client_impl*					m_client;
}; // class client

} // namespace network
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_CLIENT_H_INCLUDED