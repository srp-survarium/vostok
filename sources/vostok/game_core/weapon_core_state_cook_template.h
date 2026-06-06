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
}

namespace survarium {

struct weapon_state_creation_params;

template < typename T >
class weapon_core_state_cook_template : public resources::unmanaged_cook {
	// temp_include_all.cpp anchors reach the private new_object directly.
	friend void ::vostok::use_game_core_weapon_core_idle_state( );
	friend void ::vostok::use_game_core_weapon_core_aimed_state( );
	friend void ::vostok::use_game_core_pistol_weapon_core_idle_state( );
	friend void ::vostok::use_game_core_pistol_weapon_core_show_state( );
	friend void ::vostok::use_game_core_pistol_weapon_core_hide_state( );
	friend void ::vostok::use_game_core_weapon_core_show_state( );
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
public:
	inline	explicit	weapon_core_state_cook_template		( );
	virtual				~weapon_core_state_cook_template	( );

public:
	virtual	mutable_buffer	allocate_resource		( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist ) override;
	virtual	void			deallocate_resource		( void* arg_0 ) override;

	virtual	void			create_resource			( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer ) override;
	virtual	void			destroy_resource		( resources::unmanaged_resource* resource ) override;

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
