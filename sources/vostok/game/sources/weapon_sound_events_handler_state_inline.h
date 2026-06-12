////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_INLINE_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_INLINE_H_INCLUDED

namespace survarium {

// carcass exemplar: the weapon_sound_events_handler_state< weapon_core_reload_state > instance
// STATE[STUB]
template < typename T >
inline weapon_sound_events_handler_state< T >::weapon_sound_events_handler_state(
	weapon&			weapon,
	const float		animation_time_scale,
	resources::managed_resource_ptr const* const	animations,
	const u8		animations_count,
	void* const		sounds_buffer,
	const u8		sounds_count,
	bool			stop_sounds_on_state_finalize,
	u8				simultaneous_sounds_queue_size
)
{
	// FUNCTION BODY[0xa4230]: 0
	// <0xa4230>|0x000|+0x098:'40'	{
	// <0xa42c8>|0x098|      :'41'	}
	// ******
}

// carcass exemplar: the weapon_sound_events_handler_state< weapon_core_show_state > instance
// STATE[STUB]
template < typename T >
inline weapon_sound_events_handler_state< T >::weapon_sound_events_handler_state(
	weapon&			weapon,
	const float		animation_time_scale,
	resources::managed_resource_ptr const* const	animations,
	const u8		animations_count,
	void* const		sounds_buffer,
	const u8		sounds_count,
	bool			stop_sounds_on_state_finalize,
	u8				simultaneous_sounds_queue_size,
	bool&			shown
)
{
	// FUNCTION BODY[0x98e70]: 0
	// <0x98e70>|0x000|+0x09d:'72'	{
	// <0x98f0d>|0x09d|      :'73'	}
	// ******
}

// carcass exemplar: the weapon_sound_events_handler_state< weapon_core_shotgun_reload_one_round_substate > instance
// STATE[STUB]
template < typename T >
void weapon_sound_events_handler_state< T >::initialize( )
{
	// FUNCTION BODY[0xa1da0]: 3
	// <0xa1dae>|0x00e|+0x005:'79'
	// <0>
	// <0xa1db3>|0x013|+0x05d:'81'
	// ******
}

// carcass exemplar: the weapon_sound_events_handler_state< weapon_core_show_state > instance
// STATE[STUB]
template < typename T >
void weapon_sound_events_handler_state< T >::finalize( )
{
	// FUNCTION BODY[0x98fa0]: 2
	// <0x98fa4>|0x004|+0x017:'87'
	// <0x98fbb>|0x01b|+0x029:'88'
	// ******
}

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_INLINE_H_INCLUDED
