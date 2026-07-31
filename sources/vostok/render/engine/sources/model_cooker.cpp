#include "pch.h"
#include "model_cooker.h"
#include <vostok/render/facade/model.h>
#include "render_model_skeleton.h"

namespace vostok {

namespace configs {
class binary_config_value;
}

namespace sound {
struct world;
}

namespace render {

struct static_model_instance_user_data {
	static_model_instance_user_data( ) { }
	~static_model_instance_user_data( ) { }

	configs::binary_config_value const* config;
	sound::world* sound_world;
	resources::unmanaged_resource_ptr sound_scene;
};

static_model_instance_cook::static_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::static_model_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// FUNCTION BODY[0x76c2d0]
}

// STATE[STUB]
// claude@NOTE: legacy body queried render + collision_geometry and stored the result in
// static_model_instance::m_collision_geom, which the canonical class replaced with
// m_sound_environment (see static_model_instance_user_data above) - diverged, left in
// temp/render_legacy for the matcher.
void static_model_instance_cook::translate_query( resources::query_result_for_cook& )
{
	// FUNCTION BODY[0x76cd40]
}

// STATE[STUB]
// claude@NOTE: same divergence as translate_query (m_collision_geom removed from the
// canonical static_model_instance).
void static_model_instance_cook::on_subresources_loaded(
	resources::queries_result&,
	resources::query_result_for_cook*
)
{
	// FUNCTION BODY[0x76c7a0]
}

void static_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x76c380]
	DELETE( resource );
}

skeleton_model_instance_cook::skeleton_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::skeleton_model_instance_class,
		reuse_false,
		use_current_thread_id
	)
{
	// FUNCTION BODY[0x76c260]
}

void skeleton_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY[0x76cb10]
	skeleton_model_instance_cook_data* cook_data = NEW(skeleton_model_instance_cook_data)(&parent);

	fs_new::virtual_path_string	skeleton_config_path;
	fs_new::virtual_path_string	render_model_path;
	skeleton_config_path.assignf	( "resources/models/%s.skinned_model/skeleton", parent.get_requested_path() );
	render_model_path.assignf		( "resources/models/%s.skinned_model/render", parent.get_requested_path() );

	resources::query_resource(
		skeleton_config_path.c_str(),
		resources::binary_config_class,
		boost::bind( &skeleton_model_instance_cook::on_skeleton_config_loaded, this, _1, cook_data ),
		vostok::render::g_allocator,
		0,
		& parent
	);

	resources::query_resource(
		render_model_path.c_str(),
		resources::skeleton_render_model_instance_class,
		boost::bind	(&skeleton_model_instance_cook::on_render_model_loaded, this, _1, cook_data ),
		vostok::render::g_allocator,
		0,
		& parent
	);
}

void skeleton_model_instance_cook::on_skeleton_config_loaded(
	resources::queries_result& result,
	skeleton_model_instance_cook_data* cook_data
)
{
	// FUNCTION BODY[0x76c9c0]
	resources::query_result_for_cook* parent_query = cook_data->parent_query;
	if (!result.is_successful())
	{
		parent_query->finish_query				( result_error );
		return;
	}

	configs::binary_config_ptr config = static_cast_resource_ptr<configs::binary_config_ptr>(result[0].get_unmanaged_resource());
	fs_new::virtual_path_string path = 			 pcstr(config->get_root()["skeleton"]);

	resources::query_resource(
		path.c_str(),
		resources::skeleton_class,
		boost::bind	(&skeleton_model_instance_cook::on_skeleton_loaded, this, _1, cook_data ),
		vostok::render::g_allocator,
		0,
		cook_data->parent_query
	);
}

void skeleton_model_instance_cook::on_skeleton_loaded(
	resources::queries_result& result,
	skeleton_model_instance_cook_data* cook_data
)
{
	// FUNCTION BODY[0x76c6b0]
	if (!result.is_successful())
	{
		resources::query_result_for_cook* parent_query = cook_data->parent_query;
		parent_query->finish_query				( result_error );
		return;
	}

	cook_data->skeleton = static_cast_resource_ptr<animation::skeleton_ptr>(result[0].get_unmanaged_resource());

	cook_data->skeleton_ready = true;

	if (cook_data->render_model_ready)
		on_all_subresources_ready(cook_data);
}

void skeleton_model_instance_cook::on_render_model_loaded(
	resources::queries_result& result,
	skeleton_model_instance_cook_data* cook_data
)
{
	// FUNCTION BODY[0x76c5c0]
	if (!result.is_successful())
	{
		resources::query_result_for_cook* parent_query = cook_data->parent_query;
		parent_query->finish_query ( result_error );
		return;
	}

	cook_data->render_model = static_cast_resource_ptr<render::render_model_instance_ptr>(result[0].get_unmanaged_resource());

	cook_data->render_model_ready = true;

	if (cook_data->skeleton_ready)
		on_all_subresources_ready( cook_data );
}

void skeleton_model_instance_cook::on_all_subresources_ready(
	skeleton_model_instance_cook_data* cook_data
)
{
	// FUNCTION BODY[0x76c3c0]
	resources::query_result_for_cook* parent_query = cook_data->parent_query;

	render::skeleton_model_instance* created_resource= NEW(render::skeleton_model_instance)();

	created_resource->m_render_model = cook_data->render_model;
	created_resource->m_skeleton	 = cook_data->skeleton;

	skeleton_render_model_instance* skel_mesh = static_cast_checked<skeleton_render_model_instance*>(created_resource->m_render_model.c_ptr());

	skel_mesh->m_bones_matrices.resize	( skel_mesh->m_original->m_inverted_bones_matrices_in_bind_pose.size() );

	for (u32 i=0; i<skel_mesh->m_bones_matrices.size(); ++i)
		skel_mesh->m_bones_matrices[i] = math::float4x4().identity();

	parent_query->set_unmanaged_resource	( created_resource, resources::nocache_memory, sizeof(render::skeleton_model_instance) );
	parent_query->finish_query				( result_success );

	DELETE(cook_data);
}

void skeleton_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x76c340]
	DELETE( resource );
}

} // namespace render
} // namespace vostok
