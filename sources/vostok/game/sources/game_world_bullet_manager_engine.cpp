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

namespace survarium {

// STATE[STUB]
// claude@NOTE: parked - render-facade tracer wall. Recovered body picks a tracer
// slot (s_tracer_idx % m_bullet_tracers.size()), stores the bullet there, stamps
// bullet->m_tracer_idx, and registers a tracer with the renderer via
// scene_renderer().add_tracer( render_scene(), slot.tracer, initial_tracer_matrix )
// where initial_tracer_matrix is a function-local static float4x4().identity().
// scene_renderer::add_tracer is declared/implemented only in the shipped binary's
// vostok/render/facade/sources/scene_renderer.cpp - our reconstruction of that
// compiland lacks the tracer methods (LNK2001). Unblock once the render-facade
// add_tracer/remove_tracer/update_tracer are reconstructed.
bool game_world::attach_tracer( ::survarium::bullet* bullet )
{
	return false;
}

// STATE[STUB]
// claude@NOTE: parked - render-facade tracer wall (same as attach_tracer).
// Recovered body indexes m_bullet_tracers[ bullet->m_tracer_idx ], and if its
// .bullet matches, calls scene_renderer().remove_tracer( render_scene(),
// slot.tracer ) + clears the slot, else LOG_ERROR( "case when
// bullet_tracer.bullet != bullet" ); finally stamps bullet->m_tracer_idx=0xFFFF.
// scene_renderer::remove_tracer lives only in the shipped render-facade
// scene_renderer.cpp (not reconstructed; LNK2001). Unblock with the render facade.
bool game_world::detach_tracer( ::survarium::bullet* bullet )
{
	return false;
}

// STATE[STUB]
// claude@NOTE: parked - render-facade tracer wall (same as attach/detach_tracer).
// Recovered structure (6 stmts, local float4x4 m): builds m from
// create_translation(position) + a direction-derived orthonormal rotation (the
// 0x26-0xd1 cross-product block) + m.set_scale( float3( length, .. ) ), then calls
// scene_renderer().update_tracer( render_scene(), m_bullet_tracers[tracer_idx].tracer,
// m ). scene_renderer::update_tracer is declared/implemented only in the shipped
// render-facade scene_renderer.cpp (not reconstructed here; LNK2001 if called).
// Unblock with the render-facade add/update/remove_tracer reconstruction.
void game_world::update_tracer(
	const u16			tracer_idx,
	float3 const&		position,
	float3 const&		direction,
	const float			length
)
{
}

// STATE[STUB]
// claude@NOTE: parked - sound-facade arg-shape wall + regression risk. Recovered
// structure (3 stmts, local sound::sound_emitter_ptr sound):
//   if ( resource )                    // [esp+4]->[..] guard, plus an emitter c_ptr guard
//   {
//       sound = static_cast_resource_ptr< sound::sound_emitter_ptr >( resource );
//       sound->emit_and_play_once(
//           get_sound_scene(),                                   // engine_base-0x28 = m_sound_scene
//           get_game().get_sound_world().get_logic_world_user(), // m_game[+0x84] -> vtable+0x8
//           position );
//   }
// The emit_and_play_once arg block in the target pushes THREE zero dwords +
// sound before the get_logic_world_user() vcall (0x37-0x3d), which does not map to
// the header's emit_and_play_once( scene, user, float3 const& position, producer=0,
// receiver=0 ) by-ref signature - position appears spilled by value and the
// producer/receiver defaults interleave, so the sound-facade call boundary is the
// same by-value-vs-const& family as play_particle_system. A naive const&
// reconstruction diverges enough that objdiff drops the pairing (unpaired, like
// play_particle). Unblock when the sound::sound_emitter emit_and_play_once arg
// convention / world_user accessor shape is settled in its own match phase.
void game_world::play_sound( resources::unmanaged_resource_ptr const& resource, float3 const& position )
{
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
