////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animated_model_instance_cook.h"
#include "animated_model_instance.h"
#include <vostok/resources.h>
#include <vostok/resources_query_result.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/collision/api.h>
#include <vostok/animation/animation_player.h>
#include <vostok/physics/animated_rigid_body.h>	// new_animated_bt_hit_model
#include <vostok/game_core/damage_model.h>
#include <vostok/game_core/affects_applying_type_enum.h>

namespace vostok {
namespace physics {
	struct world;
} // namespace physics
} // namespace vostok

namespace survarium {

 animated_model_instance_cook::animated_model_instance_cook( ) :
	resources::translate_query_cook( resources::game_animated_model_instance_class, reuse_false, use_resource_manager_thread_id )
{
}

void animated_model_instance_cook::translate_request_path( pcstr request, fs_new::virtual_path_string& new_request ) const
{
	new_request.assignf( "resources/animated_model_instances/%s.animated_model", request );
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
		R_ASSERT										( data.is_successful(), "couldn't retrieve config for game animated model" );
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr config					= static_cast_resource_ptr<configs::binary_config_ptr>( data[0].get_unmanaged_resource() );
	configs::binary_config_value const& config_value	= config->get_root();
	configs::binary_config_value const& models			= config_value["models"];

	R_ASSERT											( models.value_exists( "render_animated_model" ) );
	pcstr render_model_path								= models["render_animated_model"];

	R_ASSERT											( models.value_exists( "physics_animated_model" ) );
	pcstr physics_model_path							= models["physics_animated_model"];

	R_ASSERT											( models.value_exists( "damage_collision_object" ) );
	pcstr damage_collision_path							= models["damage_collision_object"];
	fs_new::virtual_path_string							damage_config_path;
	damage_config_path.assignf							( "resources/animated_model_instances/collision_objects/%s.physics", damage_collision_path );

	R_ASSERT											( parent );
	resources::user_data_variant* user_data				= parent->user_data();
	R_ASSERT											( user_data );
	physics::world* physics								= 0;
	bool const res_user_data							= user_data->try_get( physics );
	R_ASSERT											( res_user_data );
	R_ASSERT											( physics );

	resources::user_data_variant						physics_world;
	physics_world.set									( physics );

	resources::user_data_variant const* params[] =
	{
		0,
		&physics_world,
		0
	};

	resources::request requests[] =
	{
		{ render_model_path, resources::render_animated_model_instance_class },
		{ physics_model_path, resources::physics_animated_model_instance_class },
		{ damage_config_path.c_str(), resources::binary_config_class }
	};

	resources::query_resources							(
		requests,
		array_size( requests ),
		boost::bind( &animated_model_instance_cook::on_subresources_loaded, this, _1 ),
		resources::unmanaged_allocator(),
		params,
		parent
	);
}

void animated_model_instance_cook::on_subresources_loaded( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent	= data.get_parent_query();
	if ( !data.is_successful() )
	{
		R_ASSERT									( data.is_successful(), "couldn't cook some of game animated model subresources" );
		parent->finish_query						( result_error );
		return;
	}

	animated_model_instance* new_model_instance		= VOSTOK_NEW_IMPL( resources::unmanaged_allocator(), animated_model_instance );
	new_model_instance->m_render_model				= static_cast_resource_ptr< render::animated_model_instance_ptr >( data[0].get_unmanaged_resource() );
	new_model_instance->m_physics_model				= static_cast_resource_ptr< physics::animated_model_instance_ptr >( data[1].get_unmanaged_resource() );

	configs::binary_config_ptr collision_config		= static_cast_resource_ptr< configs::binary_config_ptr >( data[2].get_unmanaged_resource() );
	new_model_instance->m_damage_collision			= physics::new_animated_bt_hit_model(
														collision_config->get_root(),
														new_model_instance->m_physics_model->m_skeleton,
														resources::unmanaged_allocator()
													);
	vostok::animation::animation_player* player		= VOSTOK_NEW_IMPL( resources::unmanaged_allocator(), vostok::animation::animation_player );
	new_model_instance->m_animation_player			= player;

	fs_new::virtual_path_string	hit_params_path;
	hit_params_path.assignf							( "resources/gameplay/hit_params/%s", new_model_instance->m_render_model->m_hit_params.c_str( ) );

	resources::user_data_variant	user_data;
	user_data.set									( type_apply_directly );
	resources::user_data_variant const* user_data_ptr[]	= { &user_data };

	resources::request	request						= { resources::damage_model_class, hit_params_path.c_str( ) };
	resources::query_resources						(
		&request,
		1,
		boost::bind( &animated_model_instance_cook::on_hit_params_loaded, this, _1, new_model_instance ),
		resources::unmanaged_allocator(),
		user_data_ptr,
		parent
	);
}

// claude@NOTE: DECODED (target 0x7695c0 caller / 0x769893 tail + this fn 292B).
// The target on_subresources_loaded differs from ours:
//  1. m_damage_collision = physics::new_animated_bt_hit_model( collision_config->get_root(),
//     m_physics_model->m_skeleton(+0x108), unmanaged_allocator() )  [NOT collision::new_animated_object]
//  2. a data[3] fetch-and-release temp exists (queries stride 0x1E0, header 0x12C ->
//     on_config_loaded must add a FOURTH sub-request; identity TBD - likely the skin)
//  3. then: virtual_path_string p; p.assignf( "resources/gameplay/hit_params/%s",
//     new_model->m_render_model->m_hit_params.c_str() );   [m_hit_params @render facade +0x110]
//     user_data_variant v; v.set( type_apply_directly );   [affects_applying_type_enum 0]
//     request r = { p.c_str(), damage_model_class /*0x5F*/ };
//     query_resources( &r, 1, bind(&on_hit_params_loaded, this, _1, new_model),
//                      unmanaged_allocator(), &v-array, parent?, 1 );
//  4. set_unmanaged_resource + finish_query move INTO on_hit_params_loaded (deferred).
// This fn (target): parent=get_parent_query; if !is_successful -> finish_query(result_error);
// else new_model->m_damage_model = static_cast_resource_ptr<damage_model_ptr>(
//   data[0].get_unmanaged_resource() ) [swap-destroy of the old at +0x110];
// parent->set_unmanaged_resource( new_model, memory_usage_type( nocache_memory,
//   sizeof(animated_model_instance) /*0x120*/ ) ); parent->finish_query( result_success );
void animated_model_instance_cook::on_hit_params_loaded( resources::queries_result& data, animated_model_instance* new_model )
{
	resources::query_result_for_cook* const	parent	= data.get_parent_query();
	if ( !data.is_successful() )
	{
		parent->finish_query						( result_error );
		return;
	}

	new_model->m_damage_model						= static_cast_resource_ptr< damage_model_ptr >( data[0].get_unmanaged_resource() );

	parent->set_unmanaged_resource					(
		new_model,
		resources::memory_usage_type(
			resources::nocache_memory,
			sizeof( animated_model_instance )
		)
	);
	parent->finish_query							( result_success );
}

void animated_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	animated_model_instance* model_instance			= static_cast_checked< animated_model_instance* >( resource );
	collision::delete_animated_object				( model_instance->m_damage_collision, resources::unmanaged_allocator() );
	vostok::animation::animation_player* player		= model_instance->m_animation_player;
	VOSTOK_DELETE_IMPL								( resources::unmanaged_allocator(), player );
	VOSTOK_DELETE_IMPL								( resources::unmanaged_allocator(), model_instance );
}

} // namespace survarium
