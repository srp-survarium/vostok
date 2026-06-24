////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_SPECIALIZATIONS_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_SPECIALIZATIONS_H_INCLUDED

namespace survarium {

// STATE[STUB]
// claude@NOTE: PARKED - show/hide new_state specializations (3 stmts, lines 32/33/47;
// target 0x98010 show). Recovered shape: compute two animation time-scales from
// `animations[0]` (lines 32/33) via resources::pinned_ptr_base<animation::
// cubic_spline_skeleton_animation const> over the show/hide animation, take their
// difference / ratio into the "shown progress" arg, then line 47:
//   return sounds ? new ( buffer.c_ptr( ) ) weapon_sound_events_handler_state< STATE >(
//       params->weapon, <scale>, animations, animations_count, sounds, sounds_count,
//       config_parameters.stop_sounds_on_state_finalize,
//       config_parameters.simultaneous_sounds_queue_size, shown ) : NULL;
// (the 9-arg ctor with bool& shown). BLOCKED: that 9-arg ctor inlines
// weapon_sound_effect::weapon_sound_effect (out-of-line in weapon_sound_effect.cpp) -
// cross-unit inline wall, plus the exact pinned_ptr time-scale computation is not yet
// decoded. NEXT: decode the two pinned_ptr reads (animation duration accessors) and
// write the ternary; the 9-arg ctor must first be matched by its own unit.
template < >
inline weapon_sound_events_handler_state< weapon_core_show_state >* weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state> >::new_state(
	mutable_buffer		buffer,
	weapon_state_creation_params const* const	params,
	resources::managed_resource_ptr const* const	animations,
	const u8			animations_count,
	void* const			sounds,
	const u8			sounds_count,
	weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_show_state > >::config_params const&	config_parameters
)
{
	return NULL;
}

// STATE[STUB] - PARKED, same shape/blocker as the show specialization above (target 0x98180).
template < >
inline weapon_sound_events_handler_state< weapon_core_hide_state >* weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_hide_state> >::new_state(
	mutable_buffer		buffer,
	weapon_state_creation_params const* const	params,
	resources::managed_resource_ptr const* const	animations,
	const u8			animations_count,
	void* const			sounds,
	const u8			sounds_count,
	weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_hide_state > >::config_params const&	config_parameters
)
{
	return NULL;
}

// STATE[STUB] - PARKED, same shape/blocker as the show specialization above (target 0x982e0).
template < >
inline weapon_sound_events_handler_state< pistol_weapon_core_show_state >* weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::pistol_weapon_core_show_state> >::new_state(
	mutable_buffer		buffer,
	weapon_state_creation_params const* const	params,
	resources::managed_resource_ptr const* const	animations,
	const u8			animations_count,
	void* const			sounds,
	const u8			sounds_count,
	weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_show_state > >::config_params const&	config_parameters
)
{
	return NULL;
}

// STATE[STUB] - PARKED, same shape/blocker as the show specialization above (target 0x98440).
template < >
inline weapon_sound_events_handler_state< pistol_weapon_core_hide_state >* weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::pistol_weapon_core_hide_state> >::new_state(
	mutable_buffer		buffer,
	weapon_state_creation_params const* const	params,
	resources::managed_resource_ptr const* const	animations,
	const u8			animations_count,
	void* const			sounds,
	const u8			sounds_count,
	weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_hide_state > >::config_params const&	config_parameters
)
{
	return NULL;
}

// STATE[STUB] - PARKED, same shape/blocker as the show specialization above (target 0x985a0).
template < >
inline weapon_sound_events_handler_state< double_barreled_weapon_core_show_state >* weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::double_barreled_weapon_core_show_state> >::new_state(
	mutable_buffer		buffer,
	weapon_state_creation_params const* const	params,
	resources::managed_resource_ptr const* const	animations,
	const u8			animations_count,
	void* const			sounds,
	const u8			sounds_count,
	weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_show_state > >::config_params const&	config_parameters
)
{
	return NULL;
}

// STATE[STUB] - PARKED, same shape/blocker as the show specialization above (target 0x98700).
template < >
inline weapon_sound_events_handler_state< double_barreled_weapon_core_hide_state >* weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::double_barreled_weapon_core_hide_state> >::new_state(
	mutable_buffer		buffer,
	weapon_state_creation_params const* const	params,
	resources::managed_resource_ptr const* const	animations,
	const u8			animations_count,
	void* const			sounds,
	const u8			sounds_count,
	weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< double_barreled_weapon_core_hide_state > >::config_params const&	config_parameters
)
{
	return NULL;
}

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_SPECIALIZATIONS_H_INCLUDED
