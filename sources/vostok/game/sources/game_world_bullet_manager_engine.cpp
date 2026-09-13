// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "game_world.h"
#include "game.h"

#include <vostok/game_core/bullet_manager.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/facade/decal_properties.h>
#include <vostok/sound/world.h>
#include <vostok/sound/sound_emitter.h>

namespace survarium {

static u16 s_tracer_idx;

bool game_world::attach_tracer( ::survarium::bullet* bullet )
{
	static float4x4 initial_tracer_matrix = float4x4( ).identity( );

	const u16 index = u16( s_tracer_idx % m_bullet_tracers.size( ) );

	bullet_tracer& tracer = m_bullet_tracers[ index ];

	bullet->m_tracer_idx = index;
	tracer.bullet = bullet;
	scene_renderer( ).add_tracer( render_scene( ), tracer.tracer, initial_tracer_matrix );
	++s_tracer_idx;

	return true;
}

bool game_world::detach_tracer( ::survarium::bullet* bullet )
{
	bullet_tracer& tracer = m_bullet_tracers[ bullet->m_tracer_idx ];
	if ( tracer.bullet == bullet )
	{
		scene_renderer( ).remove_tracer( render_scene( ), tracer.tracer );
		tracer.bullet = NULL;
	}
	else
		LOG_ERROR( "case when bullet_tracer.bullet != bullet not implemented" );

	bullet->m_tracer_idx = 0xFFFF;

	return true;
}

void game_world::update_tracer(
	const u16			tracer_idx,
	float3 const&		position,
	float3 const&		direction,
	const float			length
)
{
	float4x4 m		= create_translation( position );
	m.k.xyz( )		= direction;

	m.i.xyz( )		= cross_product( float3( 0.f, 1.f, 0.f ), direction );
	m.j.xyz( )		= cross_product( direction, m.i.xyz( ) );
	m.set_scale		( float3( 1.f, 1.f, length ) );
	scene_renderer( ).update_tracer( render_scene( ), m_bullet_tracers[ tracer_idx ].tracer, m );
}

#line 63
void game_world::play_sound( resources::unmanaged_resource_ptr const& resource, float3 const& position )
{
	if ( resource )
	{

		sound::sound_emitter_ptr sound	= static_cast_resource_ptr< sound::sound_emitter_ptr >( resource );
		sound->emit_and_play_once( get_sound_scene( ), get_game( ).get_sound_world( ).get_logic_world_user( ), position );
	}
}

void game_world::play_particle(
	resources::unmanaged_resource_ptr const&	particle,
	float3 const&		position,
	float3 const&		direction,
	float3 const&		normal
)
{
#line 75
	float4x4 m		= create_rotation( direction, normal );
	m.c.xyz( )		= position;
	scene_renderer( ).play_particle_system( render_scene( ), static_cast_resource_ptr< particle::particle_system_instance_ptr >( particle ), m );
}

void game_world::add_decal(
	resources::unmanaged_resource_ptr const&	decal,
	const u32			id,
	float				size,
	const float			depth,
	float3 const&		position,
	float3 const&		direction,
	float3 const&		normal,
	const bool			is_front_face
)
{
#line 89
	render::decal_properties	properties;
	float4x4 transform			= create_rotation( direction, normal );
	transform.c.xyz( )			= position + normal * depth * 0.5f;

	properties.material			= is_front_face ? resources::unmanaged_resource_ptr( ) : decal;

	properties.transform		= transform;
	properties.transform.set_scale( float3( 1.0f, 1.0f, 1.0f ) );

#line 101
	properties.width_height_far_distance	= float3( size, size, depth * 2.0f ) * 0.5f;

#line 105
	scene_renderer( ).update_decal( render_scene( ), id, properties );
}

void game_world::remove_decal( u32 id )
{
#line 110
	scene_renderer( ).remove_decal( render_scene( ), id );
}

void game_world::tick_bullet_manager_engine( bool is_game_paused )
{
#line 115
	if ( m_bullet_manager == NULL )
		return;
	if ( !is_game_paused )
		m_bullet_manager->tick( get_game( ).game_time_ms( ) );
}


} // namespace survarium
