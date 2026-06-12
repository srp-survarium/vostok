////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED

/* INCLUDES */
class survarium::weapon_core_shotgun_reload_start_substate;
class survarium::weapon_sound_effect;
class vostok::resources::managed_resource;

/* FORWARD REFS */
class survarium::weapon;

namespace survarium {

class weapon_sound_events_handler_state< weapon_core_shotgun_reload_start_substate > : public weapon_core_shotgun_reload_start_substate {
public:
	inline			weapon_sound_events_handler_state<survarium::weapon_core_shotgun_reload_start_substate>(
						weapon&			arg_0,
						const float		arg_1,
						resources::managed_resource_ptr const* const	arg_2,
						const u8		arg_3,
						void* const		arg_4,
						const u8		arg_5,
						bool			arg_6,
						u8				arg_7
					) { /* no source */ }
	inline			weapon_sound_events_handler_state<survarium::weapon_core_shotgun_reload_start_substate>(
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

	virtual			~weapon_sound_events_handler_state<survarium::weapon_core_shotgun_reload_start_substate>( ) { /* no source */ }

private:
	/* 0x0000 */	/* weapon_core_shotgun_reload_start_substate */
	/* 0x0170 */	weapon_sound_effect		m_sound_effect;
	/* 0x0198 */	pcvoid					m_buffer_for_sounds;
}; // class weapon_sound_events_handler_state< weapon_core_shotgun_reload_start_substate >

STATIC_SIZE_ASSERT(weapon_sound_events_handler_state< weapon_core_shotgun_reload_start_substate >, 0x1A0);

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED
