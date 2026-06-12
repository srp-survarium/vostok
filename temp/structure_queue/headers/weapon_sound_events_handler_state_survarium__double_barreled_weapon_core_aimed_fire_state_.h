////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED

/* INCLUDES */
class survarium::double_barreled_weapon_core_aimed_fire_state;
class survarium::weapon_sound_effect;
class vostok::resources::managed_resource;

/* FORWARD REFS */
class survarium::weapon;

namespace survarium {

class weapon_sound_events_handler_state< double_barreled_weapon_core_aimed_fire_state > : public double_barreled_weapon_core_aimed_fire_state {
public:
	inline			weapon_sound_events_handler_state<survarium::double_barreled_weapon_core_aimed_fire_state>(
						weapon&			arg_0,
						const float		arg_1,
						resources::managed_resource_ptr const* const	arg_2,
						const u8		arg_3,
						void* const		arg_4,
						const u8		arg_5,
						bool			arg_6,
						u8				arg_7
					) { /* no source */ }
	inline			weapon_sound_events_handler_state<survarium::double_barreled_weapon_core_aimed_fire_state>(
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

	virtual			~weapon_sound_events_handler_state<survarium::double_barreled_weapon_core_aimed_fire_state>( ) { /* no source */ }

private:
	/* 0x0000 */	/* double_barreled_weapon_core_aimed_fire_state */
	/* 0x0180 */	weapon_sound_effect		m_sound_effect;
	/* 0x01a8 */	pcvoid					m_buffer_for_sounds;
}; // class weapon_sound_events_handler_state< double_barreled_weapon_core_aimed_fire_state >

STATIC_SIZE_ASSERT(weapon_sound_events_handler_state< double_barreled_weapon_core_aimed_fire_state >, 0x1B0);

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED
