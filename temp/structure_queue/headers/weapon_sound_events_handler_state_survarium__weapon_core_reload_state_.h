////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED

/* INCLUDES */
class survarium::weapon_core_reload_state;
class survarium::weapon_sound_effect;
class vostok::resources::managed_resource;

/* FORWARD REFS */
class survarium::weapon;

namespace survarium {

class weapon_sound_events_handler_state< weapon_core_reload_state > : public weapon_core_reload_state {
public:
	inline			weapon_sound_events_handler_state<survarium::weapon_core_reload_state>(
						weapon&			weapon,
						const float		animation_time_scale,
						resources::managed_resource_ptr const* const	animations,
						const u8		animations_count,
						void* const		sounds_buffer,
						const u8		sounds_count,
						bool			stop_sounds_on_state_finalize,
						u8				simultaneous_sounds_queue_size
					) { /* no source */ }
	inline			weapon_sound_events_handler_state<survarium::weapon_core_reload_state>(
						weapon&			arg_0,
						const float		arg_1,
						resources::managed_resource_ptr const* const	arg_2,
						const u8		arg_3,
						void* const		arg_4,
						const u8		arg_5,
						bool			arg_6,
						u8				arg_7,
						bool&			arg_8
					) { /* no source */ }

	virtual	void	initialize	( ) override { /* no source */ }
	virtual	void	finalize	( ) override { /* no source */ }

	virtual			~weapon_sound_events_handler_state<survarium::weapon_core_reload_state>( ) { /* no source */ }

private:
	/* 0x0000 */	/* weapon_core_reload_state */
	/* 0x0168 */	weapon_sound_effect		m_sound_effect;
	/* 0x0190 */	pcvoid					m_buffer_for_sounds;
}; // class weapon_sound_events_handler_state< weapon_core_reload_state >

STATIC_SIZE_ASSERT(weapon_sound_events_handler_state< weapon_core_reload_state >, 0x198);

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED
