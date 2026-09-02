// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "./animated_model_instance_cook.h"

#include <vostok/physics/model.h>
#include <vostok/physics/world.h>

namespace vostok {
namespace physics {

animated_model_instance_cook::animated_model_instance_cook( ):
	resources::translate_query_cook	(
		resources::physics_animated_model_instance_class,
		reuse_false,
		thread_id_unset
	),
	m_allocator						( g_ph_allocator )
{
	register_cook( this );
}

void animated_model_instance_cook::translate_request_path( pcstr request, fs_new::virtual_path_string& new_request ) const
{
	new_request.assignf(
		"resources/animated_model_instances/physics_animated_models/%s.physics_model",
		request
	);
}

void animated_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::query_resource			(
		parent.get_requested_path(),
		resources::binary_config_class,
		boost::bind( &animated_model_instance_cook::on_config_loaded, this, _1 ),
		m_allocator,
		0,
		&parent
	);
}

void animated_model_instance_cook::on_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	if ( !data.is_successful() )
	{
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr config					= static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource() );
	configs::binary_config_value const& config_value	= config->get_root();
	configs::binary_config_value const& attributes		= config_value["attributes"];

	pcstr skeleton_path									= attributes["skeleton"];
	fs_new::virtual_path_string								skeleton_config_path;
	skeleton_config_path.assignf						( "resources/models/%s.skinned_model/skeleton", skeleton_path );

	resources::query_resource							(
		skeleton_config_path.c_str(),
		resources::binary_config_class,
		boost::bind( &animated_model_instance_cook::on_skeleton_config_loaded, this, _1 ),
		m_allocator,
		0,
		parent
	);
}

void animated_model_instance_cook::on_skeleton_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	if ( !data.is_successful() )
	{
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr skeleton_config			= static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource() );
	configs::binary_config_value const& skeleton_value	= skeleton_config->get_root();

	R_ASSERT											( skeleton_value.value_exists( "skeleton" ) );
	pcstr skeleton_path									= skeleton_value["skeleton"];

	resources::request requests[] =
	{
		{ skeleton_path, resources::skeleton_class }
	};

	query_resources										(
		requests,
		array_size( requests ),
		boost::bind( &animated_model_instance_cook::on_subresources_loaded, this, _1 ),
		m_allocator,
		0,
		parent
	);
}

void animated_model_instance_cook::on_subresources_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent	= data.get_parent_query();
	if ( !data.is_successful() )
	{
		parent->finish_query						( result_error );
		return;
	}

	animated_model_instance* new_model_instance		= VOSTOK_NEW_IMPL( m_allocator, animated_model_instance );
	new_model_instance->m_skeleton					= static_cast_resource_ptr< animation::skeleton_ptr >( data[0].get_unmanaged_resource() );

	parent->set_unmanaged_resource					(
				new_model_instance,
				resources::memory_usage_type		( resources::nocache_memory, sizeof( animated_model_instance ) )
			);
	parent->finish_query							( result_success );
}

void animated_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( m_allocator, resource );
}

} // namespace physics
} // namespace vostok
