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
// consuming asm
enum query_info_types
{
};

} // namespace lobby

#endif // #ifndef LOBBY_ENUMS_H_INCLUDED
