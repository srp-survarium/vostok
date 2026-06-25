////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_H_INCLUDED
#define WEAPON_CORE_STATE_COOK_TEMPLATE_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
	void use_game_core_weapon_core_idle_state( );
	void use_game_core_weapon_core_aimed_state( );
	void use_game_core_pistol_weapon_core_idle_state( );
	void use_game_core_pistol_weapon_core_show_state( );
	void use_game_core_pistol_weapon_core_hide_state( );
	void use_game_core_weapon_core_show_state( );
	void use_game_core_double_barreled_weapon_core_show_state( );
	void use_game_core_double_barreled_weapon_core_hide_state( );
	void use_game_core_weapon_core_hide_state( );
	void use_game_core_double_barreled_weapon_core_idle_state( );
	void use_game_core_pistol_weapon_core_aimed_idle_state( );
	void use_game_core_double_barreled_weapon_core_aimed_idle_state( );
	void use_game_core_pistol_weapon_core_fire_state( );
	void use_game_core_pistol_weapon_core_reload_state( );
	void use_game_core_weapon_core_aimed_fire_state( );
	void use_game_core_pistol_weapon_core_aimed_fire_state( );
	void use_game_core_weapon_core_reload_state( );
	void use_game_core_weapon_core_fire_state( );
	void use_game_core_weapon_core_chamber_a_round_state( );
	void use_game_core_double_barreled_weapon_core_fire_state( );
	void use_game_core_weapon_core_chamber_a_round_aimed_state( );
	void use_game_core_double_barreled_weapon_core_aimed_fire_state( );
	void use_game_core_double_barreled_weapon_core_reload_state( );
}

namespace survarium {

struct weapon_state_creation_params;

class weapon_core_idle_state;
class weapon_core_aimed_state;
class double_barreled_weapon_core_idle_state;
class double_barreled_weapon_core_aimed_idle_state;
class pistol_weapon_core_idle_state;
class pistol_weapon_core_aimed_idle_state;

// Per-state resource class id for the cook ctor; specialized per logic state.
// Inlines to the literal in every ctor (no standalone symbol in the target).
template < typename T >	resources::class_id_enum	weapon_core_state_cook_class	( );

template <>	inline	resources::class_id_enum	weapon_core_state_cook_class< weapon_core_idle_state						>( )	{ return resources::weapon_idle_state_class;						}
template <>	inline	resources::class_id_enum	weapon_core_state_cook_class< weapon_core_aimed_state					>( )	{ return resources::weapon_aimed_state_class;						}
template <>	inline	resources::class_id_enum	weapon_core_state_cook_class< double_barreled_weapon_core_idle_state		>( )	{ return resources::double_barreled_weapon_idle_state_class;			}
template <>	inline	resources::class_id_enum	weapon_core_state_cook_class< double_barreled_weapon_core_aimed_idle_state	>( )	{ return resources::double_barreled_weapon_aimed_state_class;		}
template <>	inline	resources::class_id_enum	weapon_core_state_cook_class< pistol_weapon_core_idle_state				>( )	{ return resources::pistol_weapon_idle_state_class;					}
template <>	inline	resources::class_id_enum	weapon_core_state_cook_class< pistol_weapon_core_aimed_idle_state			>( )	{ return resources::pistol_weapon_aimed_state_class;				}

template < typename T >
class weapon_core_state_cook_template : public resources::unmanaged_cook {
	// temp_include_all.cpp anchors reach the private new_object directly.
	friend void ::vostok::use_game_core_weapon_core_idle_state( );
	friend void ::vostok::use_game_core_weapon_core_aimed_state( );
	friend void ::vostok::use_game_core_pistol_weapon_core_idle_state( );
	friend void ::vostok::use_game_core_pistol_weapon_core_show_state( );
	friend void ::vostok::use_game_core_pistol_weapon_core_hide_state( );
	friend void ::vostok::use_game_core_weapon_core_show_state( );
	friend void ::vostok::use_game_core_double_barreled_weapon_core_show_state( );
	friend void ::vostok::use_game_core_double_barreled_weapon_core_hide_state( );
	friend void ::vostok::use_game_core_weapon_core_hide_state( );
	friend void ::vostok::use_game_core_double_barreled_weapon_core_idle_state( );
	friend void ::vostok::use_game_core_pistol_weapon_core_aimed_idle_state( );
	friend void ::vostok::use_game_core_double_barreled_weapon_core_aimed_idle_state( );
	friend void ::vostok::use_game_core_pistol_weapon_core_fire_state( );
	friend void ::vostok::use_game_core_pistol_weapon_core_reload_state( );
	friend void ::vostok::use_game_core_weapon_core_aimed_fire_state( );
	friend void ::vostok::use_game_core_pistol_weapon_core_aimed_fire_state( );
	friend void ::vostok::use_game_core_weapon_core_reload_state( );
	friend void ::vostok::use_game_core_weapon_core_fire_state( );
	friend void ::vostok::use_game_core_weapon_core_chamber_a_round_state( );
	friend void ::vostok::use_game_core_double_barreled_weapon_core_fire_state( );
	friend void ::vostok::use_game_core_weapon_core_chamber_a_round_aimed_state( );
	friend void ::vostok::use_game_core_double_barreled_weapon_core_aimed_fire_state( );
	friend void ::vostok::use_game_core_double_barreled_weapon_core_reload_state( );
public:
	inline	explicit	weapon_core_state_cook_template		( );
	virtual				~weapon_core_state_cook_template	( );

public:
	virtual	mutable_buffer	allocate_resource		( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist ) override;
	virtual	void			deallocate_resource		( void* arg_0 ) override;

	virtual	void			create_resource			( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer ) override;
	virtual	void			destroy_resource		( resources::unmanaged_resource* resource ) override;

	// sushi@TODO: target mangles on_subresources_ready ?...@@AAE... (private) but it is
	// public (QAE) here, so the 6 cook instantiations stay unpaired. Moving it to the
	// private section below would let them pair, but the body is also /Od-divergent
	// (target folds the 4-iteration push_back loop into the `animations` decl statement;
	// our /Od keeps it separate), so they would pair at a low %. Belongs to the
	// weapon-state-cook unit owner (shared template, 12+ instantiations).
	inline	void			on_subresources_ready	( resources::queries_result& data, mutable_buffer buffer, weapon_state_creation_params const* params );

private:
	// new_object mangles ?new_object@...@@AAE... -> private (objdiff pairs by mangled name)
			T*				new_object				(
								mutable_buffer							buffer,
								weapon_state_creation_params const*		params,
								resources::managed_resource_ptr const*	animations,
								u32										animations_count
							);
}; // class weapon_core_state_cook_template

} // namespace survarium

#include <vostok/game_core/weapon_core_state_cook_template_inline.h>

#endif // #ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_H_INCLUDED
