////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_NETWORK_TCP_PACKET_CLIENT_H_INCLUDED
#define VOSTOK_NETWORK_TCP_PACKET_CLIENT_H_INCLUDED

#include <vostok/network/api.h>
#include <vostok/network_core/client_error_codes_enum.h>

namespace vostok {

namespace network_core {
	class packet_reader;
	class tcp_packet;
	class tcp_packet_client;
} // namespace network_core

namespace network {

struct world;
class network_world;

// the evolved 2012 `client`: same deferred-order pimpl, the impl is now
// network_core::tcp_packet_client
class VOSTOK_NETWORK_API tcp_packet_client : private core::noncopyable {
public:
	explicit	tcp_packet_client		( world& world );
				~tcp_packet_client		( );

	void		connect					( pcstr host, u16 port );

	void		disconnect				( );

	void		send					( network_core::tcp_packet const& packet );

	void		set_on_packet_received	(
					boost::function< void ( network_core::packet_reader& ) > const&	on_packet_received
				);
	void		set_on_connected		( boost::function< void ( ) > const& on_connected );
	void		set_on_disconnected		( boost::function< void ( ) > const& on_disconnected );
	void		set_on_error			(
					boost::function< void ( enum network_core::client_error_codes_enum, boost::system::error_code ) > const&	on_error
				);

	// claude@MATCH: private - all the on_*/create_client/connect_impl symbols
	// mangle AAE (private non-const) in the target
private:
	void		on_packet_received		( network_core::tcp_packet const& packet );
	void		on_packet_received_impl	( network_core::packet_reader& reader );
	void		on_connected			( );
	void		on_connected_impl		( );
	void		on_disconnected			( );
	void		on_disconnected_impl	( );
	void		on_error				(
					const network_core::client_error_codes_enum	client_error_code,
					const boost::system::error_code		error_code
				);
	void		on_error_impl			(
					const network_core::client_error_codes_enum	client_error_code,
					const boost::system::error_code		error_code
				);

	void		create_client			( );

	void		connect_impl			( pcstr host, u16 port );

private:
	boost::function< void ( network_core::packet_reader& ) >	m_on_packet_received;
	boost::function< void ( ) >			m_on_connected;
	boost::function< void ( ) >			m_on_disconnected;
	boost::function< void ( enum network_core::client_error_codes_enum, boost::system::error_code ) >	m_on_error;
	network_world&						m_world;
	network_core::tcp_packet_client*	m_client;
}; // class tcp_packet_client

STATIC_SIZE_ASSERT(tcp_packet_client, 0x88);

} // namespace network
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_TCP_PACKET_CLIENT_H_INCLUDED
