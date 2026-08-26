////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED

#include "weapon_sound_effect.h"

// complete weapon-state types for the per-instantiation STATIC_SIZE_ASSERTs below
#include <vostok/game_core/weapon_core_aimed_fire_state.h>
#include <vostok/game_core/weapon_core_chamber_a_round_aimed_state.h>
#include <vostok/game_core/weapon_core_chamber_a_round_state.h>
#include <vostok/game_core/weapon_core_fire_state.h>
#include <vostok/game_core/weapon_core_hide_state.h>
#include <vostok/game_core/weapon_core_reload_state.h>
#include <vostok/game_core/weapon_core_show_state.h>
#include <vostok/game_core/weapon_core_shotgun_reload_finish_substate.h>
#include <vostok/game_core/weapon_core_shotgun_reload_one_round_substate.h>
#include <vostok/game_core/weapon_core_shotgun_reload_start_substate.h>
#include <vostok/game_core/pistol_weapon_core_aimed_fire_state.h>
#include <vostok/game_core/pistol_weapon_core_fire_state.h>
#include <vostok/game_core/pistol_weapon_core_hide_state.h>
#include <vostok/game_core/pistol_weapon_core_reload_state.h>
#include <vostok/game_core/pistol_weapon_core_show_state.h>
#include <vostok/game_core/double_barreled_weapon_core_aimed_fire_state.h>
#include <vostok/game_core/double_barreled_weapon_core_fire_state.h>
#include <vostok/game_core/double_barreled_weapon_core_hide_state.h>
#include <vostok/game_core/double_barreled_weapon_core_reload_state.h>
#include <vostok/game_core/double_barreled_weapon_core_show_state.h>


namespace survarium {

class weapon;

template < typename T > class weapon_sound_events_handler_state_cook;

// T is one of the game_core weapon-state classes; the handler wraps it and
// plays the per-state weapon sounds. The 9-arg ctor (bool& shown) exists only
// for the show/hide instantiations in the target, declared for all T (union
// across the instantiation dumps - uninstantiated members cost nothing).
template < typename T >
class weapon_sound_events_handler_state : public T {
	// the cook over this state owns the sounds buffer (allocated in new_state,
	// freed in destroy_resource) and reads m_buffer_for_sounds directly.
	friend class weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< T > >;
	inline			weapon_sound_events_handler_state	(
						weapon&			weapon,
						const float		animation_time_scale,
						resources::managed_resource_ptr const* const	animations,
						const u8		animations_count,
						void* const		sounds_buffer,
						const u8		sounds_count,
						bool			stop_sounds_on_state_finalize,
						u8				simultaneous_sounds_queue_size
					);
	inline			weapon_sound_events_handler_state	(
						weapon&			weapon,
						const float		animation_time_scale,
						resources::managed_resource_ptr const* const	animations,
						const u8		animations_count,
						void* const		sounds_buffer,
						const u8		sounds_count,
						bool			stop_sounds_on_state_finalize,
						u8				simultaneous_sounds_queue_size,
						bool&			shown
					);

	virtual	void	initialize	( ) override;
	virtual	void	finalize	( ) override;

	typedef T super;
	enum {
		weapon_animations_count = super::weapon_animations_count,
		user_animations_count = super::user_animations_count,
		total_animations_count = super::total_animations_count
	};
	static resources::class_id_enum const resource_class = super::resource_class;

public:

private:
	/* 0x0000 */			/* T */
	/* sizeof(T) + 0x00 */	weapon_sound_effect		m_sound_effect;
	/* sizeof(T) + 0x28 */	pcvoid					m_buffer_for_sounds;
}; // class weapon_sound_events_handler_state

} // namespace survarium

#include "weapon_sound_events_handler_state_inline.h"

STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::weapon_core_aimed_fire_state >, 0x198);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::weapon_core_chamber_a_round_aimed_state >, 0x198);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::weapon_core_chamber_a_round_state >, 0x198);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::weapon_core_fire_state >, 0x198);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::weapon_core_hide_state >, 0x1A0);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::weapon_core_reload_state >, 0x198);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::weapon_core_show_state >, 0x1A0);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::weapon_core_shotgun_reload_finish_substate >, 0x1A0);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::weapon_core_shotgun_reload_one_round_substate >, 0x198);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::weapon_core_shotgun_reload_start_substate >, 0x1A0);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_aimed_fire_state >, 0x1B0);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_fire_state >, 0x1B0);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_hide_state >, 0x1B0);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_reload_state >, 0x1B8);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_show_state >, 0x1B0);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_aimed_fire_state >, 0x1B0);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_fire_state >, 0x1B0);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_hide_state >, 0x1C0);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_reload_state >, 0x1B8);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_show_state >, 0x1C0);

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_H_INCLUDED
