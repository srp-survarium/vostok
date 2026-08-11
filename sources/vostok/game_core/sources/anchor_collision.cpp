#include "pch.h"
#include "anchor.h"

#include "../../collision/sources/loose_oct_tree.h"

#include <vostok/collision/animated_object.h>
#include <vostok/collision/bone_collision_data.h>
#include <vostok/collision/api.h>

#include <vostok/configs_binary_config_value.h>
#include <vostok/animation/skeleton.h>

// use_collision_shape exercises the physics shape factory (physics:: free funcs).
#include <vostok/physics/collision_shapes.h>
#include <vostok/physics/static_rigid_body.h>

namespace vostok
{
	void use_collision_shape()
	{
		physics::bt_collision_shape shape(NULL);
		shape.get_triangle_material( 10, true );

		configs::binary_config_value bcv = configs::binary_config_value();

		physics::destroy_bt_shape		( NULL );
		physics::destroy_shape			( NULL );
		physics::create_bt_primitive	( collision::primitive_box, float3(), float3() );
		physics::create_primitive_shape	( collision::primitive_box, float3(), float3() );
		physics::create_compound_shape	( bcv, float3(), "model_path" );

		physics::geometry_resource_ptr resource_ptr	( NULL );
		physics::create_btBvhTriangleMeshShape		( NULL, NULL, 10, 10, NULL, float3(), resource_ptr, resource_ptr );
		physics::create_static_triangle_mesh_shape	( NULL, NULL, 10, 10, NULL, float3(), resource_ptr, resource_ptr );
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

		ao.get_random_surface_point(10);
		ao.get_head_bone_center();
		ao.get_eyes_direction();
		ao.body_part_name(10);
	}

	void use_aabb_object()
	{
		collision::new_aabb_object( NULL, 20, float3( 1.f, 1.f, 1.f ) );
	}

	void anchor_collision( )
	{
		// NB: use_loose_oct_tree() is intentionally NOT dispatched - the old
		// monolithic IncludeAll never called it either (the body is kept defined,
		// but uncalled, to preserve identical /OPT:REF reachability).
		use_animated_object();
		use_aabb_object();
		use_collision_shape();
	}
}
