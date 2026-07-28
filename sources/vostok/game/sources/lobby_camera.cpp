////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_camera.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/input/mouse.h>
#include <vostok/input/world.h>
#include <vostok/physics/world.h>
#include <vostok/physics/ray_result.h>

namespace survarium {

 lobby_camera::lobby_camera( base_game_scene& w ) :
	game_camera( w ),
	m_target_point( 0.0f, 1.5f, 0.0f ),
	m_capture_move( false ),
	m_yaw( 0.0f ),
	m_pitch( 0.0f ),
	m_target_distance_to_focus_point( 1.5f ),
	m_current_distance_to_focus_point( 1.5f ),
	m_z_mouse_axis( 0.0f )
{
	m_rotation_delta = float2( 0.0f, 0.0f );
}

bool lobby_camera::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		actions_mask
)
{
	return false;
}

bool lobby_camera::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		actions_mask
)
{
	return false;
}

bool lobby_camera::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	actions_mask
)
{
	if ( get_game_scene( ).is_mouse_over_ui( ) )
		return false;

	if ( button == input::mouse_button_right )	// 0x152
	{
		if ( !m_capture_move && actions_mask == input::ms_key_down )
			m_capture_move	= true;
		else if ( actions_mask == input::ms_key_up )
			m_capture_move	= false;

		return true;
	}

	return false;
}

// claude@NOTE: PARKED on a cross-module wall. The body reads the namespace-scope
// global survarium::g_mouse_sensitivity (?g_mouse_sensitivity@survarium@@3MA), whose
// definition is owned by key_binder.cpp (still a stub TU); referencing it here would
// leave it undefined at link, same wall as player_input_handler::on_mouse_move.
// Recovered shape (10 stmts): if (!get_game_scene().is_mouse_over_ui()) m_z_mouse_axis
// -= z * <epsilon>; if (!m_capture_move) return false; float2 render_window_size =
// get_game_scene().get_game().engine().get_render_window_size(); then update m_yaw via
// x*g_mouse_sensitivity*pi*<k> and m_pitch via (y/window)*... and (-z)*... ; return true.
// Next step: match key_binder.cpp (defines g_mouse_sensitivity), then reconstruct.
// STATE[STUB]
bool lobby_camera::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	return false;
}

void lobby_camera::on_before_processing( input::world* input_world, const u32 current_time_in_ms )
{
	m_rotation_delta	= float2( 0.0f, 0.0f );
	m_z_mouse_axis		= 0.0f;
}

void lobby_camera::on_after_processing( input::world* input_world )
{
}

s32 lobby_camera::input_priority( )
{
	return 10;
}

void lobby_camera::on_activate( camera_director* cd )
{
}

void lobby_camera::on_deactivate( )
{
}

// claude@NOTE: 14/14 statement STRUCTURE MATCH. Residual is a 0x10 frame-size delta:
// the target folds the inline float3(m_pitch,0,0) temp into the freed mul4x3 output
// area, shifting every later [esp+N] offset (and an edx/eax choice in the position
// store) - an LTCG scheduling artifact, not source-steerable.
void lobby_camera::tick( )
{
	m_yaw	+= m_rotation_delta.x;
	m_pitch	+= m_rotation_delta.y;

	if ( !math::is_zero( m_z_mouse_axis ) )
	{
		m_target_distance_to_focus_point	= m_current_distance_to_focus_point + m_z_mouse_axis;
		math::clamp	( m_target_distance_to_focus_point, 1.5f, 40.0f );
	}

	math::clamp	( m_pitch, -math::pi_d3, math::pi_d3 );

	float4x4 new_inverted_view	= float4x4( ).identity( );
	new_inverted_view			= new_inverted_view * math::create_rotation_y( m_yaw );
	new_inverted_view			= new_inverted_view * math::create_rotation( float3( m_pitch, 0.0f, 0.0f ) );

	m_current_distance_to_focus_point	= m_target_distance_to_focus_point;

	float3 ray_direction		= -new_inverted_view.k.xyz( );

	process_collision	( m_target_point, ray_direction, m_current_distance_to_focus_point );

	new_inverted_view.c.xyz( )	= m_target_point - ray_direction * m_current_distance_to_focus_point;

	m_inverted_view_matrix		= new_inverted_view;
}

void lobby_camera::process_collision(
	float3 const&		target_point,
	float3 const&		direction,
	float&				distance_to_focus_point
)
{
	physics::closest_ray_result result = get_game_scene( ).get_physics_world( )->ray_test(
		target_point, direction, distance_to_focus_point, 16, 8 );

	if ( result.object )
		distance_to_focus_point = ( result.hit_point_world - target_point ).length( );
}

// claude@NOTE: STRUCTURE MATCH. Sole residual is a vtable-slot delta: target calls
// game::input_world() at slot [edx+28h], our game.h declares it at [edx+20h]. That is
// a game.h virtual-ordering divergence (another unit's structure) - not fixable here.
void lobby_camera::on_focus( bool b_focus_enter )
{
	if ( b_focus_enter )
		get_game_scene().get_game().input_world().add_handler	( *this );
	else
		get_game_scene().get_game().input_world().remove_handler	( *this );
}

} // namespace survarium
