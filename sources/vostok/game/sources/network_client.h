////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CLIENT_H_INCLUDED
#define NETWORK_CLIENT_H_INCLUDED

#include <boost/array.hpp>
#include <boost/system/error_code.hpp>
#include <vostok/login_server/message_types.h>	// connection/handshaking/socket/login/lobby_server error enums
#include <vostok/network/http_client.h>
#include <vostok/network/login_client.h>
#include <vostok/network_core/disconnect_event_types_enum.h>
#include <vostok/network_core/udp_match_stats.h>
#include <vostok/game_core/client_player_update.h>

#include "base_network_client.h"
#include "flash_text.h"
#include "game_status.h"
#include "lobby_client.h"
#include "match_client.h"
#include "messaging_client.h"
#include "network_stats.h"	// stats_row value members
#include "player_desc.h"

namespace vostok {
namespace network_core {
	class packet_reader;
} // namespace network_core
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class camera_director;
class game;
class game_world;
struct hit_info;
struct match_options;
class player_input;

// void* network_client::`scalar deleting destructor'( u32 ) // FUNCTION BODY[0x921e0]: <0x921d0>|0x000|      :'74'	{

class network_client : public base_network_client {
public:
										network_client						( game& g, const bool is_spectator );
	virtual								~network_client						( );

	virtual	void						load								( pcstr map_name, camera_director const& director ) override
	{
		// target emits an UNguarded eater call (push director-ref; push map_name;
		// call <folded-empty>; add esp,8) - the VOSTOK_UNREFERENCED_PARAMETERS guard
		// (if(identity(false))) would DCE-collapse the whole block to a bare ret. The
		// direct helper call survives. &director keeps camera_director incomplete-OK.
		vostok::detail::unreferenced_parameter_helper( map_name, &director );
	}

	virtual	void						connect_to_login					(
											pcstr			host,
											const u16		port,
											pcstr			account_name,
											pcstr			account_password
										) override;

	virtual	void						disconnect							( ) override;

	virtual	void						tick								( u32 current_time_in_ms, const bool is_game_paused ) override;

	virtual	bool						has_bandwidth						( ) const override
	{
		return true;
	}

	virtual	void						send_local_player_input				(
											player_input const&		input,
											const u32				time_in_ms,
											float4x4 const&			transform,
											const float				look_pitch
										) override;

	virtual	void						initiate_kill_current_player		( ) override;

	virtual	void						initiate_respawn_current_player		( ) override
	{
	}

	virtual	bool						is_player_local						( const u8 player_id ) const override;

	virtual	void						unload								( ) override;

	virtual	network::login_client&		login_client						( ) override
	{
		return m_login_client;
	}

	virtual	class lobby_client&			lobby_client						( ) override
	{
		return m_lobby_client;
	}

	virtual	class match_client&			match_client						( ) override
	{
		return m_match_client;
	}

	virtual	class messaging_client&		messaging_client					( ) override
	{
		return m_messaging_client;
	}

	// the obvious source (m_match_client's options); a matcher confirms
	virtual	struct match_options&			match_options						( ) override { /* no source */ return m_match_client.get_match_options( ); }

	virtual	void						on_player_hit_received				( hit_info const& info ) override
	{
	}

	virtual	player_ptr					get_player							( const u8 id ) const override;
			game_world&					get_game_world						( );

	inline	player_ptr					get_local_player					( )
	{
		return m_local_player;
	}

			game_team_id				get_player_team						( pcstr player_profile_name );

	inline	bool						is_player_connected					( u32 arg_0 ) { /* no source */ return false; }

	// claude@MATCH: private from here - the process_*/on_*/draw_stats/query_players/
	// send_*/setup_camera_for_warmup/http_* symbols mangle AAE, and
	// apply_use_physics_controller_for_current/close_current_match mangle EAE
	// (private virtual); the dump prints everything public
private:
	virtual	void						apply_use_physics_controller_for_current( ) override;

	inline	void						on_player_death						( network_core::packet_reader& arg_0 ) { /* no source */ }

			void						draw_stats							( u32 current_time_in_ms );

			void						process_match_info					( network_core::packet_reader& reader );
			void						process_player_profile				( network_core::packet_reader& reader );
			void						process_team_bases					( network_core::packet_reader& reader );
			void						process_match_finished				( network_core::packet_reader& __formal );
			void						process_shop_action					( network_core::packet_reader& packet );

			void						query_players						( );

	inline	void						create_new_player					( network_core::packet_reader& arg_0 ) { /* no source */ }
	inline	void						destroy_player						( network_core::packet_reader& arg_0 ) { /* no source */ }
			void						destroy_player_impl					( const u8 id );

			void						process_player_action				( network_core::packet_reader& packet, const u32 time_in_ms );

			void						on_match_packet_received			( u8 message_type, network_core::packet_reader& packet );
			void						on_match_disconnected				( network_core::disconnect_event_types_enum disconnect_event_type );
			void						on_lobby_packet_received			( network_core::packet_reader& reader );

