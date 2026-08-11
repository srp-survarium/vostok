////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_SPECIALIZATIONS_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_SPECIALIZATIONS_H_INCLUDED

namespace survarium {

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
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 0 ] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 4 ] )->length_in_frames( );
	return sounds ? new ( buffer.c_ptr( ) ) weapon_sound_events_handler_state< weapon_core_show_state >(
		static_cast< weapon& >( params->weapon ), weapon_anim_length / user_anim_length, animations, animations_count, sounds, sounds_count,
		config_parameters.stop_sounds_on_state_finalize, config_parameters.simultaneous_sounds_queue_size, params->shown
	) : NULL;
}

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
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 0 ] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 4 ] )->length_in_frames( );
	return sounds ? new ( buffer.c_ptr( ) ) weapon_sound_events_handler_state< weapon_core_hide_state >(
		static_cast< weapon& >( params->weapon ), weapon_anim_length / user_anim_length, animations, animations_count, sounds, sounds_count,
		config_parameters.stop_sounds_on_state_finalize, config_parameters.simultaneous_sounds_queue_size, params->shown
	) : NULL;
}

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
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 0 ] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 8 ] )->length_in_frames( );
	return sounds ? new ( buffer.c_ptr( ) ) weapon_sound_events_handler_state< pistol_weapon_core_show_state >(
		static_cast< weapon& >( params->weapon ), weapon_anim_length / user_anim_length, animations, animations_count, sounds, sounds_count,
		config_parameters.stop_sounds_on_state_finalize, config_parameters.simultaneous_sounds_queue_size, params->shown
	) : NULL;
}

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
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 0 ] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 8 ] )->length_in_frames( );
	return sounds ? new ( buffer.c_ptr( ) ) weapon_sound_events_handler_state< pistol_weapon_core_hide_state >(
		static_cast< weapon& >( params->weapon ), weapon_anim_length / user_anim_length, animations, animations_count, sounds, sounds_count,
		config_parameters.stop_sounds_on_state_finalize, config_parameters.simultaneous_sounds_queue_size, params->shown
	) : NULL;
}

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
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 0 ] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 12 ] )->length_in_frames( );
	return sounds ? new ( buffer.c_ptr( ) ) weapon_sound_events_handler_state< double_barreled_weapon_core_show_state >(
		static_cast< weapon& >( params->weapon ), weapon_anim_length / user_anim_length, animations, animations_count, sounds, sounds_count,
		config_parameters.stop_sounds_on_state_finalize, config_parameters.simultaneous_sounds_queue_size, params->shown
	) : NULL;
}

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
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 0 ] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[ 12 ] )->length_in_frames( );
	return sounds ? new ( buffer.c_ptr( ) ) weapon_sound_events_handler_state< double_barreled_weapon_core_hide_state >(
		static_cast< weapon& >( params->weapon ), weapon_anim_length / user_anim_length, animations, animations_count, sounds, sounds_count,
		config_parameters.stop_sounds_on_state_finalize, config_parameters.simultaneous_sounds_queue_size, params->shown
	) : NULL;
}

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_SPECIALIZATIONS_H_INCLUDED
