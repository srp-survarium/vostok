#include "pch.h"
#include "model_cooker.h"
#include <vostok/render/facade/model.h>
#include <vostok/render/static_model_instance_user_data.h>
#include "render_model_skeleton.h"

namespace vostok {
namespace render {
// 15 target lines are likely retail-compiled-out source.
static_model_instance_cook::static_model_instance_cook( ) : resources::translate_query_cook( resources::static_model_instance_class, reuse_false, use_current_thread_id )
{ resources::register_cook( this );
}
// 3 target lines are likely retail-compiled-out source.
void static_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{


	fs_new::virtual_path_string render_path;
	render_path.assignf( "%s.model/render", parent.get_requested_path( ) );


	u8 resource_count = 1;
	if ( parent.user_data( ) )
	{
		static_model_instance_user_data model_user_data;
		parent.user_data( )->try_get( model_user_data );
		if ( model_user_data.config->value_exists( "sound_environment" ) &&
			 strings::length( (pcstr)(*model_user_data.config)["sound_environment"] ) )
			resource_count = 2;
	}

	resources::request* requests = (resources::request*)ALLOCA( sizeof(resources::request) * resource_count );
	requests[0].path = render_path.c_str( );

	requests[0].id = resources::static_render_model_instance_class;
	resources::user_data_variant const** user_data = (resources::user_data_variant const**)ALLOCA( sizeof(resources::user_data_variant const*) * resource_count );
	for ( u8 i = 0; i < resource_count; ++i ) user_data[i] = 0;
	// 5 target lines are likely retail-compiled-out source.
	resources::user_data_variant sectors_environment_data;
	resources::user_data_variant sound_environment_data;
	if ( parent.user_data( ) ) { static_model_instance_user_data model_user_data;
		parent.user_data( )->try_get( model_user_data );
		if ( model_user_data.config->value_exists( "sectors" ) )
		{
			sectors_environment_data.set( (*model_user_data.config)["sectors"] );
			user_data[0] = &sectors_environment_data;
		}

		if ( model_user_data.config->value_exists( "sound_environment" ) &&
			 strings::length( (pcstr)(*model_user_data.config)["sound_environment"] ) )
		{
			requests[1].path = (pcstr)(*model_user_data.config)["sound_environment"];
			requests[1].id = resources::sound_environment_class;
			sound_environment_data.set( model_user_data );
			user_data[1] = &sound_environment_data;
		}
	}

	resources::query_resources(
		requests,
		resource_count,
		boost::bind( &static_model_instance_cook::on_subresources_loaded, this, _1, &parent ),
		g_allocator,
		user_data,
		&parent
	);
}


void static_model_instance_cook::on_subresources_loaded( resources::queries_result& data, resources::query_result_for_cook* parent_query )
{
	if ( data.is_successful( ) )
	{ static_model_instance* created_resource = NEW(static_model_instance)( );
		created_resource->m_render_model = static_cast_resource_ptr<render_model_instance_ptr>( data[0].get_unmanaged_resource( ) );
		created_resource->m_sound_environment = 0;

		if ( data.size( ) > 1 )
			created_resource->m_sound_environment = data[1].get_unmanaged_resource( );

		parent_query->set_unmanaged_resource( created_resource, resources::nocache_memory, sizeof(static_model_instance) );
		parent_query->finish_query( result_success );
	}
	else parent_query->finish_query( result_error );

}


void static_model_instance_cook::delete_resource( resources::resource_base* resource )
{ DELETE( resource );
}
// 3 target lines are likely retail-compiled-out source.
skeleton_model_instance_cook::skeleton_model_instance_cook( ) : resources::translate_query_cook( resources::skeleton_model_instance_class, reuse_false, use_current_thread_id )
{ resources::register_cook( this );
}


void skeleton_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{ skeleton_model_instance_cook_data* cook_data = NEW(skeleton_model_instance_cook_data)(&parent);

	fs_new::virtual_path_string skeleton_config_path;
	fs_new::virtual_path_string render_model_path;
	skeleton_config_path.assignf( "resources/models/%s.skinned_model/skeleton", parent.get_requested_path() );
	render_model_path.assignf( "%s.skinned_model/render", parent.get_requested_path() );
	resources::query_resource(
		skeleton_config_path.c_str(),
		resources::binary_config_class,
		boost::bind(
			&skeleton_model_instance_cook::on_skeleton_config_loaded, this, _1, cook_data ),
		vostok::render::g_allocator,
		0,
		&parent
	);
	resources::query_resource(
		render_model_path.c_str(),
		resources::skeleton_render_model_instance_class,
		boost::bind(
			&skeleton_model_instance_cook::on_render_model_loaded, this, _1, cook_data ),
		vostok::render::g_allocator,
		0,
		&parent
	);
}


void skeleton_model_instance_cook::on_skeleton_config_loaded( resources::queries_result& result, skeleton_model_instance_cook_data* cook_data )
{
	if (!result.is_successful())
	{
		cook_data->parent_query->finish_query( result_error );
		return;
	}

	configs::binary_config_ptr config = static_cast_resource_ptr<configs::binary_config_ptr>(result[0].get_unmanaged_resource());
	fs_new::virtual_path_string path = pcstr(config->get_root()["skeleton"]);

	resources::query_resource(
		path.c_str(),
		resources::skeleton_class,
		boost::bind( &skeleton_model_instance_cook::on_skeleton_loaded, this, _1, cook_data ),
		vostok::render::g_allocator,
		0,
		cook_data->parent_query
	);
}


void skeleton_model_instance_cook::on_skeleton_loaded( resources::queries_result& result, skeleton_model_instance_cook_data* cook_data )
{ if (!result.is_successful())
	{
		resources::query_result_for_cook* parent_query = cook_data->parent_query;
		parent_query->finish_query( result_error );
		return;
	}

	cook_data->skeleton = static_cast_resource_ptr<animation::skeleton_ptr>(result[0].get_unmanaged_resource());

	cook_data->skeleton_ready = true;

	if (cook_data->render_model_ready)
		on_all_subresources_ready(cook_data);
}


void skeleton_model_instance_cook::on_render_model_loaded( resources::queries_result& result, skeleton_model_instance_cook_data* cook_data )
{ if (!result.is_successful())
	{
		resources::query_result_for_cook* parent_query = cook_data->parent_query;
		parent_query->finish_query( result_error );
		return;
	}

	cook_data->render_model = static_cast_resource_ptr<render::render_model_instance_ptr>(result[0].get_unmanaged_resource());

	cook_data->render_model_ready = true;

	if (cook_data->skeleton_ready)
		on_all_subresources_ready( cook_data );
}


void skeleton_model_instance_cook::on_all_subresources_ready( skeleton_model_instance_cook_data* cook_data )
{

	resources::query_result_for_cook* parent_query = cook_data->parent_query;

	render::skeleton_model_instance* created_resource = NEW(render::skeleton_model_instance)();
	created_resource->m_render_model = cook_data->render_model;

	created_resource->m_skeleton = cook_data->skeleton;

	skeleton_render_model_instance* skel_mesh = static_cast_checked<skeleton_render_model_instance*>(created_resource->m_render_model.c_ptr());

	skel_mesh->m_bones_matrices.resize( skel_mesh->m_original->m_inverted_bones_matrices_in_bind_pose.size() );
	for (u32 i=0; i<skel_mesh->m_bones_matrices.size(); ++i)
		skel_mesh->m_bones_matrices[i] = math::float4x4().identity();
	parent_query->set_unmanaged_resource( created_resource, resources::nocache_memory, sizeof(render::skeleton_model_instance) );
	parent_query->finish_query( result_success );

	DELETE(cook_data);
}


void skeleton_model_instance_cook::delete_resource( resources::resource_base* resource )
{ DELETE( resource );
}

} // namespace render
} // namespace vostok
