#include "pch.h"
#include "anchor.h"

#include <vostok/physics/sources/bullet_include.h>

#include <vostok/configs_binary_config_value.h>
#include <vostok/animation/skeleton.h>

#include <vostok/physics/animated_rigid_body.h>
#include <vostok/physics/api.h>
#include <vostok/physics/character_controller.h>
#include <vostok/physics/collision_shapes.h>
#include <vostok/physics/engine.h>

// The target retains both standalone bodies without a direct call edge. Keep
// their symbols without fabricating callers that perturb LTCG across physics.
#pragma comment(linker, "/include:?parallelComponent@physics@vostok@@YA?AVbtVector3@@ABV3@0@Z")
#pragma comment(linker, "/include:?contact_pair_test@bullet_physics_world@physics@vostok@@QAEXAAUcontact_test_predicate@23@PAVbtCollisionObject@@1@Z")

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

	void use_animated_rigid_body()
	{
		configs::binary_config_value config = configs::binary_config_value();
		animation::skeleton_ptr skeleton_ptr( NULL );
		physics::new_animated_bt_hit_model( config, skeleton_ptr, NULL );
	}

	void anchor_physics( )
	{
		use_bt_character_controller();
		use_physics_api();
		use_animated_rigid_body();
	}
}
