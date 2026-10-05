// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "step_manager.h"
#include "game_world.h"
#include "game.h"
#include "player.h"
#include "base_network_client.h"
#include <vostok/physics/world.h>
#include <vostok/physics/ray_result.h>
#include <vostok/physics/base_physics_object.h>
#include <vostok/game_core/game_material_manager.h>
#include <vostok/game_core/material_pair.h>
#include <vostok/sound/world.h>
#include <vostok/sound/sound_emitter.h>

namespace survarium {

 step_manager::step_manager( ) :
	m_decal_id( 0 )
{
}

// claude@NOTE: LTCG custom-convention residual - the target promotes `world` to ecx and
// `position` to eax (no slots), passes `a` as the only [ebp+] stack arg, and dispatches
// add_decal / play_particle through the bullet_manager_engine sub-vptr ([world+0xC4]).
// emit_and_play_once records all 6 parameters after default-argument expansion.
// Source SHAPE is the target's 17-stmt body (lines 34-67); our base lowers
// to 15 stmts: the resource_ptr/intrusive_ptr c_ptr() + emit/decal/particle arg-marshaling
// inline as fewer line-table entries here than in the target (cross-module inline boundary),
// and /Ox spills the lone-use foot_material_id local the target keeps in a register (the extra
// base `const u8 foot_material_id` named local is that /Ox projection, not a phantom to delete).
// The quantity gap + byte residual are LTCG/arg-passing and cross-module inline boundaries.
// The remaining bullet_manager_engine sub-vptr accessor belongs to game_world; take the
// inline-vs-call hit here.
void step_manager::on_step(
	player const&		a,
	float3 const&		position,
	float3 const&		direction,
	game_world&			world
) const
{
	physics::closest_ray_result ray_result	= world.get_physics_world( )->ray_test( float3( position.x, position.y + 1.f, position.z ), float3( 0.f, -1.f, 0.f ), 2.f, 8, 48 );

	if ( !ray_result.object )
		return;

	base_network_client* const network_client	= world.get_game( ).get_network_client( );

	input_mode_type_enum input_mode;
	if ( network_client &&
		 network_client->get_current_player( ).c_ptr( ) &&
		 network_client->get_current_player( )->id == a.id )
		input_mode						= world.get_current_input_mode( );
	else
		input_mode						= third_person_mode;

	u8 const foot_material_id			= ( input_mode == first_person_mode )
											? a.foot_1st_view_game_material_id
											: a.foot_3rd_view_game_material_id;

	u16 const triangle_material_id		= ray_result.object->get_collision_group( );

	material_pair const* const pair		= world.get_game_material_manager( ).get_pair( triangle_material_id, foot_material_id );

	sound::sound_emitter_ptr sound		= static_cast_resource_ptr< sound::sound_emitter_ptr >( pair->sound( ) );

	if ( sound.c_ptr( ) )
		sound->emit_and_play_once(
			world.get_sound_scene( ),
			world.get_game( ).get_sound_world( ).get_logic_world_user( ),
			ray_result.hit_point_world
		);

	if ( pair->decal1( ).c_ptr( ) )
		world.add_decal(
			pair->decal1( ),
			m_decal_id++,
			pair->decal1_size( ),
			0.1f,
			ray_result.hit_point_world,
			direction,
			ray_result.hit_normal_world,
			true
		);

	if ( m_decal_id == 0x20 )
		m_decal_id						= 0;

	if ( pair->has_particle( ) )
		world.play_particle(
			pair->particle( ),
			ray_result.hit_point_world,
			float3( 0.f, 0.f, 0.f ),
			float3( 0.f, 0.f, 0.f )
		);
}


} // namespace survarium
