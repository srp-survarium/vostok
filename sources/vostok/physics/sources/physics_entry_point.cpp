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

// claude@NOTE: the static_cast<base_allocator&> keeps malloc_impl virtual (matches the
// target's [g_mt_allocator+10h] vtable call); a concrete pthreads3_allocator devirtualises
// to pt3malloc in /Od. Remaining residual is the bullet_physics_world ctor's optimized
// COMDAT convention (this-in-eax + returns this), which spills through esi in our base.
vostok::physics::world* create_world_bt( vostok::memory::base_allocator* allocator, physics::engine& engine )
{
	return VOSTOK_NEW_IMPL( static_cast<memory::base_allocator&>( memory::g_mt_allocator ), bullet_physics_world )( memory::g_mt_allocator, engine );
}

// claude@NOTE: STRUCTURE MATCH (3/3 stmts). Residual is non-steerable: (1) LTCG
// const-folds the sole caller's g_mt_allocator arg into g_ph_allocator (base reads
// the parameter); (2) the inlined collision_shape_cook base-ctor reads thread id via
// GetCurrentThreadId() in the target vs a cached global in our /Od base (cross-module).
void set_memory_allocator( memory::base_allocator* allocator )
{
	ASSERT				( !g_ph_allocator || g_ph_allocator==allocator );
	g_ph_allocator		= allocator;

	static collision_shape_cook collision_shape_cooker_static( true );
	static collision_shape_cook collision_shape_cooker_dynamic( false );
	static animated_model_instance_cook animated_model_cook;
}

void destroy_world( vostok::memory::base_allocator* allocator, world* w )
{
	w->destroy( );
	VOSTOK_DELETE_IMPL( static_cast<memory::base_allocator&>( memory::g_mt_allocator ), w );
}

} // namespace physics
} // namespace vostok
