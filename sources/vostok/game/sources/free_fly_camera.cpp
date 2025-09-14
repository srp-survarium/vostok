////////////////////////////////////////////////////////////////////////////
//	Created 	: 24.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "free_fly_camera.h"
#include <vostok/console_command.h>

#include "game.h"
#include "game_world.h"
#include "camera_director.h"
#include <vostok/input/gamepad.h>
#include <vostok/input/keyboard.h>
#include <vostok/input/mouse.h>
#include <vostok/input/world.h>

using vostok::input::gamepad;
using vostok::input::keyboard;
using vostok::input::mouse;

namespace survarium{

free_fly_camera::free_fly_camera( survarium::game_scene& w, camera_director_ptr& cd ) 
:super				( w ),
m_prev_time_ms		( 0 ),
m_prev_delta_sec	( -1.0f ),
m_mouse_move		( 0,0,0 ),
m_camera_director	( cd )
{
}

bool free_fly_camera::on_keyboard_action( vostok::input::world* input_world, 
										vostok::input::enum_keyboard key, 
										vostok::input::enum_keyboard_action action )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( input_world );

	if(action == vostok::input::kb_key_hold)
		m_keyb_events.push_back	( key );

	return false;
}

bool free_fly_camera::on_gamepad_action( vostok::input::world* input_world, 
										vostok::input::gamepad_button button, 
										vostok::input::enum_gamepad_action action )
{
	VOSTOK_UNREFERENCED_PARAMETER		(action);

	if ( button == vostok::input::gamepad_x) {
		input_world->get_gamepad()->set_vibration	( input::gamepad_vibrator_left, input_world->get_gamepad()->get_vibration( input::gamepad_vibrator_left ) + 0.01f );
		return					true;
	}

	if ( button == vostok::input::gamepad_b) {
		input_world->get_gamepad()->set_vibration	( input::gamepad_vibrator_right, input_world->get_gamepad()->get_vibration( input::gamepad_vibrator_right ) + 0.01f );
		return					true;
	}
	return	false;
}

bool free_fly_camera::on_mouse_key_action( vostok::input::world* input_world, 
										vostok::input::mouse_button button, 
										vostok::input::enum_mouse_key_action action )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( input_world );

	if(action==vostok::input::ms_key_hold)
		m_mouse_events.push_back	( button );

	return false;
}

bool free_fly_camera::on_mouse_move( vostok::input::world* input_world, int x, int y, int z )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( input_world );

	m_mouse_move.x += x;
	m_mouse_move.y += y;
	m_mouse_move.z += z;
	return false;
}

bool free_fly_camera::keyb_event_present( int e )
{
	return std::find(m_keyb_events.begin(), m_keyb_events.end(), e) != m_keyb_events.end();
}

bool free_fly_camera::mouse_event_present( int e )
{
	return std::find(m_mouse_events.begin(), m_mouse_events.end(), e) != m_mouse_events.end();
}

void free_fly_camera::build_view_matrix( vostok::math::float2 const& raw_angles, float const shift_forward, float const shift_right, float const shift_up )
{
	float4x4 const view_inverted	= m_camera_director->get_inverted_view_matrix( );
	float3 const angles_zxy			= view_inverted.get_angles( math::rotation_zxy );

	float3 new_angles_zxy			= float3( raw_angles.x + angles_zxy.x, raw_angles.y+angles_zxy.y, angles_zxy.z );
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
	super::on_focus ( b_focus_enter );
	if(b_focus_enter)
	{
		m_game_scene.get_game().input_world().add_handler						(*this);
	}else
		m_game_scene.get_game().input_world().remove_handler(*this);
}

void free_fly_camera::on_activate( camera_director* cd )
{
	super::on_activate		( cd );
	m_prev_time_ms			= m_game_scene.get_game().time_ms();
	m_prev_delta_sec		= -1.0f;
	m_inverted_view_matrix	= cd->get_inverted_view_matrix	( );
}

