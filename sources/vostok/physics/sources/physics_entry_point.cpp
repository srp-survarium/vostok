////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Description : default entry point functions
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/api.h>

#include <vostok/linkage_helper.h>
#include <vostok/physics/world.h>

#include "./animated_model_instance_cook.h"
#include "./collision_shape_cook.h"
#include "./bullet_physics_world.h"

memory::base_allocator*	vostok::physics::g_ph_allocator = NULL;

VOSTOK_DECLARE_LINKAGE_ID(physics_entry_point)

namespace vostok {
namespace physics {

vostok::physics::world* create_world_bt( vostok::memory::base_allocator* allocator, physics::engine& engine )
{
	return VOSTOK_NEW_IMPL( allocator, bullet_physics_world )( *allocator, engine ); // <0x72e5f0>|0x000|0x000:'24'
}

// STATE[STUB]:
void set_memory_allocator( memory::base_allocator* allocator )
{
	ASSERT				( !g_ph_allocator || g_ph_allocator==allocator );
	g_ph_allocator		= allocator;

	static collision_shape_cook collision_shape_cooker_static( true ); // <0x72e494>|0x000|0x000:'32'
	static collision_shape_cook collision_shape_cooker_dynamic( false );
	static animated_model_instance_cook animated_model_cook;
}

void destroy_world( vostok::memory::base_allocator* allocator, world* w )
{
	w->destroy( ); // <0x72e450>|0x000|0x000:'39'
	VOSTOK_DELETE_IMPL( allocator, w );
}

} // namespace physics
} // namespace vostok
