////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

#include "weapon_sound_events_handler_state.h"

namespace survarium {

struct weapon_state_creation_params;

// Per-state resource class id for the sound-cook ctor; specialized per wrapped
// logic state. Inlines to the literal in every ctor (no standalone symbol in the
// target). T is a weapon_sound_events_handler_state< state > instantiation.
template < typename T >	resources::class_id_enum	weapon_sound_events_handler_state_cook_class	( );

template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< weapon_core_show_state						> >( )	{ return resources::weapon_show_state_class;							}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< weapon_core_hide_state						> >( )	{ return resources::weapon_hide_state_class;							}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< weapon_core_reload_state						> >( )	{ return resources::weapon_reload_state_class;						}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< weapon_core_chamber_a_round_state			> >( )	{ return resources::weapon_chamber_a_round_state_class;				}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< weapon_core_chamber_a_round_aimed_state		> >( )	{ return resources::weapon_chamber_a_round_aimed_state_class;		}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< weapon_core_fire_state						> >( )	{ return resources::weapon_fire_state_class;							}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< weapon_core_aimed_fire_state					> >( )	{ return resources::weapon_aimed_fire_state_class;					}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< weapon_core_shotgun_reload_start_substate		> >( )	{ return resources::weapon_shotgun_reload_start_substate_class;		}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< weapon_core_shotgun_reload_one_round_substate	> >( )	{ return resources::weapon_shotgun_reload_one_substate_class;		}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< weapon_core_shotgun_reload_finish_substate	> >( )	{ return resources::weapon_shotgun_reload_finish_substate_class;		}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< double_barreled_weapon_core_show_state		> >( )	{ return resources::double_barreled_weapon_show_state_class;			}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< double_barreled_weapon_core_hide_state		> >( )	{ return resources::double_barreled_weapon_hide_state_class;			}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< double_barreled_weapon_core_reload_state		> >( )	{ return resources::double_barreled_weapon_reload_state_class;		}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< double_barreled_weapon_core_fire_state		> >( )	{ return resources::double_barreled_weapon_fire_state_class;			}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< double_barreled_weapon_core_aimed_fire_state	> >( )	{ return resources::double_barreled_weapon_aimed_fire_state_class;	}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< pistol_weapon_core_show_state					> >( )	{ return resources::pistol_weapon_show_state_class;					}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< pistol_weapon_core_hide_state					> >( )	{ return resources::pistol_weapon_hide_state_class;					}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< pistol_weapon_core_reload_state				> >( )	{ return resources::pistol_weapon_reload_state_class;				}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< pistol_weapon_core_fire_state					> >( )	{ return resources::pistol_weapon_fire_state_class;					}
template <>	inline	resources::class_id_enum	weapon_sound_events_handler_state_cook_class< weapon_sound_events_handler_state< pistol_weapon_core_aimed_fire_state			> >( )	{ return resources::pistol_weapon_aimed_fire_state_class;			}

// T is a weapon_sound_events_handler_state< state > instantiation
template < typename T >
class weapon_sound_events_handler_state_cook : public resources::unmanaged_cook , public boost::noncopyable {
public:
	struct config_params {
		inline	config_params	( ) { /* no source */ }

	public:
		/* 0x0000 */	bool	stop_sounds_on_state_finalize;
		/* 0x0001 */	u8		simultaneous_sounds_queue_size;
	}; // struct config_params

public:
	inline						weapon_sound_events_handler_state_cook	( );

private:
	// the four cook virtuals + the helpers mangle private (EAE in the target);
	// reached via the resources_manager vtable + the create_resource bind chain.
	virtual	mutable_buffer		allocate_resource		( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist ) override;
	virtual	void				deallocate_resource		( void* buffer ) override;
	virtual	void				create_resource			( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer ) override;
	virtual	void				destroy_resource		( resources::unmanaged_resource* resource ) override;

	inline	void				on_subresources_ready	(
									resources::queries_result&				data,
									mutable_buffer							buffer,
									weapon_state_creation_params const*	params,
									config_params							config_parameters
								);

	inline	T*					new_state				(
									mutable_buffer		buffer,
									weapon_state_creation_params const* const	params,
									resources::managed_resource_ptr const* const	animations,
									const u8			animations_count,
									void* const			sounds,
									const u8			sounds_count,
									config_params const&	config_parameters
								);

public:
	virtual						~weapon_sound_events_handler_state_cook	( ) { /* no source */ }
}; // class weapon_sound_events_handler_state_cook

// animation time-scale calculators (defined in weapon_sound_events_handler_state_cook.cpp)
float	reload_animation_time_scale_calculator	( resources::managed_resource_ptr const& reload_animation, weapon_state_creation_params const& params );
float	fire_animation_time_scale_calculator	( resources::managed_resource_ptr const& reload_animation, weapon_state_creation_params const& params );
float	shotgun_reload_timescale_calculator		( resources::managed_resource_ptr const& reload_animation, weapon_state_creation_params const& params );
float	always_unit_timescale_calculator		( resources::managed_resource_ptr const& arg_0, weapon_state_creation_params const& arg_1 );

} // namespace survarium

#include "weapon_sound_events_handler_state_cook_inline.h"
#include "weapon_sound_events_handler_state_cook_specializations.h"

STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_aimed_fire_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_chamber_a_round_aimed_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_chamber_a_round_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_fire_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_hide_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_reload_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_show_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_shotgun_reload_finish_substate > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_shotgun_reload_one_round_substate > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_shotgun_reload_start_substate > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_aimed_fire_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_fire_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_hide_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_reload_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_show_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_aimed_fire_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_fire_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_hide_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_reload_state > >, 0x20);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_show_state > >, 0x20);

STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_aimed_fire_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_chamber_a_round_aimed_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_chamber_a_round_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_fire_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_hide_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_reload_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_show_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_shotgun_reload_finish_substate > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_shotgun_reload_one_round_substate > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::weapon_core_shotgun_reload_start_substate > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_aimed_fire_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_fire_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_hide_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_reload_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::pistol_weapon_core_show_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_aimed_fire_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_fire_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_hide_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_reload_state > >::config_params, 0x2);
STATIC_SIZE_ASSERT(survarium::weapon_sound_events_handler_state_cook< survarium::weapon_sound_events_handler_state< survarium::double_barreled_weapon_core_show_state > >::config_params, 0x2);

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_H_INCLUDED
