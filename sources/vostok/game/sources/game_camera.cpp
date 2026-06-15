////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_camera.h"
#include "camera_director.h"

namespace survarium {

// STATE[STUB]
 game_camera::game_camera( base_game_scene& w ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game_scene( w ),
	// legacy m_vertical_fov -> canonical m_fov_factor
	m_fov_factor( 57.5f ),
	m_near_plane( 0.2f ),
	m_far_plane( 5000.0f )
{
	// FUNCTION BODY[0x5d6ba0]: 4
	// <0x5d6ba0>|0x000|+0x030:'39'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d6bd0>|0x030|      :'44'	}
	// ******
}

// STATE[STUB]
void game_camera::tick( )
{
	// FUNCTION BODY[0x5d6b70]: 8
	// <0x5d6b70>|0x000|+0x000:'47'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d6b70>|0x000|      :'56'	}
	// ******
}

// STATE[STUB]
void game_camera::set_position_direction( float3 const& p, float3 const& d )
{
	m_inverted_view_matrix	= math::invert4x3( math::create_camera_direction( p, d, float3( 0.f, 1.f, 0.f ) ) );

	// FUNCTION BODY[0x5d6c40]: 1
	// <0x5d6c46>|0x006|+0x0b9:'60'
	// ******
}

// STATE[STUB]
void game_camera::on_activate( camera_director* cd )
{
	m_inverted_view_matrix = cd->get_inverted_view_matrix( );

	// FUNCTION BODY[0x5d6b80]: 1
	// <0x5d6b81>|0x001|+0x014:'65'
	// ******
}

// STATE[STUB]
float4x4 game_camera::get_projection_matrix( float2 const& window_size ) const
{
	float const aspect	= window_size.x / window_size.y;

	// legacy m_vertical_fov -> canonical m_fov_factor
	return math::create_perspective_projection(
		m_fov_factor * math::pi / 180.0f,
		aspect,
		m_near_plane,
		m_far_plane
	);

	// FUNCTION BODY[0x5d6be0]: 8
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5d6be0>|0x000|+0x050:'77'
	// ******
}

// STATE[STUB]
float game_camera::get_vertical_fov( ) const
{
	return 0.0f;

	// FUNCTION BODY[0x5d6b60]: 1
	// <0x5d6b60>|0x000|+0x008:'82'
	// ******
}

} // namespace survarium
