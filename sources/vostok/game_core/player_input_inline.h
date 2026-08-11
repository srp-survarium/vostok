////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_INPUT_INLINE_H_INCLUDED
#define PLAYER_INPUT_INLINE_H_INCLUDED

namespace survarium {

inline bool player_input::is_sprinting( ) const
{
	return is_trying_to_sprint( )
		&& ( actions_mask & 0x001 ) != 0
		&& ( actions_mask & 0x16E ) == 0;
}

} // namespace survarium

#endif // #ifndef PLAYER_INPUT_INLINE_H_INCLUDED
