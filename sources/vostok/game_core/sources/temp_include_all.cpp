#include "pch.h"
#include "temp_include_all.h"

#include "../../collision/sources/loose_oct_tree.h"

#include <vostok/ai/npc_statistics.h>

#include <vostok/collision/animated_object.h>
#include <vostok/collision/bone_collision_data.h>
#include <vostok/collision/api.h>

#include <vostok/configs_binary_config_value.h>
#include <vostok/managed_allocator.h>

#include <vostok/render/facade/one_way_render_channel.h>
#include <vostok/render/world.h>
#include <vostok/render/engine/world.h>
#include <vostok/render/facade/debug_renderer.h>

#include <vostok/physics/animated_rigid_body.h>
#include <vostok/physics/ghost_object.h>
#include <vostok/physics/collision_shapes.h>
#include <vostok/physics/static_rigid_body.h>

// #include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <vostok/network_core/http_client.h>
#include <vostok/network_core/tcp_packet.h>

namespace vostok
{
	void use_log()
	{
		LOG_WARNING("ERROR %d", 10);
		logging::append(
			logging::log_callback_boost( core::g_log_callback ),
			(void*)0,
			logging::format_specifier( logging::format_specifier_time_brief ),
			"file",
			41,
			"use_log",
			"game_core:",
			logging::warning,
			"%s",
			"Hello!"
		);

		logging::append(
			logging::log_callback_boost( core::g_log_callback ),
			(void*)0,
			&logging::log_format( ),
			"file",
			41,
			"use_log",
			"game_core:",
			logging::warning,
			"%s",
			"Hello!"
		);

	}

	void example_callback(const char *name)
	{
		printf("%s\n", name);
	}

	void use_network_core_tcp_packet()
	{
		memory::stack_allocator stack_allocator;
		network_core::tcp_packet packet( stack_allocator );

		network_core::buffer_to_send( packet );
		network_core::buffer_to_receive_into( packet );
	}


	void use_network_core_http_client()
	{
		boost::asio::io_service io_service( 10 );
		network_core::http_client http_client( io_service );
		http_client.set_on_error( boost::bind( use_network_core_http_client ) );
		http_client.get( "server", "path", boost::bind(&example_callback, "hello" ) );

		boost::asio::streambuf buff;
		network_core::read_lines_from_stream( "prefix", buff );
	}

	void use_static_rigid_body()
	{
		physics::bt_collision_shape_ptr shape(NULL);

		physics::bt_static_rigid_body body( shape, NULL );
		body.get_rigid_body( );
		body.get_triangle_material( 10, true );
		body.apply_impulse( float3(), float3() );
		body.set_transform( float4x4() );
		body.get_transform( );
		body.get_collision_group( );
		body.get_bt_collision_obect( );

	}

	void use_collision_shape()
	{
		physics::bt_collision_shape shape(NULL);
		shape.get_triangle_material( 10, true );

		configs::binary_config_value bcv = configs::binary_config_value();

		physics::destroy_bt_shape			( NULL );
		physics::destroy_shape			( NULL );
		physics::create_bt_primitive		( collision::primitive_box, float3(), float3() );
		physics::create_primitive_shape	( collision::primitive_box, float3(), float3() );
		physics::create_compound_shape	( bcv, float3(), "model_path" );

		physics::geometry_resource_ptr resource_ptr	( NULL );
		physics::create_btBvhTriangleMeshShape		( NULL, NULL, 10, 10, NULL, float3(), resource_ptr, resource_ptr );
		physics::create_static_triangle_mesh_shape	( NULL, NULL, 10, 10, NULL, float3(), resource_ptr, resource_ptr );
	}


