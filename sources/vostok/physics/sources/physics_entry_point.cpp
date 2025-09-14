////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Description : default entry point functions
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "physics_world.h"
#include "bullet_physics_world.h"
#include "collision_shape_cook.h"
#include <vostok/physics/api.h>
#include <vostok/linkage_helper.h>

memory::base_allocator*	vostok::physics::g_ph_allocator = NULL;

VOSTOK_DECLARE_LINKAGE_ID(physics_entry_point)

namespace vostok {
namespace physics{

vostok::physics::world* create_world( vostok::memory::base_allocator* allocator, vostok::physics::engine& engine )
{
	return VOSTOK_NEW_IMPL( allocator, physics_world )	( *allocator, engine );
}

vostok::physics::world* create_world_bt( vostok::memory::base_allocator* allocator, physics::engine& engine )
{
	return VOSTOK_NEW_IMPL( allocator, bullet_physics_world )	( *allocator, engine );
}

void set_memory_allocator	( memory::base_allocator* allocator )
{
	ASSERT				( !g_ph_allocator || g_ph_allocator==allocator );
	g_ph_allocator		= allocator;

	static collision_shape_cook collision_shape_cooker_static( true );
	static collision_shape_cook collision_shape_cooker_dynamic( false );
}

void destroy_world( vostok::memory::base_allocator* allocator, world* w )
{
	VOSTOK_DELETE_IMPL( allocator, w);
}

} // namespace physics
} // namespace vostok
