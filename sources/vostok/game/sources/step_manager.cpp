// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "step_manager.h"
#include "game_world.h"
#include "game.h"
#include "player.h"
#include "base_network_client.h"
#include <vostok/physics/world.h>
#include <vostok/physics/ray_result.h>
#include <vostok/physics/base_physics_object.h>
#include <vostok/physics/rigid_body_base.h>
#include <vostok/game_core/game_material_manager.h>
#include <vostok/game_core/material_pair.h>
#include <vostok/sound/world.h>
#include <vostok/sound/sound_emitter.h>

namespace survarium {

 step_manager::step_manager( ) :
	m_decal_id( 0 )
{
}

// sushi@TODO: Recover sound, decal and particle call-boundary statement attribution.











void step_manager::on_step(
	player const&		a,
	float3 const&		position,
	float3 const&		direction,
	game_world&			world
) const
{
	physics::closest_ray_result ray_result	= world.get_physics_world( )->ray_test( float3( position.x, position.y + 1.f, position.z ), float3( 0.f, -1.f, 0.f ), 2.f, 48, 8 );

	if ( !ray_result.object )
		return;

	input_mode_type_enum const input_mode	= world.get_game( ).get_network_client( )->is_player_current( a.id )
											? world.get_current_input_mode( )
											: third_person_mode;

	u8 const foot_material_id			= ( input_mode == first_person_mode )
											? a.foot_1st_view_game_material_id
											: a.foot_3rd_view_game_material_id;

	u16 const triangle_material_id		= static_cast< physics::bt_rigid_body_base* >( ray_result.object )->get_triangle_material( ray_result.triangle_index, ray_result.is_shape_index );

	material_pair const* const pair		= world.get_game_material_manager( ).get_pair( foot_material_id, triangle_material_id );

	sound::sound_emitter_ptr sound		= static_cast_resource_ptr< sound::sound_emitter_ptr >( pair->sound( ) );

	if ( sound.c_ptr( ) )
		sound->emit_and_play_once(
			world.get_sound_scene( ),
			world.get_game( ).get_sound_world( ).get_logic_world_user( ),
			ray_result.hit_point_world,
			0,
			0,
			input_mode == first_person_mode
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
			float3( 1.f, 0.f, 0.f ),
			float3( 0.f, 1.f, 0.f )
		);
}


} // namespace survarium