void free_fly_camera::tick( )
{

#if VOSTOK_PLATFORM_WINDOWS
	u32 const game_time_ms		= m_game_scene.get_game().time_ms( );

	ASSERT						( game_time_ms >= m_prev_time_ms );
	float const current_time_delta	= float( game_time_ms - m_prev_time_ms );

	if(m_prev_delta_sec<0.0f)
		m_prev_delta_sec		= current_time_delta;
	else
		m_prev_delta_sec		= current_time_delta*0.1f + m_prev_delta_sec*0.9f;

	float factor				= 60.f * 0.001f * m_prev_delta_sec;
	float angle_factor			= 0.5f;

	m_prev_time_ms				= game_time_ms;

	static u32 counter = 0;
	if(keyb_event_present(input::key_q))
		LOG_INFO("[%d] %f",counter++, current_time_delta);

	if( m_keyb_events.empty() && 
		m_mouse_events.empty() && 
		math::is_zero(m_mouse_move.x) && math::is_zero(m_mouse_move.y) && math::is_zero(m_mouse_move.z) )
		return;

	if ( keyb_event_present(input::key_lcontrol) || keyb_event_present(input::key_rcontrol) )
		factor					*= 20.f;

	if ( keyb_event_present( input::key_lshift ) || keyb_event_present( input::key_rshift ) )
		factor					*= .1f;

	if ( keyb_event_present( input::key_lalt ) || keyb_event_present( input::key_ralt ) ) 
		angle_factor			*= .1f;

	float						forward = 0.f;
	float						right	= 0.f;
	float						up		= 0.f;

	if ( mouse_event_present(input::mouse_button_left) )
		forward					+= factor*.1f;
	
	if ( mouse_event_present(input::mouse_button_right) )
		forward					-= factor*.1f;

	if ( keyb_event_present(input::key_d) )
		right				+= factor*.1f;
	
	if ( keyb_event_present(input::key_a) )
		right				-= factor*.1f;
	
	if ( keyb_event_present(input::key_w) )
		up					+= factor*.1f;
	
	if ( keyb_event_present(input::key_s) )
		up					-= factor*.1f;

	float const	angle_x			= angle_factor * math::deg2rad( m_mouse_move.y );
	float const	angle_y			= angle_factor * math::deg2rad( m_mouse_move.x ) * 0.75f;
	build_view_matrix			( float2( angle_x, angle_y ), forward, right, up );

	m_keyb_events.clear();
	m_mouse_events.clear();
	m_mouse_move.x = 0.0f;
	m_mouse_move.y = 0.0f;
	m_mouse_move.z = 0.0f;
#else
	raw<gamepad>::ptr const gamepad	= m_game_world.get_game().input_world().get_gamepad();
	if ( !( gamepad->get_state().buttons & input::gamepad_x ) )
		gamepad->set_vibration	( input::gamepad_vibrator_left, gamepad->get_vibration( input::gamepad_vibrator_left ) - .01f );

	if ( !( gamepad->get_state().buttons & input::gamepad_b ) )
		gamepad->set_vibration	( input::gamepad_vibrator_right, gamepad->get_vibration( input::gamepad_vibrator_right ) - .01f );

	ASSERT						( gamepad );
	gamepad::state const& state	= gamepad->get_state ( );
	u32 const game_time_ms		= m_game_world.get_game().time_ms( );
	float const time_delta		= float( game_time_ms - m_start_time_ms );
	m_start_time_ms				= game_time_ms;
	float						factor = 10.f*.001f*time_delta;
	build_view_matrix			(
		float2(
			state.left_thumb_stick.y*.001f,
			state.left_thumb_stick.x*.001f*.75f
		),
		( state.left_trigger - state.right_trigger ) * factor,
		state.right_thumb_stick.x,
		state.right_thumb_stick.y
	);
#endif
}



}// namespace survarium