	void use_ghost_object()
	{
		physics::bt_collision_shape_ptr shape(NULL);
		physics::bt_ghost_object ghost = physics::bt_ghost_object( shape, NULL );
		physics::base_physics_objects_type result( NULL, 10 );

		ghost.get_overlapping_objects_count( );
		ghost.set_transform( math::float4x4() );
		ghost.get_transform( );
		ghost.get_collision_group( );
		ghost.get_overlapping_objects_count( );
		ghost.get_bt_collision_obect( );
		ghost.get_overlapping_objects( result );
		ghost.insert( NULL, 10, 20 );
		physics::destroy_ghost_object( physics::create_ghost_object( shape, math::float4x4() ) );
	}

	void use_loose_oct_tree()
	{
		collision::loose_oct_tree tree(NULL, 100., 10);
		tree.unmove_all();
		tree.get_aabb();
	}

	void use_animated_object()
	{

		configs::binary_config_value bcv = configs::binary_config_value();
		animation::skeleton_ptr ms = animation::skeleton_ptr();
		memory::stack_allocator stack_allocator;

		collision::animated_object ao = collision::animated_object(bcv, ms, 10, stack_allocator);
		ao.update(NULL, NULL);
		ao.destroy(NULL);
		ao.get_bones_count();
		ao.get_aabb();
		ao.get_geometry();

		render::scene_ptr scene = vostok::render::scene_ptr();
		memory::managed_allocator alloc = memory::managed_allocator(100, 100);
		render::one_way_render_channel channel = render::one_way_render_channel(alloc);
		render::engine::world world = render::engine::world();
		render::debug::renderer renderer = render::debug::renderer(channel, alloc, world);


		float4x4 transform;
		ao.draw_collision(scene, renderer, transform);

		ao.get_random_surface_point(10);
		ao.get_head_bone_center();
		ao.get_eyes_direction();
		ao.body_part_name(10);
	}

	void use_aabb_object()
	{
		collision::new_aabb_object( NULL, 20, float3( 1.f, 1.f, 1.f ) );
	}

	void use_animated_rigid_body()
	{
		{
			physics::bt_animated_rigid_body arb = physics::bt_animated_rigid_body( NULL, NULL, 10 );
			float4x4 matrix;

			arb.center_of_mass_offset();
			arb.update_bone_matrix(10, matrix, true);
			arb.get_aabb();
			arb.get_bone_transform(10);
		}

		{
			// btCompoundShape* new_compound_shape_from_hit_targets_config( configs::binary_config_value const& config, geometries_type& geometries_data, memory::base_allocator* allocator )
			configs::binary_config_value config = configs::binary_config_value();
			physics::geometries_type geometries( NULL, 10 );
			physics::new_compound_shape_from_hit_targets_config( config, geometries, NULL );
		}

		{
			configs::binary_config_value config = configs::binary_config_value();
			// physics::calculate_bt_hit_target_size( config );

			// calculate_bt_animated_body_size_from_hit_targets_config
			physics::calculate_bt_animated_body_size_from_hit_targets_config( config );
		}

		{
			// sushi@NOTE: Called from animated_object::animated_object there game_material_id is set to 10 and linker hardcoded it here
			// bt_animated_rigid_body*	new_animated_rigid_body		( btCompoundShape* shape, u16 game_material_id, memory::base_allocator* allocator );
			physics::new_animated_rigid_body ( NULL, 10, NULL );
		}
		{
			// void						destroy_animated_rigid_body	( bt_animated_rigid_body* body, memory::base_allocator* allocator );
			physics::destroy_animated_rigid_body( NULL, NULL );
		}
	}

}


