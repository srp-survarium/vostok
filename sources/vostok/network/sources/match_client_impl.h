// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MATCH_CLIENT_IMPL_H_INCLUDED
#define MATCH_CLIENT_IMPL_H_INCLUDED

#include <boost/array.hpp>
#include <vostok/memory_single_size_buffer_allocator.h>
#include <vostok/network_core/udp_match_client.h>
#include <vostok/network_core/udp_match_types.h>
#include <vostok/network_core/disconnect_event_types_enum.h>
#include <vostok/login_server/message_types.h>

namespace vostok {

namespace network_core {
	class packet_reader;
	class udp_match_packet;
	class udp_network_flow_emulator;
	struct udp_network_flow_emulator_options;
} // namespace network_core

namespace network {

// NOTE: the PDB carries THREE layouts of this class (2048- and 8192-packet
// storage, udp_match_client 0xB20 vs 0xB28) from per-TU type drift in the
// original build; the ctor disassembly (0x776240: 0x2000 * 0x12C arena) and
// our matched network_core::udp_match_client (0xB28) pin the real one.
class match_client_impl {
public:
	enum state
	{
		waiting_for_permission		= 0x0,
		handshaked					= 0x1,
	}; // enum state

public:
								match_client_impl		(
									boost::asio::io_service&	io_service,
									network_core::udp_match_packets_orderer&	packets_orderer,
									network_core::udp_network_flow_emulator_options const*	options
								);
								~match_client_impl		( );

			void				connect					(
									pcstr		host,
									u16			port,
									u32			current_time_in_ms,
									network_core::udp_match_packet const*	packet,
									boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > const&	on_connected
								);

			void				disconnect				( );

	// Target emits this only inline in enqueue_impl.
	inline	void				enqueue					( network_core::udp_match_packet* packet )
	{
		m_client.enqueue	( packet );
	}

	// claude@MATCH: positive guard, no early return - the target's single `jne`
	// over the call (an `if ( x ) return;` shape emits je + jmp instead)
	inline	void				send_queued_packets		( const u32 current_time_in_ms )
	{
		if ( !is_disconnected( ) )
			m_client.send_queued_packets	( current_time_in_ms );
	}

	inline	bool				is_connected			( ) const { return m_client.is_connected( ); }
	inline	bool				is_disconnected			( ) const { return m_client.is_disconnected( ); }

	inline	void				set_on_disconnect		(
									boost::function< void ( enum network_core::disconnect_event_types_enum ) > const&	on_disconnect
								)
	{
		m_on_disconnect		= on_disconnect;
	}

			void				set_on_packet_received	(
									boost::function< void ( u8, network_core::packet_reader& ) > const&	on_packet_received
								);

			network_core::udp_match_packet*	clone_packet	( network_core::udp_match_packet const& packet );

	inline	network_core::udp_match_stats const&	get_stats				( ) const { return m_client.get_stats( ); }
	inline	u32					last_receive_time_in_ms	( ) const { return m_client.last_receive_time_in_ms( ); }

// the target manglings are AAE (private) for both callbacks
private:
			void				on_packet_received		( u8 message_type, network_core::packet_reader& reader );
			void				on_disconnect			( const network_core::disconnect_event_types_enum disconnect_type );

public:
	// Target emits this only inline in match_client::new_packet.
	static inline	void		construct_packet		(
									network_core::udp_match_packets_orderer&	packets_orderer,
									network_core::udp_match_packet&		packet,
									u8									message_type
								)
	{
		network_core::udp_match_connection::construct_packet( packets_orderer, packet, message_type );
	}

private:
	boost::array< char[300], 8192 >		m_packets_storage;
	memory::single_size_buffer_allocator< 300, threading::single_threading_policy >	m_packets_allocator;
	boost::function< void ( enum network_core::disconnect_event_types_enum ) >	m_on_disconnect;
	network_core::udp_network_flow_emulator*	m_network_flow_emulator;
	network_core::udp_match_client		m_client;
	boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) >	m_on_connected;
	boost::function< void ( u8, network_core::packet_reader& ) >	m_on_packet_received;
	state								m_state;
}; // class match_client_impl

STATIC_SIZE_ASSERT(match_client_impl, 0x258BA8);

} // namespace network
} // namespace vostok

#endif // #ifndef MATCH_CLIENT_IMPL_H_INCLUDED
