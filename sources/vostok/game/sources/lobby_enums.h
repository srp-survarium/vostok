////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef LOBBY_ENUMS_H_INCLUDED
#define LOBBY_ENUMS_H_INCLUDED

// the global lobby:: namespace belongs to the original lobby-client glue
// module, absent from our tree (see messaging_enums.h for the pattern);
// parked under game with its survarium consumers until that module is rebuilt
namespace lobby {

// no PDB record (only referenced, as a parameter type, by survarium::lobby_menu /
// lobby_client) - enumerators unknown, a matcher recovers them from the
// consuming asm (query_prices/query_profile_contents pass the literal sub-id)
enum query_info_types
{
	query_profile_info		= 0x2,
	query_prices_info		= 0x6,
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
