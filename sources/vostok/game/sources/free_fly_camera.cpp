// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "free_fly_camera.h"
#include "base_game_scene.h"
#include "game.h"
#include "camera_director.h"
#include "key_binder.h"
#include <vostok/console_command.h>
#include <vostok/input/gamepad.h>
#include <vostok/input/keyboard.h>
#include <vostok/input/mouse.h>
#include <vostok/input/world.h>
namespace vostok {
namespace console_commands {
	VOSTOK_CORE_API void execute( pcstr str, execution_filter filter );
} // namespace console_commands
} // namespace vostok
namespace survarium {
free_fly_camera::free_fly_camera( base_game_scene& w, camera_director& cd ) :
	game_camera			( w ),
	m_camera_director	( cd ),
	m_prev_time_ms		( 0 ),
	m_prev_delta_sec	( -1.0f ),
	m_mouse_move		( 0, 0, 0 )
{
}
bool free_fly_camera::on_keyboard_action(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( input_world );
	game&				current_game = get_game_scene().get_game();
	toggle_action_enum	actions_mask_type;
	game_action_id const action_id	= current_game.get_key_binder().get_binded_action(
		key, actions_mask_type, 1 );
	if ( action == input::kb_key_down )
	{
		switch ( action_id )
		{
		case kPAUSE:
			current_game.toggle_pause	( );
			break;
		case kSERIALIZE_PLAYER_STATE:
			vostok::console_commands::execute	( "serialize_player_state", vostok::console_commands::execution_filter_all );
			break;
		case kDESERIALIZE_PLAYER_STATE:
			vostok::console_commands::execute	( "deserialize_player_state", vostok::console_commands::execution_filter_all );
			break;
		}
		if ( actions_mask_type == toggle_action )
			m_keyb_events.push_back	( key );
	}
	else if ( action == input::kb_key_hold && actions_mask_type == hold_action )
		m_keyb_events.push_back	( key );
	return false;
}

bool free_fly_camera::on_gamepad_action(
	input::world*					input_world,
	input::gamepad_button			button,
	input::enum_gamepad_action		action
)
{
	VOSTOK_UNREFERENCED_PARAMETER		( action );

	if ( button == input::gamepad_x ) {
		input_world->get_gamepad()->set_vibration	( input::gamepad_vibrator_left, input_world->get_gamepad()->get_vibration( input::gamepad_vibrator_left ) + 0.01f );
		return					true;
	}

	if ( button == input::gamepad_b ) {
		input_world->get_gamepad()->set_vibration	( input::gamepad_vibrator_right, input_world->get_gamepad()->get_vibration( input::gamepad_vibrator_right ) + 0.01f );
		return					true;
	}
	return	false;
}

bool free_fly_camera::on_mouse_key_action(
	input::world*					input_world,
	input::mouse_button				button,
	input::enum_mouse_key_action	action
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( input_world );

	if ( action == input::ms_key_hold )
		m_mouse_events.push_back	( button );

	return false;
}

bool free_fly_camera::on_mouse_move(
	input::world*		input_world,
	s32					x,
	s32					y,
	s32					z
)
{
	VOSTOK_UNREFERENCED_PARAMETERS	( input_world );

	m_mouse_move.x += math::deg2rad( float( x ) );
	m_mouse_move.y += math::deg2rad( float( y ) );
	m_mouse_move.z += math::deg2rad( float( z ) );

	return false;
}
bool free_fly_camera::keyb_event_present( s32 e )
{
	return std::find( m_keyb_events.begin(), m_keyb_events.end(), e ) != m_keyb_events.end();
}
bool free_fly_camera::mouse_event_present( s32 e )
{
	return std::find( m_mouse_events.begin(), m_mouse_events.end(), e ) != m_mouse_events.end();
}
void free_fly_camera::build_view_matrix(
	float2 const&		raw_angles,
	const float			shift_forward,
	const float			shift_right,
	const float			shift_up
)
{
	float4x4 const view_inverted	= m_camera_director.get_inverted_view_matrix( );
	float3 const angles_zxy			= view_inverted.get_angles( math::rotation_zxy );
	float3 new_angles_zxy			= float3( angles_zxy.x - raw_angles.x, angles_zxy.y - raw_angles.y, angles_zxy.z * 0.75f );
	new_angles_zxy.x				= math::clamp_r( new_angles_zxy.x, -math::pi_d2, math::pi_d2 );
	float4x4 rotation				= math::create_rotation( new_angles_zxy, math::rotation_zxy );
	float3 const position			=	view_inverted.c.xyz( ) +
										view_inverted.i.xyz( ) * shift_right +
										view_inverted.j.xyz( ) * shift_up +
										view_inverted.k.xyz( ) * shift_forward;
	float4x4 const translation		= math::create_translation( position );
	m_inverted_view_matrix			= rotation * translation;
}

void free_fly_camera::on_focus( bool b_focus_enter )
{
	game_camera::on_focus		( b_focus_enter );
	if ( b_focus_enter )
	{
		get_game_scene().get_game().input_world().add_handler	( *this );
	}
	else
		get_game_scene().get_game().input_world().remove_handler	( *this );
}

void free_fly_camera::on_activate( camera_director* cd )
{
	game_camera::on_activate	( cd );

	m_prev_time_ms				= get_game_scene().get_game().game_time_ms( );
	m_prev_delta_sec			= -1.0f;
	m_inverted_view_matrix		= cd->get_inverted_view_matrix( );

	m_inverted_view_matrix.j.xyz( )	= float3( 0.f, 1.f, 0.f ); // claude@NOTE: target writes xyz directly; base materializes the faithful float3 temporary.
	m_inverted_view_matrix.i.xyz( )	= math::cross_product( m_inverted_view_matrix.j.xyz( ), m_inverted_view_matrix.k.xyz( ) );
}

void free_fly_camera::tick( )
{

	u32 const current_time_ms		= get_game_scene().
		get_game().
		game_permanent_time_ms( );
	float const current_time_delta	= float( current_time_ms - m_prev_time_ms );

	if ( m_prev_delta_sec < 0.0f )
		m_prev_delta_sec		= current_time_delta;
	else
		m_prev_delta_sec		= current_time_delta * 0.1f + m_prev_delta_sec * 0.9f;

	float factor				= 60.f * 0.001f * m_prev_delta_sec;
	float angle_factor			= 0.5f;

	m_prev_time_ms				= current_time_ms;

	static u32 counter = 0;
	if ( keyb_event_present( input::key_q ) )
		LOG_INFO( "timedelta: [%d] %f", counter++, current_time_delta );
	if ( m_keyb_events.empty() &&
		m_mouse_events.empty() &&
		math::is_zero( m_mouse_move.x ) && math::is_zero( m_mouse_move.y ) && math::is_zero( m_mouse_move.z ) )
		return;

	if ( keyb_event_present( input::key_lcontrol ) || keyb_event_present( input::key_rcontrol ) )
		factor					*= 20.f;

	if ( keyb_event_present( input::key_lshift ) || keyb_event_present( input::key_rshift ) )
		factor					*= .1f;

	if ( keyb_event_present( input::key_lalt ) || keyb_event_present( input::key_ralt ) )
		angle_factor			*= .1f;

	float						forward = 0.f;
	float						right	= 0.f;
	float						up		= 0.f;

	if ( mouse_event_present( input::mouse_button_left ) )
		forward					+= factor * .1f;

	if ( mouse_event_present( input::mouse_button_right ) )
		forward					-= factor * .1f;

	if ( keyb_event_present( input::key_d ) )
		right				+= factor * .1f;

	if ( keyb_event_present( input::key_a ) )
		right				-= factor * .1f;

	if ( keyb_event_present( input::key_w ) )
		up					+= factor * .1f;

	if ( keyb_event_present( input::key_s ) )
		up					-= factor * .1f;

	build_view_matrix			(
		float2( angle_factor * m_mouse_move.y, angle_factor * m_mouse_move.x * 0.75f ),
		forward, right, up );

	m_keyb_events.clear();
	m_mouse_events.clear();

	m_mouse_move.x = 0.0f;
	m_mouse_move.y = 0.0f;
	m_mouse_move.z = 0.0f;
}

} // namespace survarium
