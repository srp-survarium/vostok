// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GAME_MATERIAL_MANAGER_COOK_H_INCLUDED
#define GAME_MATERIAL_MANAGER_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {
	struct material_effects_instance_cook_data;
}
}

namespace survarium {

class game_material_manager;
class material_pair;

class game_material_manager_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
	typedef resources::translate_query_cook super;

					game_material_manager_cook	( bool server_usage );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource				( resources::resource_base* res ) override;

private:
	enum res_type {
		decal1		= 0x0000,
		decal2		= 0x0001,
		sound		= 0x0002,
		particle	= 0x0003,
	};

	struct query_ext_data {
		/* 0x0000 */	material_pair*	pair;
		/* 0x0004 */	res_type		type;
		/* 0x0008 */	vostok::render::material_effects_instance_cook_data*	cd;
	}; // struct query_ext_data
	typedef vector< query_ext_data > ext_data_vec;

			void	on_configs_loaded			( resources::queries_result& data );
			void	on_decals_loaded			( resources::queries_result& data, ext_data_vec* ext_data );

			void	create_game_materials		(
						game_material_manager* const			manager,
						configs::binary_config_value const&		materials_root
					);
			void	create_game_material_pairs	(
						resources::query_result_for_cook&		parent_query,
						game_material_manager* const			manager,
						configs::binary_config_value const&		pairs_root
					);
private:
	/* 0x0020 */	bool	m_server_usage;
}; // class game_material_manager_cook

STATIC_SIZE_ASSERT(game_material_manager_cook, 0x24);

} // namespace survarium

#endif // #ifndef GAME_MATERIAL_MANAGER_COOK_H_INCLUDED
