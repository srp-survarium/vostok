////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_cook;
class vostok::const_buffer;
class vostok::resources::managed_resource;
class survarium::weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state> >::config_params;
class survarium::weapon_state_creation_params const* const;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::unmanaged_resource;
class survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state>;
class survarium::weapon_state_creation_params;

namespace survarium {

class weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_show_state > > : public resources::unmanaged_cook , public boost::noncopyable {
public:
	inline						weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state> >( ) { /* no source */ }

	virtual	mutable_buffer		allocate_resource		(
									resources::query_result_for_cook&		in_query,
									const_buffer							raw_file_data,
									bool									file_exist
								) override { /* no source */ }
	virtual	void				deallocate_resource		( void* buffer ) override { /* no source */ }
	virtual	void				create_resource			(
									resources::query_result_for_cook&		parent,
									const_buffer							raw_file_data,
									mutable_buffer							in_out_unmanaged_resource_buffer
								) override { /* no source */ }
	virtual	void				destroy_resource		( resources::unmanaged_resource* resource ) override { /* no source */ }

	inline	void				on_subresources_ready	(
									resources::queries_result&				data,
									mutable_buffer							buffer,
									weapon_state_creation_params const*		params,
									weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_show_state > >::config_params	config_parameters
								) { /* no source */ }

	inline	weapon_sound_events_handler_state< weapon_core_show_state >*	new_state				(
									mutable_buffer		buffer,
									weapon_state_creation_params const* const	params,
									resources::managed_resource_ptr const* const	animations,
									const u8			animations_count,
									void* const			sounds,
									const u8			sounds_count,
									weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_show_state > >::config_params const&	config_parameters
								) { /* no source */ }

	virtual						~weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_show_state> >( ) { /* no source */ }
}; // class weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_show_state > >

STATIC_SIZE_ASSERT(weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_show_state > >, 0x20);

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_H_INCLUDED
