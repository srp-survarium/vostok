////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_input.h"

namespace survarium {

// STATE[STUB]
// survarium::player_input::player_input()
player_input::player_input( )
{
}

// STATE[STUB]
// void survarium::player_input::serialize(vostok::network_core::udp_match_packet&) const
void player_input::serialize( vostok::network_core::udp_match_packet& packet ) const
{
	// FUNCTION BODY
	// <0x700e89>|0x000|0x000:'23'
	// <0x700e94>|0x00b|0x00b:'24'
	// <0x700ea2>|0x019|0x00e:'25'
	// ******
}

// STATE[STUB]
// void survarium::player_input::deserialize(vostok::network_core::packet_reader&)
void player_input::deserialize( vostok::network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x700dfb>|0x000|0x000:'30'
	// <0x700e2d>|0x032|0x032:'31'
	// <0x700e60>|0x065|0x033:'32'
	// ******
}

// STATE[STUB]
// bool survarium::player_input::is_empty() const
bool player_input::is_empty( ) const
{
	return false;
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x700d69>|0x000|0x000:'56'
	// ******
}

} // namespace survarium
