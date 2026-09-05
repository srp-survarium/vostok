// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_PHYSICS_COLLISION_SHAPE_COOK_H_INCLUDED
#define VOSTOK_PHYSICS_COLLISION_SHAPE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace physics {

class bt_collision_shape;

class collision_shape_cook : public resources::translate_query_cook {
private:
	struct cook_data  {
		/* 0x0000 */	resources::query_result_for_cook*	parent_query;
		/* 0x0004 */	fs_new::virtual_path_string			model_path;
		/* 0x0118 */	float3								scale_;
	};

typedef resources::translate_query_cook super;

public:
								collision_shape_cook		( bool static_object );

	virtual void				translate_query				( resources::query_result_for_cook& parent ) override;
	virtual void				delete_resource				( resources::resource_base* resource ) override;

private:
	// STATE[UNMATCHABLE]: translate_query's retail line program keeps its request
	// construction in the .cpp, and no other client caller expands this helper.
	inline	void				load_collision_resources	( collision_shape_cook::cook_data* arg_0 ) { /* no source */ }
			void				on_collision_sources_loaded	( resources::queries_result& data, collision_shape_cook::cook_data* cd );
			bt_collision_shape*	create_primitives_shape		( configs::binary_config_value const& primitives_t, collision_shape_cook::cook_data* cd );

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	bool	m_static_object;
}; // class collision_shape_cook

STATIC_SIZE_ASSERT(collision_shape_cook, 0x24);

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_COLLISION_SHAPE_COOK_H_INCLUDED
