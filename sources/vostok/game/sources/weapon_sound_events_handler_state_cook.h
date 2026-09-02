// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include <vostok/animation/api.h>
#include <vostok/animation/cubic_spline_skeleton_animation.h>
#include <vostok/game_core/weapon_state_creation_params.h>

#include "weapon_sound_events_handler_state.h"

namespace survarium {

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

float reload_animation_time_scale_calculator( resources::managed_resource_ptr const& reload_animation, weapon_state_creation_params const& params );
float fire_animation_time_scale_calculator( resources::managed_resource_ptr const& reload_animation, weapon_state_creation_params const& params );
float shotgun_reload_timescale_calculator( resources::managed_resource_ptr const& reload_animation, weapon_state_creation_params const& params );
float always_unit_timescale_calculator( resources::managed_resource_ptr const& arg_0, weapon_state_creation_params const& arg_1 );

typedef float ( &weapon_sound_events_handler_state_animation_time_scale_calculator_type )(
	resources::managed_resource_ptr const&,
	weapon_state_creation_params const&
);

template < typename T >
struct weapon_sound_events_handler_state_cook_traits;

#define DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( state_type, weapon_count, user_count ) \
	template < > struct weapon_sound_events_handler_state_cook_traits< weapon_sound_events_handler_state< state_type > > { \
		enum { weapon_animations_count = weapon_count, user_animations_count = user_count, animations_count = weapon_count + user_count }; \
		typedef weapon_sound_events_handler_state_animation_time_scale_calculator_type animation_time_scale_calculator_type; \
		static animation_time_scale_calculator_type animation_time_scale_calculator; \
	}

DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( weapon_core_show_state, 4, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( weapon_core_hide_state, 4, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( weapon_core_reload_state, 4, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( weapon_core_chamber_a_round_state, 4, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( weapon_core_chamber_a_round_aimed_state, 4, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( weapon_core_fire_state, 4, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( weapon_core_aimed_fire_state, 4, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( weapon_core_shotgun_reload_start_substate, 4, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( weapon_core_shotgun_reload_one_round_substate, 4, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( weapon_core_shotgun_reload_finish_substate, 4, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( pistol_weapon_core_show_state, 8, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( pistol_weapon_core_hide_state, 8, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( pistol_weapon_core_reload_state, 8, 8 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( pistol_weapon_core_fire_state, 8, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( pistol_weapon_core_aimed_fire_state, 8, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( double_barreled_weapon_core_show_state, 12, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( double_barreled_weapon_core_hide_state, 12, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( double_barreled_weapon_core_reload_state, 8, 8 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( double_barreled_weapon_core_fire_state, 8, 4 );
DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS( double_barreled_weapon_core_aimed_fire_state, 8, 4 );

#undef DECLARE_WEAPON_SOUND_STATE_COOK_TRAITS

// T is a weapon_sound_events_handler_state< state > instantiation
template < typename T >
class weapon_sound_events_handler_state_cook : public resources::unmanaged_cook , private boost::noncopyable {
public:
	typedef T type_to_create;

public:
	inline						weapon_sound_events_handler_state_cook	( );

	struct config_params {
public:
		inline	config_params( ) :
			stop_sounds_on_state_finalize( false ),
			simultaneous_sounds_queue_size( 1 )
		{ }

		/* 0x0000 */	bool	stop_sounds_on_state_finalize;
		/* 0x0001 */	u8		simultaneous_sounds_queue_size;
	}; // struct config_params

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

	typedef resources::unmanaged_cook super;

}; // class weapon_sound_events_handler_state_cook

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
