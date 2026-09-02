// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef LOBBY_ENUMS_H_INCLUDED
#define LOBBY_ENUMS_H_INCLUDED

// the global lobby:: namespace belongs to the original lobby-client glue
// module, absent from our tree (see messaging_enums.h for the pattern);
// parked under game with its survarium consumers until that module is rebuilt
namespace lobby {

// canonical: headers/others/enums/lobby__query_info_types.h
enum query_info_types
{
	q_client_state					= 0x0,
	q_enumerate_profiles			= 0x1,
	q_profile_contents				= 0x2,
	q_enumerate_inventory			= 0x3,
	q_profile_slots_restrictions	= 0x4,
	q_items_compatibility			= 0x5,
	q_price_items					= 0x6,
	q_account_money					= 0x7,
	q_player_skills					= 0x8,
	q_player_skills_tree			= 0x9,
	q_service_prices				= 0xa,
	q_player_reputations			= 0xb,
};

// canonical: headers/others/enums/lobby__client_state_enum.h (lobby_client's
// m_status member)
enum client_state_enum
{
	surf_lobby_menu				= 0x0,
	in_match_making_order		= 0x1,
	in_match_making				= 0x2,
	in_match					= 0x3,
	unknown						= 0x4,
};

} // namespace lobby

#endif // #ifndef LOBBY_ENUMS_H_INCLUDED
