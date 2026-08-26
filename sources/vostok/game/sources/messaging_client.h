////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MESSAGING_CLIENT_H_INCLUDED
#define MESSAGING_CLIENT_H_INCLUDED

#include <boost/system/error_code.hpp>
#include <vostok/network/tcp_packet_client.h>
#include <vostok/network_core/client_error_codes_enum.h>
#include <vostok/game_core/game_team_id.h>
#include <vostok/game_core/scheduler.h>	// scheduler::identifier value member

#include "account_list_item.h"
#include "messaging_enums.h"
#include "server_connection_info.h"

namespace vostok {
namespace network_core {
	class packet_reader;
} // namespace network_core
} // namespace vostok

namespace survarium {

class chat_handler;
class game;

class messaging_client : private boost::noncopyable {
public:
			explicit								messaging_client				( game& g );
													~messaging_client				( );

			void									connect							( server_connection_info const& connection_info );

			void									disconnect						( );

			void									on_message_typed				( wchar_t const* input_text, messaging::message_channel_enum message_chanel );

	inline	void									set_local_name					( pcstr arg_0 ) { /* no source */ }

			void									assign_match_channel_order		(
														const u32				match_id,
														const game_team_id		team_id
													);

			void									query_for_friend_list			( );
			void									query_for_friends_status		( );
			void									query_for_ignore_list			( );
			void									add_to_friend_list				( const u32 account_id );
			void									remove_from_friend_list			( const u32 account_id );
			void									add_to_ignore_list				( const u32 account_id );
			void									remove_from_ignore_list			( const u32 account_id );

			void									find_players_by_name			( pcstr player_name );

	inline	server_connection_info&					connection_info					( ) { /* no source */ return m_connection_info; }

	inline	vectora< account_list_item > const&		get_friend_list					( ) { /* no source */ return m_friend_list; }
	inline	vectora< account_list_item > const&		get_ignore_list					( ) { /* no source */ return m_ignore_list; }
	inline	vectora< account_list_item > const&		get_found_players_list			( ) { /* no source */ return m_found_players_list; }

	inline	bool									connected						( ) const { /* no source */ return false; }

	inline	pcstr									local_user_name					( ) { /* no source */ return m_local_name; }
	inline	game_team_id							local_player_team				( ) { /* no source */ return m_game_team_id; }

	// claude@MATCH: private from here - the sign_in_on_packet_received/on_*/
	// read_*/accept_message_from/process_incoming_text_message/
	// update_channel_subscriptions/parse_receiver_channel symbols mangle AAE
	// (the dump prints them public)
private:
			void									sign_in_on_packet_received		( network_core::packet_reader& reader );
			void									on_packet_received				( network_core::packet_reader& reader );
			void									on_connected					( );
			void									on_disconnected					( );
			void									on_error						(
														network_core::client_error_codes_enum	client_error_code,
														boost::system::error_code		system_error_code
													);

			bool									read_friend_list				( network_core::packet_reader& reader );
			bool									read_friend_status				( network_core::packet_reader& reader );
			bool									read_ignore_list				( network_core::packet_reader& reader );
			bool									read_found_players				( network_core::packet_reader& reader );

			void									process_incoming_text_message	( network_core::packet_reader& reader );

			bool									accept_message_from				( const u32 sender_account_id, messaging::client_type_enum sender_type );

			void									update_channel_subscriptions	( );

			messaging::message_channel_enum			parse_receiver_channel			( wchar_t const* w_receiver_name, const bool in_match );

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	game&							m_game;
	/* 0x0004 */	chat_handler&					m_chat_handler;
	/* 0x0008 */	server_connection_info			m_connection_info;
	/* 0x0088 */	messaging::client_state_enum	m_connection_state;
	/* 0x0090 */	network::tcp_packet_client		m_network_client;
	/* 0x0118 */	char							m_local_name[32];
	/* 0x0138 */	u32								m_match_channel_id_;
	/* 0x013c */	game_team_id					m_game_team_id;
	/* 0x0140 */	vectora< account_list_item >	m_friend_list;
	/* 0x0150 */	vectora< account_list_item >	m_ignore_list;
	/* 0x0160 */	vectora< account_list_item >	m_found_players_list;
	/* 0x0170 */	scheduler::identifier			m_scheduler_identifier;
}; // class messaging_client

STATIC_SIZE_ASSERT(messaging_client, 0x178);

} // namespace survarium

#endif // #ifndef MESSAGING_CLIENT_H_INCLUDED
