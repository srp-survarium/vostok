#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_COOK_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include <vostok/resources_fs.h>

#include "material.h"
#include "material_effects_instance.h"
#include <vostok/render/facade/model.h>

namespace vostok {
namespace render {

struct speedtree_data {
	resources::query_result_for_cook* m_parent_query;
	material_ptr m_materials[5];
	material_effects_instance_ptr m_material_effects[5];
	fs_new::virtual_path_string m_material_path[5];
	speedtree_tree_base_ptr m_model;
	bool m_failed;

	bool all_assests_ready( ) { return false; }
};

STATIC_SIZE_ASSERT( speedtree_data, 0x598 );

struct speedtree_cook : public resources::translate_query_cook {
	speedtree_cook( );
	virtual ~speedtree_cook( ) { }

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void on_speedtree_raw_data_loaded( resources::queries_result& data, speedtree_data* creation_data );
	void on_model_materials_loaded( resources::queries_result& data, speedtree_data* cook_data );
	void on_material_effects_created( resources::queries_result&, speedtree_data* ) { }
	void query_materail_effects( speedtree_data* cook_data );
	void finish_model_creation( speedtree_data* cook_data );
};

STATIC_SIZE_ASSERT( speedtree_cook, 0x20 );

struct speedtree_instance_cook : public resources::translate_query_cook {
	speedtree_instance_cook( );
	virtual ~speedtree_instance_cook( ) { }

	virtual void translate_query( resources::query_result_for_cook& parent );
	virtual void delete_resource( resources::resource_base* resource );

private:
	void on_speedtree_loaded(
		resources::queries_result& data,
		resources::query_result_for_cook* parent_query
	);
};

STATIC_SIZE_ASSERT( speedtree_instance_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_SPEEDTREE_COOK_H_INCLUDED
