////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MATCH_CLIENT_H_INCLUDED
#define MATCH_CLIENT_H_INCLUDED

#include <boost/function.hpp>
#include <vostok/login_server/message_types.h>	// connection/handshaking/socket/lobby_server error enums
#include <vostok/network/match_client.h>
#include <vostok/network_core/disconnect_event_types_enum.h>
#include <vostok/game_core/game_net_defines.h>	// match_options (value member)
#include <vostok/game_core/network_messages.h>	// network_packets_orderer + the match message-type enums

namespace vostok {
namespace network {
	struct world;
} // namespace network
namespace network_core {
	class packet_reader;
	class udp_match_packet;
	struct udp_match_stats;
} // namespace network_core
} // namespace vostok

namespace survarium {

class network_client;

class match_client : public boost::noncopyable {
	// network_client::on_connected_to_match drives the private send-queue flag
	// directly (codegen-neutral friendship; PDB does not record it)
	friend class network_client;
public:
			explicit							match_client						( network::world& world );
												~match_client						( );

			void								connect								(
													pcstr		host,
													u16			port,
													u32			session_id,
													u32			current_time_in_ms,
													boost::function< void( enum connection_error_types_enum, enum handshaking_error_types_enum, enum socket_error_types_enum, enum lobby_server_message_types_enum ) > const&	callback
												);

			void								disconnect							( );

			void								enqueue								( network_core::udp_match_packet* packet );

			void								send_queued_packets					( const u32 current_time_in_ms );

	inline	bool								are_there_any_packets_to_send		( ) const { /* no source */ return m_are_there_any_packets_to_send; }

	inline	u32									last_send_queed_packets_time_in_ms	( ) const { /* no source */ return m_last_send_queed_packets_time_in_ms; }

	inline	network_core::udp_match_packet*		new_packet							( match_client_message_types_enum arg_0 ) { return m_client.new_packet( ( u8 )arg_0 ); }
	inline	void								delete_packet						( network_core::udp_match_packet*& arg_0 ) { /* no source */ }

	inline	void								set_on_packet_received				( boost::function< void( u8, network_core::packet_reader& ) > const& arg_0 ) { m_client.set_on_packet_received( arg_0 ); }
	inline	void								set_on_disconnect					(
													boost::function< void( enum network_core::disconnect_event_types_enum ) > const&	arg_0
												) { m_client.set_on_disconnect( arg_0 ); }

	inline	bool								is_connected						( ) const { return m_client.is_connected( ); }
	inline	bool								is_disconnected						( ) const { return m_client.is_disconnected( ); }

	inline	network_core::udp_match_stats const&	get_stats							( ) const { return m_client.get_stats( ); }

	inline	u32									last_receive_time_in_ms				( ) const { return m_client.last_receive_time_in_ms( ); }

	inline	match_options&						get_match_options					( ) { /* no source */ return m_match_options; }

	inline	void								on_connected						(
													const connection_error_types_enum		arg_0,
													const handshaking_error_types_enum		arg_1,
													const socket_error_types_enum			arg_2,
													const lobby_server_message_types_enum	arg_3
												) { /* no source */ }

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	network::match_client		m_client;
	/* 0x00f8 */	network_packets_orderer< enum vostok::match_client_message_types_enum, enum vostok::match_server_message_types_enum >	m_packets_orderer;
	/* 0x00fc */	match_options				m_match_options;
	/* 0x2398 */	u32							m_last_send_queed_packets_time_in_ms;
	/* 0x239c */	bool						m_are_there_any_packets_to_send;
}; // class match_client

STATIC_SIZE_ASSERT(match_client, 0x23A0);

} // namespace survarium

#endif // #ifndef MATCH_CLIENT_H_INCLUDED
