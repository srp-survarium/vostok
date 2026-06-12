////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_cook;
class vostok::const_buffer;
class vostok::resources::managed_resource;
class survarium::weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_chamber_a_round_state> >::config_params;
class survarium::weapon_state_creation_params const* const;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::unmanaged_resource;
class survarium::weapon_sound_events_handler_state<survarium::weapon_core_chamber_a_round_state>;
class survarium::weapon_state_creation_params;

namespace survarium {

class weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_chamber_a_round_state > > : public resources::unmanaged_cook , public boost::noncopyable {
public:
	inline						weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_chamber_a_round_state> >( ) { /* no source */ }

	virtual	mutable_buffer		allocate_resource		( resources::query_result_for_cook& arg_0, const_buffer arg_1, bool arg_2 ) override { /* no source */ }
	virtual	void				deallocate_resource		( void* arg_0 ) override { /* no source */ }
	virtual	void				create_resource			( resources::query_result_for_cook& arg_0, const_buffer arg_1, mutable_buffer arg_2 ) override { /* no source */ }
	virtual	void				destroy_resource		( resources::unmanaged_resource* arg_0 ) override { /* no source */ }

	inline	void				on_subresources_ready	(
									resources::queries_result&				arg_0,
									mutable_buffer							arg_1,
									weapon_state_creation_params const*		arg_2,
									weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_chamber_a_round_state > >::config_params	arg_3
								) { /* no source */ }

	inline	weapon_sound_events_handler_state< weapon_core_chamber_a_round_state >*	new_state				(
									mutable_buffer		arg_0,
									weapon_state_creation_params const* const	arg_1,
									resources::managed_resource_ptr const* const	arg_2,
									const u8			arg_3,
									void* const			arg_4,
									const u8			arg_5,
									weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_chamber_a_round_state > >::config_params const&	arg_6
								) { /* no source */ }

	virtual						~weapon_sound_events_handler_state_cook<survarium::weapon_sound_events_handler_state<survarium::weapon_core_chamber_a_round_state> >( ) { /* no source */ }
}; // class weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_chamber_a_round_state > >

STATIC_SIZE_ASSERT(weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< weapon_core_chamber_a_round_state > >, 0x20);

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_H_INCLUDED
