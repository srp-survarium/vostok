// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
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

damage_zone::~damage_zone( )
{
}

// claude@NOTE: Source operations and loop shape are complete. The target inlines the
// resource_ptr copy and drops the two unused trailing arguments from its sole caller.
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

// claude@NOTE: Flow and statements match. The sole caller uses a target-only
// this-on-stack convention, which changes this function's frame and register allocation.
void damage_zone::play_particles( vector< resources::unmanaged_resource_ptr > const& particles ) const
{
	vectora< float3 > shapes_centers( g_allocator );
	get_shapes_centers( shapes_centers );
	u32 const count = shapes_centers.size( );

	for ( u32 i = 0; i < count; ++i )
		m_game_world.renderer( ).scene( ).play_particle_system(
			m_game_world.render_scene( ),
			static_cast_resource_ptr< particle::particle_system_instance_ptr >( particles[ i ] ),
			create_translation( shapes_centers[ i ] ) );
}

void damage_zone::stop_particles( vector< resources::unmanaged_resource_ptr > const& particles ) const
{
	for ( u32 n = particles.size( ),
		i = 0; i < n; ++i )
		m_game_world.renderer( ).scene( ).remove_particle_system_instance( m_game_world.render_scene( ), particles[ i ] );
}

// claude@NOTE: Flow and statements match. The target passes play_particles' this pointer
// on the stack; the base emits an ordinary thiscall. Revisit with caller-context control.
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