namespace survarium
{

void bpp_dump_stats(u32, float, float, pcstr) { }


IncludeAll::IncludeAll()
{
	//
	//
	//
	vostok::use_log();
	vostok::use_network_core_http_client();
	vostok::use_static_rigid_body();
	vostok::use_animated_object();
	vostok::use_animated_rigid_body();
	vostok::use_aabb_object();
	vostok::use_ghost_object();
	vostok::use_collision_shape();

	//
	// YEEET
	//
	hit_type_parameters* htp = new hit_type_parameters("hand", 1., 1., 1., 0);
	htp->apply_damage(10., 100);
	htp->set_parameters(10., 20., 30.);

	booster_damage_protector* bdp = new booster_damage_protector("hand", 0.5, 1.);
	bdp->reduce_damage("__whatever", "hand", 100);

	vostok::ai::npc_statistics stats = vostok::ai::npc_statistics();

	//
	// DAMAGE MODEL
	//
	damage_model dm = damage_model(type_apply_directly);

	dm.add_body_part(NULL);
	dm.hit_body_part(1, "part_name", "damage_type", 10., 20., 30, NULL);
	dm.apply_med_kit("head", 1000);
	dm.tick(10, 20);
	dm.fill_stats(stats, 20);
	dm.dump_stats(&bpp_dump_stats);
	dm.reset();
	dm.apply_affect("part_name", affects_type_bleeding, affect_canceling);
	dm.cancel_affect("part_name", affects_type_bleeding);
	dm.subscribe_on_affect(affects_type_bleeding, NULL);
	dm.unsubscribe_from_affect(affects_type_bleeding, NULL);
	dm.notify_on_affect_event("body_part_name", affects_type_bleeding, affect_canceling);
	dm.add_damage_protector("damage_type", 1000., 10000.);
	dm.register_body_part_damage_protector("part_name", NULL);
	dm.unregister_body_part_damage_protector("part_name", NULL);
	dm.get_affects_applying_type();
	dm.get_body_part("part_name");
	dm.pop_body_part();
	dm.get_total_health();
	// dm.on_broken_limb_affect("bodypart", affects_type_bleeding, affect_canceling);

	//
	// BODY PART PARAMETERS
	//
	body_part_parameters* bpp = new body_part_parameters(
		"name",
		10.f,
		10.f,
		10.f,
		true,
		dm,
		1);



	bpp->add_hit_type(NULL);
	bpp->add_threshold(NULL);
	bpp->hit_by_type("hit_type", 10, 10., 10., false, NULL);
	bpp->increase_health(10);
	bpp->decrease_health(20);
	bpp->regenerate(10, 20);
	bpp->dump_state(&bpp_dump_stats, 0);
	bpp->dump_state(stats, 10);
	bpp->reset();

	bpp->apply_affect_by_force(affects_type_blindness, affect_canceling, 20);
	bpp->can_affect_death();
	bpp->has_affect_protector(affects_type_blindness);
	bpp->get_health_in_percentage();
	bpp->cancel_affect_by_force(affects_type_blindness);
	bpp->add_damage_protector(NULL);
	bpp->remove_damage_protector(NULL);
	bpp->pop_hit_type();
	bpp->pop_threshold();

	bpp->is_affect_applied(affects_type_blindness);
	bpp->get_hit_parameters("hit_params");
	bpp->set_parameters(10.f, 20.f);

	bpp->check_affects(10);
	bpp->update_affects(20);
	bpp->apply_affects(NULL, 30);

	Callback1 cb1;
	Callback2 cb2;
}


Callback1::Callback1()
{
	m_complete = boost::bind(&Callback1::complete, this, _1);
	m_partial  = boost::bind(&Callback1::partial , this, _1, 10);
}



void Callback1::complete(const char *name)
{
	m_complete = boost::bind(&Callback1::complete, this, _1);
}

void Callback1::partial(const char *name, int value)
{
	m_partial  = boost::bind(&Callback1::partial , this, _1, 10);
}



Callback2::Callback2()
{
	m_complete = boost::bind(&Callback2::complete, this, _1, _2);
	m_partial  = boost::bind(&Callback2::partial , this, _1, _2, 10.f);
}

void Callback2::complete(const char *name, int value)
{
	m_complete = boost::bind(&Callback2::complete, this, _1, _2);
}

void Callback2::partial(const char *name, int value, float precision)
{
	m_partial  = boost::bind(&Callback2::partial , this, _1, _2, 10.f);
}


}