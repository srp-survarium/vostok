#include "pch.h"
#include "animated_model_instance_cook.h"

#include <vostok/configs_binary_config_value.h>
#include <vostok/render/facade/model.h>

namespace vostok {
namespace render {

animated_model_instance_cook::animated_model_instance_cook( ) :
	resources::translate_query_cook(
		resources::render_animated_model_instance_class,
		reuse_false,
		use_resource_manager_thread_id
	)
{
}

void animated_model_instance_cook::translate_request_path(
	pcstr							request,
	fs_new::virtual_path_string&	new_request
) const
{
	new_request.assignf( "resources/animated_model_instances/render_animated_models/%s.render_model", request );
}

void animated_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::query_resource						(
		parent.get_requested_path(),
		resources::binary_config_class,
		boost::bind( &animated_model_instance_cook::on_config_loaded, this, _1 ),
		resources::unmanaged_allocator(),
		0,
		&parent
	);
}

void animated_model_instance_cook::on_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	if ( !data.is_successful() )
	{
		R_ASSERT										( data.is_successful(), "couldn't retrieve config for render animated model" );
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr config					= static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource() );
	configs::binary_config_value const& config_value	= config->get_root();
	configs::binary_config_value const& attributes		= config_value["attributes"];

	pcstr render_model_path								= attributes["render_model"];
	fs_new::virtual_path_string							skeleton_config_path;
	fs_new::virtual_path_string							model_config_path;
	skeleton_config_path.assignf						( "resources/models/%s.skinned_model/skeleton", render_model_path );
	model_config_path.assignf							( "resources/models/%s.skinned_model/settings", render_model_path );

	resources::request requests[] =
	{
		{ skeleton_config_path.c_str(), resources::binary_config_class },
		{ model_config_path.c_str(), resources::binary_config_class }
	};

	resources::query_resources						(
		requests,
		array_size( requests ),
		boost::bind( &animated_model_instance_cook::on_skeleton_config_loaded, this, _1, config ),
		resources::unmanaged_allocator(),
		0,
		parent
	);
}

void animated_model_instance_cook::on_skeleton_config_loaded(
	resources::queries_result&	data,
	configs::binary_config_ptr	config
)
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	if ( !data.is_successful() )
	{
		R_ASSERT										( data.is_successful(), "couldn't retrieve config for render animated model skeleton" );
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr skeleton_config			= static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource() );
	configs::binary_config_ptr model_config				= static_cast_resource_ptr<configs::binary_config_ptr>( data[1].get_unmanaged_resource() );
	configs::binary_config_value const& skeleton_value	= skeleton_config->get_root();
	configs::binary_config_value const& config_value	= config->get_root();
	configs::binary_config_value const& attributes		= config_value["attributes"];

	R_ASSERT											( skeleton_value.value_exists( "skeleton" ) );
	pcstr skeleton_path									= skeleton_value["skeleton"];

	R_ASSERT											( attributes.value_exists( "render_model" ) );
	pcstr render_model_path								= attributes["render_model"];
	fs_new::virtual_path_string							render_config_path;
	render_config_path.assignf							( "resources/models/%s.skinned_model/render", render_model_path );

	resources::request requests[] =
	{
		{ skeleton_path, resources::skeleton_class },
		{ render_config_path.c_str(), resources::skeleton_render_model_instance_class }
	};

	query_resources										(
		requests,
		array_size( requests ),
		boost::bind( &animated_model_instance_cook::on_subresources_loaded, this, _1, config ),
		resources::unmanaged_allocator(),
		0,
		parent
	);
}

void animated_model_instance_cook::on_subresources_loaded(
	resources::queries_result&	data,
	configs::binary_config_ptr	config
)
{
	resources::query_result_for_cook* const	parent	= data.get_parent_query();
	if ( !data.is_successful() )
	{
		R_ASSERT									( data.is_successful(), "couldn't cook some of render animated model subresources" );
		parent->finish_query						( result_error );
		return;
	}

	animation::skeleton_ptr skeleton					= static_cast_resource_ptr< animation::skeleton_ptr >( data[0].get_unmanaged_resource() );
	animated_model_instance* new_model_instance		= VOSTOK_NEW_IMPL( resources::unmanaged_allocator(), animated_model_instance );
	new_model_instance->m_model						= static_cast_resource_ptr< render_model_instance_ptr >( data[1].get_unmanaged_resource() );

	configs::binary_config_value const& config_value	= config->get_root();
	new_model_instance->m_hit_params					= config_value.value_exists( "hit_params" ) ?
		pcstr( config_value["hit_params"] ) : "human_hit_params";

	parent->set_unmanaged_resource					(
				new_model_instance,
				resources::memory_usage_type		( resources::nocache_memory, sizeof( animated_model_instance ) )
			);
	parent->finish_query							( result_success );
}

void animated_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	animated_model_instance* model_instance			= static_cast_checked< animated_model_instance* >( resource );
	VOSTOK_DELETE_IMPL								( resources::unmanaged_allocator(), model_instance );
}

} // namespace render
} // namespace vostok
