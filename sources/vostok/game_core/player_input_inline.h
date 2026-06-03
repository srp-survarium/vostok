////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_INPUT_INLINE_H_INCLUDED
#define PLAYER_INPUT_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
inline bool player_input::is_sprinting( ) const
{	// sushi@TODO: Mask constants
	 return ( actions_mask & 0x200 ) != 0
		&& ( actions_mask & 0x001 ) != 0
		&& ( actions_mask & 0x16E ) == 0;

	// FUNCTION BODY[0xbafb0]: 2
	// <0>
	// <0xbafb9>|0x009|+0x044:'22'
	// ******
}

} // namespace survarium

#endif // #ifndef PLAYER_INPUT_INLINE_H_INCLUDED
