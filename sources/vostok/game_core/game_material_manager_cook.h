////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_MATERIAL_MANAGER_COOK_H_INCLUDED
#define GAME_MATERIAL_MANAGER_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {
	class material_effects_instance_cook_data;
}
}

namespace survarium {

class game_material_manager;
class material_pair;

class game_material_manager_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
					game_material_manager_cook	( bool server_usage );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;
	virtual	void	delete_resource				( resources::resource_base* res ) override;

			void	on_configs_loaded			( resources::queries_result& data );

	struct query_ext_data {
		enum res_type {
			decal1		= 0x0000,
			decal2		= 0x0001,
			sound		= 0x0002,
			particle	= 0x0003,
		};

		/* 0x0000 */	material_pair*									pair;
		/* 0x0004 */	game_material_manager_cook::res_type			type;
		/* 0x0008 */	render::material_effects_instance_cook_data*	cd;
	}; // struct query_ext_data

			void	on_decals_loaded			( resources::queries_result& data, vector< game_material_manager_cook::query_ext_data >* ext_data );

			void	create_game_materials		( game_material_manager* manager, configs::binary_config_value const& materials_root );
			void	create_game_material_pairs	( resources::query_result_for_cook& parent_query, game_material_manager* manager, configs::binary_config_value const& pairs_root );

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
	/* 0x0020 */	bool	m_server_usage;
}; // class game_material_manager_cook

STATIC_SIZE_ASSERT(game_material_manager_cook, 0x24);

} // namespace survarium

#endif // #ifndef GAME_MATERIAL_MANAGER_COOK_H_INCLUDED
