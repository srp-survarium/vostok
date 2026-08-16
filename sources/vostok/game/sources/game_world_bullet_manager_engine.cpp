////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

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

// claude@NOTE: faithful sound-facade reconstruction (paired). Two byte residuals are
// cross-unit sound-module work: (1) the target's emit_and_play_once pushes a 6th
// trailing default arg (a `bool`) absent from sound_emitter.h's 5-arg decl - same
// missing 6-arg overload step_manager::on_step flagged; (2) the guard folds the
// sound_emitter c_ptr test into the outer `if ( resource )` (static_cast over the
// resource pointer, no temp construct) so the target is 3 statements where our inner
// `if ( sound.c_ptr( ) )` is a 4th - both recover once the sound-facade
// emit_and_play_once arg convention is settled in the sound module.
void game_world::play_sound( resources::unmanaged_resource_ptr const& resource, float3 const& position )
{
	if ( resource )
	{
		sound::sound_emitter_ptr sound	= static_cast_resource_ptr< sound::sound_emitter_ptr >( resource );
		if ( sound.c_ptr( ) )
			sound->emit_and_play_once(
				get_sound_scene( ),
				get_game( ).get_sound_world( ).get_logic_world_user( ),
				position
			);
	}
}

// claude@NOTE: STRUCTURE MATCH (3 stmts, create_rotation + translation row +
// play). The play_particle_system call boundary carries the render-facade
// signature residual (same as weapon::play_weapon_fire_pfx / object_solid_visual):
// the PDB / render::engine::world_pc take in_instance BY VALUE so the target owns
// the temp particle_system_instance_ptr, but scene_renderer.h's facade spells it
// const& so our base constructs AND destroys the temp in the caller (+0x2c bytes).
// objdiff loses the fuzzy pairing on that residual (shows unpaired) but the rich
// structure-diff pairs it cleanly. Recovers when the facade play_particle_system
// signature is corrected (its own render-facade match phase).
void game_world::play_particle(
	resources::unmanaged_resource_ptr const&	particle,
	float3 const&		position,
	float3 const&		direction,
	float3 const&		normal
)
{
	float4x4 m		= create_rotation( direction, normal );
	m.c.xyz( )		= position;

	scene_renderer( ).play_particle_system( render_scene( ), static_cast_resource_ptr< particle::particle_system_instance_ptr >( particle ), m );
}

// claude@NOTE: STRUCTURE MATCH (7 stmts, all paired - no BASE_ONLY/TRGT_ONLY).
// Residual is inline-vs-call on line 90: the target INLINES create_rotation(
// direction, normal ) (0x124 of cross-product / normalize matrix math) while our
// /Od build OUT-LINES it to a single `call` (0x13) - the od-helper-inline-depth
// wall, not source-steerable. Line 105 (update_decal) carries the decal_properties
// inline field-fill + call-boundary residual (5 projection bools re-set to 1,
// alpha/clip = -1) that the target folds into the call site. Both are byte caps
// over the correct shape, not structure gaps.
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
	render::decal_properties	properties;

	float4x4 transform			= create_rotation( direction, normal );
	transform.c.xyz( )			= position + normal * depth * 0.5f;

	properties.material			= is_front_face ? resources::unmanaged_resource_ptr( ) : decal;

	properties.transform		= transform;
	properties.transform.set_scale( float3( 1.0f, 1.0f, 1.0f ) );

	properties.width_height_far_distance	= float3( size, size, depth * 2.0f ) * 0.5f;

	scene_renderer( ).update_decal( render_scene( ), id, properties );
}

void game_world::remove_decal( u32 id )
{
	scene_renderer( ).remove_decal( render_scene( ), id );
}

void game_world::tick_bullet_manager_engine( bool is_game_paused )
{
	if ( m_bullet_manager == NULL || is_game_paused )
		return;

	m_bullet_manager->tick( get_game( ).game_time_ms( ) );
}


} // namespace survarium