			void						process_player_hit					( network_core::packet_reader& packet );

			void						on_connected_to_match				(
											const connection_error_types_enum		arg_0 /* connection_error_types_enum connection_error */,
											const handshaking_error_types_enum		arg_1 /* handshaking_error_types_enum handshaking_error */,
											const socket_error_types_enum			arg_2 /* socket_error_types_enum socket_error */,
											const lobby_server_message_types_enum	arg_3 /* lobby_server_message_types_enum message_type */
										);
			void						on_connected_to_lobby				( );
			void						on_disconnected_from_lobby			( );
			void						on_connected_to_login				(
											const connection_error_types_enum		arg_0 /* connection_error_types_enum connection_error */,
											const handshaking_error_types_enum		arg_1 /* handshaking_error_types_enum handshaking_error */,
											const socket_error_types_enum			arg_2 /* socket_error_types_enum socket_error */,
											const login_server_message_types_enum	arg_3 /* login_server_message_types_enum message_type */
										);
			void						on_http_error						( boost::system::error_code __formal );
			void						on_players_ready					( resources::queries_result& data, u32 players_count );

			void						process_player_kill					( network_core::packet_reader& packet );
			void						process_player_respawn				( network_core::packet_reader& packet );
			void						process_initialize_victory_items	( network_core::packet_reader& packet );
			void						process_base_capture_progress		( network_core::packet_reader& packet );
			void						process_match_time					( network_core::packet_reader& packet );
			void						process_respawn_timer				( network_core::packet_reader& packet );
			void						process_match_wait_timer			( network_core::packet_reader& packet );
			void						process_game_status					( network_core::packet_reader& packet );
			void						process_player_kd_stats				( network_core::packet_reader& packet );
			void						process_victory_item_take_or_put	( network_core::packet_reader& packet );
			void						process_affect_damage_model			( network_core::packet_reader& packet );

			void						player_visibility_change			( network_core::packet_reader& packet );

			void						on_trap_placed						( network_core::packet_reader& packet );
			void						on_trap_removed						( network_core::packet_reader& packet );
			void						on_trap_fired						( network_core::packet_reader& packet );
			void						on_trap_disarmed					( network_core::packet_reader& packet );

			void						game_world_object_state_arrived		( network_core::packet_reader& reader );

			void						on_world_sync_request				( );

			void						damage_model_state_arrived			( network_core::packet_reader& packet );

			void						send_sync_request					( );

			void						process_sync_response				( network_core::packet_reader& packet );

			void						send_player_inputs					( );

			bool						http_query_server_connection_info	( u8 type );

			void						on_http_result_ready				( pcstr content, u8 type );

	virtual	void						close_current_match					( bool user_initiate ) override;

			void						setup_camera_for_warmup				( );

	typedef fixed_vector< client_player_update, 32 > player_inputs_type;

private:
	/* 0x0000 */	/* base_network_client */
	/* 0x0020 */	network::login_client				m_login_client;
	/* 0x0218 */	class lobby_client					m_lobby_client;
	/* 0x0a88 */	class match_client					m_match_client;
	/* 0x2e28 */	class messaging_client				m_messaging_client;
	/* 0x2fa0 */	network::http_client				m_http_client;
	/* 0x3010 */	network_core::udp_match_stats		m_previous_stats;
	/* 0x3090 */	player_inputs_type					m_player_inputs;
	/* 0x3c18 */	boost::array< player_desc, 20 >		m_net_players;
	/* 0x3cb8 */	player_ptr							m_local_player;
	/* 0x3cbc */	stats_row							m_sent;
	/* 0x3d7c */	stats_row							m_sent_low_level;
	/* 0x3e3c */	stats_row							m_resent;
	/* 0x3efc */	stats_row							m_received;
	/* 0x3fbc */	stats_row							m_received_low_level;
	/* 0x407c */	stats_row							m_received_duplicated;
	/* 0x413c */	flash_text							m_max_local_sequence_difference_caption;
	/* 0x4148 */	flash_text							m_max_local_sequence_difference_value;
	/* 0x4154 */	flash_text							m_unacknowledged_packets_caption;
	/* 0x4160 */	flash_text							m_unacknowledged_packets_value;
	/* 0x416c */	u32									m_last_tick_time_in_ms;
	/* 0x4170 */	u32									m_last_sync_request_time;
	/* 0x4174 */	u32									m_last_player_input_send_time_in_ms;
	/* 0x4178 */	u32									m_last_send_queued_packets_time_in_ms;
	/* 0x417c */	u32									m_server_latency;
	/* 0x4180 */	game_status							m_game_status;
	/* 0x4184 */	bool								m_is_spectator;
	/* 0x4185 */	bool								m_is_player_ticked;
	/* 0x4186 */	bool								m_is_time_synchronized_first_time;
}; // class network_client

STATIC_SIZE_ASSERT(network_client, 0x4188);

} // namespace survarium

#endif // #ifndef NETWORK_CLIENT_H_INCLUDED
