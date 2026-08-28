////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MESSAGING_ENUMS_H_INCLUDED
#define MESSAGING_ENUMS_H_INCLUDED

// the global messaging:: namespace belongs to the original messaging-client
// glue module, absent from our tree (same situation as vostok/scaleform -
// see scaleform_engine.h); parked under game with its survarium consumers
// (chat_handler, lobby_menu, messaging_client) until that module is rebuilt
namespace messaging {

// canonical: headers/others/enums/messaging__message_channel_enum.h
enum message_channel_enum
{
	server_message_channel		= 0x0,
	player_general_channel		= 0x1,
	player_system_channel		= 0x2,
	player_clan_channel			= 0x3,
	player_private_channel		= 0x4,
	player_match_channel		= 0x5,
	player_team1_channel		= 0x6,
	player_team2_channel		= 0x7,
	player_squad_channel		= 0x8,
	max_channel_num				= 0x9,
};

// canonical: headers/others/enums/messaging__friendship_actions_enum.h
enum friendship_actions_enum
{
	add_friend					= 0x0,
	remove_friend				= 0x1,
	add_ignorable				= 0x2,
	remove_ignorable			= 0x3,
	find_players				= 0x4,
	query_friend_list			= 0x5,
	query_ignore_list			= 0x6,
	update_friends_status		= 0x7,
};

// canonical: headers/others/enums/messaging__client_state_enum.h
// (messaging_client's m_connection_state member)
enum client_state_enum
{
	client_disconnecting	= 0x0,
	client_disconnected		= 0x1,
	client_connecting		= 0x2,
	client_connected		= 0x3,
};

// canonical: headers/others/enums/messaging__client_type_enum.h
// (messaging_client::accept_message_from's sender_type parameter)
enum client_type_enum
{
	unknown_client_type						= 0x0,
	login_server_client_type				= 0x1,
	lobby_server_client_type				= 0x2,
	match_server_client_type				= 0x3,
	message_server_client_type				= 0x4,
	account_client_type						= 0x5,
	administrative_client_type				= 0x6,
	match_maker_server_client_type			= 0x7,
	stats_processor_server_client_type		= 0x8,
};

// canonical: headers/others/messaging__send_message_params.h
// (process_incoming_text_message's params local)
struct send_message_params {
	/* 0x0000 */	u32						receiver_account_id;
	/* 0x0004 */	char					receiver_name[32];
	/* 0x0024 */	u32						sender_account_id;
	/* 0x0028 */	char					sender_name[32];
	/* 0x0048 */	client_type_enum		sender_type;
	/* 0x004c */	message_channel_enum	message_channel;
	/* 0x0050 */	char					( *message_body )[256];
};

STATIC_SIZE_ASSERT( send_message_params, 0x54 );

} // namespace messaging

#endif // #ifndef MESSAGING_ENUMS_H_INCLUDED
