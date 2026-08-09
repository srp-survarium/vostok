#include "pch.h"
#include "material_cook.h"
#include "material.h"

namespace vostok {
namespace render {
// 15 target lines are likely retail-compiled-out source.
material_cook::material_cook( ) : resources::translate_query_cook( resources::material_class, reuse_true, use_current_thread_id ) { }
void material_cook::translate_query(
	resources::query_result_for_cook& parent
)
{
	if ( parent.user_data() )
	{
		configs::binary_config_ptr			cfg;
		parent.user_data()->try_get			(cfg);
		on_material_binary_config_loaded	( &parent, cfg.c_ptr() );
	}
	else
	{
		fs_new::virtual_path_string			path;
		// 11 target lines are likely retail-compiled-out source.
		path.assignf						("resources/material_instances/%s.", parent.get_requested_path());

		resources::query_resource			(
			path.c_str(),
			resources::binary_config_class,
			boost::bind( &material_cook::on_material_config_loaded, this, _1 ),
			g_allocator,
			NULL, & parent
		);
	}

}
// 19 target lines are likely retail-compiled-out source.
void material_cook::on_material_config_loaded( resources::queries_result& result )
{
	resources::query_result_for_cook* const parent = result.get_parent_query();

	if(!result.is_successful())
	{
		parent->finish_query(result_error, assert_on_fail_false);
		return;
	}

	configs::binary_config* cfg			= static_cast_checked<configs::binary_config*>(result[0].get_unmanaged_resource().c_ptr());
	on_material_binary_config_loaded	( parent, cfg );
}

void material_cook::on_material_binary_config_loaded( resources::query_result_for_cook* parent, configs::binary_config* cfg )
{
	material* created_resource			= NEW(material)( cfg );
	created_resource->m_material_name	= parent->get_requested_path();

	parent->set_unmanaged_resource		( created_resource, resources::nocache_memory, sizeof(material) );
	parent->finish_query				( result_success );

}
void material_cook::delete_resource(
	resources::resource_base* resource )
{

	material* material_to_delete = static_cast_checked<material*>( resource ); DELETE( material_to_delete );
}


} // namespace render
} // namespace vostok
