// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_CORE_COOK_H_INCLUDED
#define WEAPON_CORE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

class weapon_core;
struct weapon_state_creation_params;

class weapon_core_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
	typedef resources::translate_query_cook super;
	// sushi@TODO: Recover the core registration list; the retained weapon_cook list belongs to game, not this helper.
	static	void	register_cooks_for_logic_states( ) { /* no source */ }

	explicit		weapon_core_cook			( );

public:
	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource				( resources::resource_base* resource ) override;

protected:
	virtual	void	on_weapon_config_loaded		( resources::queries_result& data );

protected:
			void	process_loading_weapon_core	( resources::query_result_for_cook* const parent, configs::binary_config_ptr config_ptr,		weapon_core* object_to_cook );
			void	load_weapon_parameters		(											configs::binary_config_ptr config_ptr,		weapon_core* object_to_cook );

private:
			void	on_core_subresources_ready	( resources::queries_result& data,			configs::binary_config_ptr config_ptr,		weapon_core* object_to_cook );
	// sushi@TODO: Recover this callback's query/continuation; the live pipeline loads skeleton resources, not skeleton config.
	inline	void	on_skeleton_config_loaded	( resources::queries_result& data,			configs::binary_config_ptr config_ptr,		weapon_core* object_to_cook ) { /* no source */ }
			void	query_weapon_states			( resources::query_result_for_cook* const parent, configs::binary_config_ptr config_ptr,		weapon_core* object_to_cook );
			void	on_weapon_states_ready		( resources::queries_result& data,			weapon_state_creation_params const* params, weapon_core* object_to_cook );

private:
	virtual	u32		cooked_object_size			( weapon_core& object_to_cook ) const;
}; // class weapon_core_cook

STATIC_SIZE_ASSERT(weapon_core_cook, 0x20);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_COOK_H_INCLUDED
