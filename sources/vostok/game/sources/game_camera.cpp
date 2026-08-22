////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_camera.h"
#include "camera_director.h"
#include <vostok/console_command.h>

namespace survarium {

float s_camera_far_plane	= 5000.0f;
float default_vertical_fov	= 57.5f;

game_camera::game_camera( base_game_scene& w ) :
	m_game_scene( w ),
	m_near_plane( 0.05f ),
	m_far_plane( s_camera_far_plane ),
	m_fov_factor( 1.0f )
{
}

void game_camera::tick( )
{
}

void game_camera::set_position_direction( float3 const& p, float3 const& d )
{
	m_inverted_view_matrix	= math::invert4x3( math::create_camera_direction( p, d, float3( 0.f, 1.f, 0.f ) ) );
}

void game_camera::on_activate( camera_director* cd )
{
	m_inverted_view_matrix = cd->get_inverted_view_matrix( );
}

float4x4 game_camera::get_projection_matrix( float2 const& window_size ) const
{
	return math::create_perspective_projection(
		( ( default_vertical_fov / 180.0f ) * math::pi ) * m_fov_factor,
		window_size.x / window_size.y,
		m_near_plane,
		m_far_plane
	);
}

float game_camera::get_vertical_fov( ) const
{
	return default_vertical_fov;
}

} // namespace survarium
