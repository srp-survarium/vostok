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

damage_zone::damage_zone( game_world& game_world ) :
	m_game_world( game_world )
{
}

// claude@NOTE: empty body, STRUCTURE matches (0/0); compiler-generated member-dtor walk.
// Capped on the m_particles vector dtor inlining (__destroy_range_aux + inline
// vostok_mspace_free vs base routing) - not source-steerable.
damage_zone::~damage_zone( )
{
}

// claude@NOTE: STRUCTURE matches (4/4): the size hoist is reproduced by the multi-line
// comma for-init `for ( u32 n = .size(), i = 0; ...)` (n enregisters, no PDB local).
// Remaining diff is vector<resource_ptr>::push_back inlining its element-copy (target
// inlines the intrusive resource_ptr copy + lock xadd refcount; base routes through
// _Param_Construct) plus the LTCG drop of the two unused trailing formals (ret 4 vs 10h).
void damage_zone::load(
	configs::binary_config_value const&		cfg_val,
	vector< resources::unmanaged_resource_ptr >&	resources,
	vector< render::light_props >&		,	// PDB: __formal, genuinely unused
	u32										// PDB: __formal, genuinely unused (two collide - left unnamed)
)
{
	damage_zone_core::load( cfg_val );

	for ( u32 n = resources.size( ),
		i = 0; i < n; ++i )
		m_particles.push_back( resources[ i ] );
}

// claude@NOTE: capped on inline-vs-call + reg-alloc residual + a /Od local-spill the
// target enregisters. The target hoists shapes_centers.size() once (off the condition) with
// NO named loop local; the comma for-init reproduces the SHAPE, but unlike the sibling
// stop_particles/load (size off a by-ref PARAM), here the size comes off the body-scope
// vectora local `shapes_centers` whose lifetime spans the loop, so under /Od `n` spills to a
// NAMED slot (base records 3 locals incl. n; target 2). Both for-init orders were tried
// (i=0,n=size vs n=size,i=0): neither drops `n` under /Od (it is a frame-allocation effect of
// the live body vectora, not a source-shape error) and `n=size,i=0` also costs ~2% (62->60),
// so the i=0-first form is kept. Dominant residual is the renderer facade
// scene()->play_particle_system inline-vs-call depth + a size/index register swap.
void damage_zone::play_particles( vector< resources::unmanaged_resource_ptr > const& particles ) const
{
	vectora< float3 > shapes_centers( g_allocator );
	get_shapes_centers( shapes_centers );

	for ( u32 i = 0,
		n = shapes_centers.size( );
		i < n; ++i )
		m_game_world.renderer( ).scene( ).play_particle_system(
			m_game_world.render_scene( ),
			static_cast_resource_ptr< particle::particle_system_instance_ptr >( particles[ i ] ),
			create_translation( shapes_centers[ i ] ) );
}

// claude@NOTE: capped on inline-vs-call + reg-alloc + LTCG-convention residual. STRUCTURE
// matches (3/3): the size hoist is reproduced by the multi-line comma for-init (n
// enregisters with no PDB local). Remaining diff is operator[]/get_pointer inline-vs-call,
// a size/index register swap, and the LTCG this-on-stack convention (sole caller deactivate).
void damage_zone::stop_particles( vector< resources::unmanaged_resource_ptr > const& particles ) const
{
	for ( u32 n = particles.size( ),
		i = 0; i < n; ++i )
		m_game_world.renderer( ).scene( ).remove_particle_system_instance( m_game_world.render_scene( ), particles[ i ] );
}

// claude@NOTE: STRUCTURE matches (2/2); residual is 1 byte - target adjusts `this` for the
// play_particles call by reusing the dead damage_zone_core* (add esi,-0x108), base emits a
// fresh lea ecx,[esi-0x108]; reg-alloc, not source-steerable.
void damage_zone::activate( zone_group* owner, physics::world* p_world, scheduler& scheduler )
{
	damage_zone_core::activate( owner, p_world, scheduler );

	if ( !m_particles.empty( ) )
		play_particles( m_particles );
}

void damage_zone::deactivate( )
{
	damage_zone_core::deactivate( );

	if ( !m_particles.empty( ) )
		stop_particles( m_particles );
}


} // namespace survarium
