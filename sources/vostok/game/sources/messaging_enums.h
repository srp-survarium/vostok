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

// no PDB record (only referenced, as a parameter type, by survarium::lobby_menu /
// chat_handler / messaging_client) - enumerators unknown, a matcher recovers
// them from the consuming asm
enum friendship_actions_enum
{
};

} // namespace messaging

#endif // #ifndef MESSAGING_ENUMS_H_INCLUDED
