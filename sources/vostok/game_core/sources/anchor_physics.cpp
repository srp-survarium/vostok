#include "pch.h"
#include "anchor.h"

#include <vostok/physics/sources/bullet_include.h>

#include <vostok/configs_binary_config_value.h>
#include <vostok/animation/skeleton.h>

#include <vostok/physics/animated_rigid_body.h>
// bone_collision_data.h (full type) is required by use_animated_rigid_body's
// `geometries_type geometries(NULL, 10)` - geometries_type is
// buffer_vector<bone_collision_data>, whose ctor needs sizeof(bone_collision_data).
// animated_rigid_body.h only forward-declares it; the plain data header completes
// it WITHOUT pulling animated_object.h's dllexport explicit instantiation (kept
// single-owned by anchor_collision.cpp).
#include <vostok/collision/bone_collision_data.h>
#include <vostok/physics/api.h>
#include <vostok/physics/character_controller.h>
#include <vostok/physics/collision_shapes.h>
#include <vostok/physics/contact_test_predicate.h>
#include <vostok/physics/engine.h>
#include <vostok/physics/ghost_object.h>
#include <vostok/physics/sources/bullet_character_controller.h>
#include <vostok/physics/static_rigid_body.h>

namespace vostok
{
	void use_physics_api()
	{
		physics::engine engine;
		physics::create_world_bt( NULL, engine );
		physics::destroy_world( NULL, NULL );
		physics::set_memory_allocator( NULL );
	}

	void use_bt_character_controller()
	{
		memory::stack_allocator stack_allocator;
		physics::bt_character_controller* cc2 = physics::create_character_controller( stack_allocator, NULL );
		(void)cc2;
		physics::bt_character_controller cc(NULL);
		cc.allocator();
		cc.initialize();

		float4x4 t;
		cc.activate(t);
		cc.deactivate();
		cc.get_transform();
		cc.set_transform(t);
		cc.set_walk_direction(float3());
		cc.has_updates();
		cc.jump();
		cc.end_jump();
		cc.adjust_foot_transform( float3(), float3(), float3(), 1.0, 1.0, t);
		cc.update_action(10);
		cc.can_jump();
		cc.on_ground();
		cc.set_crouch(true);
		cc.can_crouch();
		cc.can_stand();
	}

	void use_bullet_character_controller()
	{
		physics::bullet_character_controller controller( NULL, float2(), float2(), 10, 20 );
		controller.set_transform( btTransform( ) );
		controller.set_crouch( true );
		controller.can_stand( );
		controller.get_transform( );
		controller.on_ground( );
		controller.can_jump( );
		controller.end_jump( );
		controller.jump( );
		controller.set_desired_walk_vector( btVector3( ) );
		// controller.setup_shape_dim( float2( ) );
		controller.insert( NULL );
		controller.remove( NULL );
		controller.updateAction( NULL, 10.0 );


		physics::computeReflectionDirection( btVector3( ), btVector3( ) );
		physics::perpindicularComponent( btVector3( ), btVector3( ) );
		physics::parallelComponent( btVector3( ), btVector3( ) );
		physics::setup_game_material_groups( NULL, 10 );
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

		physics::create_static_rigid_body( physics::bt_rigid_body_construction_info( ) );
		physics::destroy_static_rigid_body( NULL );
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

			physics::calculate_bt_animated_body_size_from_hit_targets_config( config );
		}

		{
			configs::binary_config_value config = configs::binary_config_value();
			animation::skeleton_ptr skeleton_ptr( NULL );
			// physics::calculate_bt_hit_target_size( config );

			// calculate_bt_animated_body_size_from_hit_targets_config
			physics::calculate_bt_animated_body_size_from_hit_targets_config( config );
			physics::new_animated_bt_hit_model( config, skeleton_ptr, NULL );
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

	void anchor_physics( )
	{
		use_bt_character_controller();
		use_physics_api();
		use_static_rigid_body();
		use_animated_rigid_body();
		use_bullet_character_controller();
	}
}
