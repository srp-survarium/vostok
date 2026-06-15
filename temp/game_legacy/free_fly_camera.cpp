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

// REMAINDER (batch 3 skip): both write game_camera::m_inverted_view_matrix, a
// private base member with no canonical setter -> structural surgery, skipped.

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


void free_fly_camera::on_activate( camera_director* cd )
{
	super::on_activate		( cd );
	m_prev_time_ms			= m_game_scene.get_game().time_ms();
	m_prev_delta_sec		= -1.0f;
	m_inverted_view_matrix	= cd->get_inverted_view_matrix	( );
}



}// namespace survarium
