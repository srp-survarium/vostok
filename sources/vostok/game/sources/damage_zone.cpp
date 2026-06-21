////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "damage_zone.h"
#include "game_world.h"
#include "game_memory.h"

#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/math_float4x4_inline.h>

namespace survarium {

// STATE[STUB]
damage_zone::damage_zone( game_world& game_world ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game_world( game_world )
{
	// FUNCTION BODY[0x5bcf30]
	// <0x5bcf30>|0x000|      :'18'	{
	// ******
}

// STATE[STUB]
 damage_zone::~damage_zone( )
{
	// FUNCTION BODY[0x5bcea0]: 0
	// <0x5bcea0>|0x000|+0x033:'21'	{
	// <0x5bced3>|0x033|      :'22'	}
	// ******
}

// STATE[STUB]
void damage_zone::load(
	configs::binary_config_value const&		cfg_val,
	vector< resources::unmanaged_resource_ptr >&	resources,
	vector< render::light_props >&		,	// PDB: __formal, genuinely unused
	u32										// PDB: __formal, genuinely unused (two collide - left unnamed)
)
{
	// FUNCTION BODY[0x5bce20]: 9
	// <0x5bce2a>|0x00a|+0x00c:'29'
	// <0>
	// <0x5bce36>|0x016|+0x006:'31'
	// <0x5bce3c>|0x01c|+0x014:'32'
	// <0x5bce50>|0x030|+0x045:'33'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// claude@NOTE: PARKED on a loop-structure residual, NOT the facade (the facade
// play_particle_system by-value fix has landed). The target computes
// shapes_centers.size() in its OWN statement (line 64) BEFORE the for (line 66) -
// a /Od size-hoist with NO named local (target records only particles + shapes_centers,
// no i, no count) - so the bound caching cannot be reproduced by a named-count loop
// (that would add a local the target lacks) nor by our folded `for ( i < .size(); )`
// (base attributes size to the for header). Tried: nothing reproduces the unnamed
// hoisted-size 7-statement split. Next: needs the original loop spelling that CSEs
// .size() out of the condition under /Od (likely an accessor-inlining order quirk).
void damage_zone::play_particles( vector< resources::unmanaged_resource_ptr > const& particles ) const
{
	vectora< float3 > shapes_centers( g_allocator );
	get_shapes_centers( shapes_centers );

	for ( u32 i = 0; i < shapes_centers.size( ); ++i )
		m_game_world.renderer( ).scene( ).play_particle_system(
			m_game_world.render_scene( ),
			static_cast_resource_ptr< particle::particle_system_instance_ptr >( particles[ i ] ),
			create_translation( shapes_centers[ i ] ) );
}

// claude@NOTE: PARKED on the same loop-structure residual as play_particles, NOT the
// facade (remove_particle_system_instance doesn't take in_instance by value anyway).
// The target splits the for into 3 statements - particles.size() hoisted into a
// separate init statement (line 76) then a tiny cond+incr (line 77) then the body
// (line 78) - with NO named loop counter recorded. Our folded `for ( i < .size(); )`
// keeps the size in the for header (2 statements). Same unnamed-size-hoist /Od quirk
// as play_particles; not reproducible from the loop header alone.
void damage_zone::stop_particles( vector< resources::unmanaged_resource_ptr > const& particles ) const
{
	for ( u32 i = 0; i < particles.size( ); ++i )
		m_game_world.renderer( ).scene( ).remove_particle_system_instance( m_game_world.render_scene( ), particles[ i ] );
}

// STATE[STUB]
void damage_zone::activate( zone_group* owner, physics::world* p_world, scheduler& scheduler )
{
	// FUNCTION BODY[0x5bd110]: 5
	// <0x5bd110>|0x000|+0x019:'106'
	// <0x5bd129>|0x019|+0x014:'107'
	// <0x5bd13d>|0x02d|+0x00e:'108'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void damage_zone::deactivate( )
{
	// FUNCTION BODY[0x5bd000]: 5
	// <0x5bd003>|0x003|+0x005:'115'
	// <0x5bd008>|0x008|+0x014:'116'
	// <0x5bd01c>|0x01c|+0x00e:'117'
	// <0>
	// <1>
	// ******
}


} // namespace survarium
