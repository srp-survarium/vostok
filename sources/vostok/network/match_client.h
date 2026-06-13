////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_NETWORK_MATCH_CLIENT_H_INCLUDED
#define VOSTOK_NETWORK_MATCH_CLIENT_H_INCLUDED

#include <vostok/network/api.h>
#include <vostok/login_server/message_types.h>
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/network_core/udp_match_packets_allocator.h>
#include <vostok/network_core/disconnect_event_types_enum.h>

namespace vostok {

// the game_core reachability anchor (temp_include_all.cpp); befriended below so
// it can call the private on_*/create_* members (free decls + friends emit no bytes)
void use_network_clients( );

namespace network_core {
	class packet_reader;
	class udp_match_packet;
	struct udp_match_packets_orderer;
	struct udp_network_flow_emulator_options;
} // namespace network_core

namespace network {

struct world;
class network_world;
class match_client_impl;

class VOSTOK_NETWORK_API match_client : public core::noncopyable {
public:
								match_client						(
									world&		world,
									network_core::udp_match_packets_orderer&	packets_orderer,
									network_core::udp_network_flow_emulator_options const*	options
								);
								~match_client						( );

			void				connect								(
									pcstr								host,
									u16									port,
									u32									current_time_in_ms,
									network_core::udp_match_packet*		packet,
									boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > const&	callback
								);

			void				disconnect							( );

			void				enqueue								( network_core::udp_match_packet* packet );

			void				send_queued_packets					( u32 current_time_in_ms );

			network_core::udp_match_packet*	new_packet				( u8 message_type );

	// STATE[STUB]
	inline	void				delete_packet						( network_core::udp_match_packet*& packet )
	{
		VOSTOK_UNREFERENCED_PARAMETER	( packet );
	}

	// STATE[STUB]
	inline	void				set_on_packet_received				( boost::function< void ( u8, network_core::packet_reader& ) > const& on_packet_received )
	{
		m_on_packet_received	= on_packet_received;
	}

	// STATE[STUB]
	inline	void				set_on_disconnect					(
									boost::function< void ( enum network_core::disconnect_event_types_enum ) > const&	on_disconnect
								)
	{
		m_on_disconnected		= on_disconnect;
	}

	// STATE[STUB]: trivial getter (buildability return)
	inline	network_core::udp_match_stats const&	get_stats		( ) const { return m_stats; }

			bool				is_connected						( ) const;
			bool				is_disconnected						( ) const;

			u32					last_receive_time_in_ms				( ) const;

	// STATE[STUB]
	inline	network_core::udp_match_packet*	new_response_packet		( )
	{
		return NULL;
	}

	// STATE[STUB]
	inline	void				delete_response_packet				( network_core::udp_match_packet*& packet )
	{
		VOSTOK_UNREFERENCED_PARAMETER	( packet );
	}

// the target manglings are AAE (private) for the whole create_*/on_* surface
private:
			void				create_client						( network_core::udp_network_flow_emulator_options const* options );
			void				create_responses_packets_allocator	( );

			void				on_connected						(
									connection_error_types_enum			connection_error,
									handshaking_error_types_enum		handshaking_error,
									socket_error_types_enum				socket_error,
									lobby_server_message_types_enum		lobby_error
								);
			void				on_packet_received_impl				( u8 message_type, network_core::packet_reader& reader );
			void				on_packet_received					( u8 message_type, network_core::packet_reader& reader );
			void				on_disconnect_impl					( network_core::disconnect_event_types_enum type );
			void				on_disconnect						( network_core::disconnect_event_types_enum type );

private:
	friend void ::vostok::use_network_clients( );

private:
	network_core::udp_match_packets_allocator_ptr	m_order_packets_allocator;
	network_core::udp_match_packets_allocator_ptr	m_response_packets_allocator;
	network_core::udp_match_stats		m_stats;
	boost::function< void ( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) >	m_on_connected;
	boost::function< void ( u8, network_core::packet_reader& ) >	m_on_packet_received;
	boost::function< void ( enum network_core::disconnect_event_types_enum ) >	m_on_disconnected;
	network_core::udp_match_packets_orderer&	m_packets_orderer;
	network_world&						m_world;
	match_client_impl**					m_client;
}; // class match_client

STATIC_SIZE_ASSERT(match_client, 0xF8);

} // namespace network
} // namespace vostok

#endif // #ifndef VOSTOK_NETWORK_MATCH_CLIENT_H_INCLUDED
