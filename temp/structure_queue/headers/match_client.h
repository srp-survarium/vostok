////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MATCH_CLIENT_H_INCLUDED
#define MATCH_CLIENT_H_INCLUDED

/* INCLUDES */
enum const vostok::connection_error_types_enum;
enum const vostok::handshaking_error_types_enum;
enum const vostok::lobby_server_message_types_enum;
enum const vostok::socket_error_types_enum;
class vostok::network::match_client;
class survarium::network_packets_orderer<enum vostok::match_client_message_types_enum,enum vostok::match_server_message_types_enum>;
struct survarium::match_options;
enum vostok::match_client_message_types_enum;

/* FORWARD REFS */
class boost::function<void __cdecl(enum vostok::connection_error_types_enum,enum vostok::handshaking_error_types_enum,enum vostok::socket_error_types_enum,enum vostok::lobby_server_message_types_enum)>;
class boost::function<void __cdecl(enum vostok::network_core::disconnect_event_types_enum)>;
class boost::function<void __cdecl(unsigned char,vostok::network_core::packet_reader &)>;
class vostok::network::world;
class vostok::network_core::udp_match_packet;
class vostok::network_core::udp_match_stats;

namespace survarium {

class match_client : public boost::noncopyable {
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

	inline	bool								are_there_any_packets_to_send		( ) const { /* no source */ }

	inline	u32									last_send_queed_packets_time_in_ms	( ) const { /* no source */ }

	inline	network_core::udp_match_packet*		new_packet							( match_client_message_types_enum arg_0 ) { /* no source */ }
	inline	void								delete_packet						( network_core::udp_match_packet*& arg_0 ) { /* no source */ }

	inline	void								set_on_packet_received				( boost::function< void( u8, network_core::packet_reader& ) > const& arg_0 ) { /* no source */ }
	inline	void								set_on_disconnect					(
													boost::function< void( enum network_core::disconnect_event_types_enum ) > const&	arg_0
												) { /* no source */ }

	inline	bool								is_connected						( ) const { /* no source */ }
	inline	bool								is_disconnected						( ) const { /* no source */ }

	inline	network_core::udp_match_stats const&	get_stats							( ) const { /* no source */ }

	inline	u32									last_receive_time_in_ms				( ) const { /* no source */ }

	inline	match_options&						get_match_options					( ) { /* no source */ }

	inline	void								on_connected						(
													const connection_error_types_enum		arg_0,
													const handshaking_error_types_enum		arg_1,
													const socket_error_types_enum			arg_2,
													const lobby_server_message_types_enum	arg_3
												) { /* no source */ }

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	network::match_client		m_client;
	/* 0x00f8 */	network_packets_orderer< enum match_client_message_types_enum, enum match_server_message_types_enum >	m_packets_orderer;
	/* 0x00fc */	match_options				m_match_options;
	/* 0x2398 */	u32							m_last_send_queed_packets_time_in_ms;
	/* 0x239c */	bool						m_are_there_any_packets_to_send;
}; // class match_client

STATIC_SIZE_ASSERT(match_client, 0x23A0);

} // namespace survarium

#endif // #ifndef MATCH_CLIENT_H_INCLUDED
