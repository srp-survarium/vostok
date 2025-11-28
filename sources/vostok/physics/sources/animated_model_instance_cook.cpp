////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "./animated_model_instance_cook.h"

#include <vostok/physics/model.h>
#include <vostok/physics/world.h>

/*
 * This is heavily based on the original xr2 source code.
 * Match percentage is low, however.
 * This is because of how differently `intrusive_ptr` was compiled.
 * And I currently do not have fixes for this.
 * Hopefully the problem will solve itself once overall matching percentage is better.
 */

namespace vostok {
namespace physics {

// STATE[49%|PARTIAL]
animated_model_instance_cook::animated_model_instance_cook( ):
	resources::translate_query_cook	(
		resources::physics_animated_model_instance_class,
		reuse_false,
		thread_id_unset
	),
	m_allocator						( g_ph_allocator )
{
	register_cook( this );	// <0x72cd82>|0x000|0x000:'25'
}

// STATE[100%|DONE]
void animated_model_instance_cook::translate_request_path( pcstr request, fs_new::virtual_path_string& new_request ) const
{
	new_request.assignf(
		"resources/animated_model_instances/physics_animated_models/%s.physics_model",
		request
	); // <0x72cdb0>|0x000|0x000:'30'
}

// STATE[85%|DONE]
void animated_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::query_resource			(
		parent.get_requested_path(),
		resources::binary_config_class,
		boost::bind( &animated_model_instance_cook::on_config_loaded, this, _1 ),
		m_allocator,
		0,
		&parent
	); // <0x72d1cd>|0x000|0x000:'42'
}

// STATE[76%|DONE]
void animated_model_instance_cook::on_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();											// <0x72d05c>|0x000|0x000:'47'
	if ( !data.is_successful() )
	{
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr config					= static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource() );
	configs::binary_config_value const& config_value	= config->get_root();												// <0x72d0cb>|0x06f|0x042:'55'
	configs::binary_config_value const& attributes		= config_value["attributes"];

	pcstr skeleton_path									= attributes["skeleton"];
	fs_new::virtual_path_string								skeleton_config_path;
	skeleton_config_path.assignf						( "resources/models/%s.skinned_model/skeleton", skeleton_path );	// <0x72d0f3>|0x097|0x006:'60'

	resources::query_resource							(
		skeleton_config_path.c_str(),
		resources::binary_config_class,
		boost::bind( &animated_model_instance_cook::on_skeleton_config_loaded, this, _1 ),
		m_allocator,
		0,
		parent
	);																														// <0x72d125>|0x0c9|0x032:'69'
}

// STATE[76%|DONE]
// sushi@NOTE: Only skeleton config is left from xray, collision_config and ragdoll config were removed (as well with `config` argument)
void animated_model_instance_cook::on_skeleton_config_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	if ( !data.is_successful() )														// <0x72cf40>|0x007|0x007:'75'
	{
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr skeleton_config			= static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource() );
	configs::binary_config_value const& skeleton_value	= skeleton_config->get_root();	// <0x72cfa6>|0x06d|0x042:'82'

	R_ASSERT											( skeleton_value.value_exists( "skeleton" ) );
	pcstr skeleton_path									= skeleton_value["skeleton"];	// <0x72cfb0>|0x077|0x00a:'85'

	resources::request requests[] =
	{
		{ skeleton_path, resources::skeleton_class }
	};																					// <0x72cfba>|0x081|0x00a:'101'

	query_resources										(
		requests,
		array_size( requests ),
		boost::bind( &animated_model_instance_cook::on_subresources_loaded, this, _1 ),
		m_allocator,
		0,
		parent
	);																					// <0x72cfbc>|0x083|0x002:'113'
}

// STATE[55%|DONE]
void animated_model_instance_cook::on_subresources_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent	= data.get_parent_query();
	if ( !data.is_successful() )
	{
		parent->finish_query						( result_error );											// <0x72cdf4>|0x01a|0x017:'121'
		return;
	}

	animated_model_instance* new_model_instance		= VOSTOK_NEW_IMPL( m_allocator, animated_model_instance );	// <0x72ce0a>|0x030|0x016:'125'
	new_model_instance->m_skeleton					= static_cast_resource_ptr< animation::skeleton_ptr >( data[0].get_unmanaged_resource() );

	R_ASSERT( parent );
	resources::user_data_variant* user_data = parent->user_data();
	R_ASSERT( user_data );
	world *physics_world = 0;
	bool res_user_data = user_data->try_get( physics_world );
	R_ASSERT( res_user_data );
	R_ASSERT( physics_world );

	parent->set_unmanaged_resource					(
				new_model_instance,
				resources::memory_usage_type		( resources::nocache_memory, sizeof( animated_model_instance ) )
			);																									// <0x72cee1>|0x107|0x0a9:'148'
	parent->finish_query							( result_success );											// <0x72cdfc>|0x022|-0x0e5:'149'
}

// STATE[100%|DONE]
void animated_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// <1>
	// <2>
	VOSTOK_DELETE_IMPL( m_allocator, resource ); // <0x72cd01>|0x000|0x000:'156'
}

} // namespace physics
} // namespace vostok