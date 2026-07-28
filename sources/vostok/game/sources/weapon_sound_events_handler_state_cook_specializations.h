////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_SPECIALIZATIONS_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_SPECIALIZATIONS_H_INCLUDED

namespace survarium {

// STATE[STUB]
// claude@NOTE: PARKED - show/hide new_state specializations (3 stmts, lines 32/33/47;
// target 0x98010 show). The 9-arg ctor wall is CLEARED (the ctor is now bodied + the
// weapon_core accessor map recovered, so on_sound_event/init/finalize all build). Line 47
// is ready: return sounds ? new ( buffer.c_ptr( ) ) weapon_sound_events_handler_state<
// STATE >( params->weapon, <scale>, animations, animations_count, sounds, sounds_count,
// config_parameters.stop_sounds_on_state_finalize,
// config_parameters.simultaneous_sounds_queue_size, shown ) : NULL.
// REMAINING WALL (lines 32/33): the <scale> arg is computed from TWO
// resources::pinned_ptr_base<animation::cubic_spline_skeleton_animation const> reads over
// animations[0]: each pins animations[0] then reads the cubic_spline keyframe arrays
// ([anim+0x14] base, then [base], [base+4] strides into the keyframe/value tables;
// movss [base+keyidx*..-4] = end value, movss [base+key0<<4] = start value), giving
// scale = (end_a - start_a) / (end_b - start_b) (subss/subss/divss). NEXT: decode the
// cubic_spline_skeleton_animation keyframe-table layout (the [+0x14]/[+0]/[+4] reads) to
// spell the two pinned-ptr duration expressions, then this whole 3-stmt body falls out.
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
