////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\game_camera.h"

namespace survarium {

// STATE[STUB]
explicit game_camera::game_camera( base_game_scene& w )
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
	// FUNCTION BODY[0x5d6c40]: 1
	// <0x5d6c46>|0x006|+0x0b9:'60'
	// ******
}

// STATE[STUB]
void game_camera::on_activate( camera_director* cd )
{
	// FUNCTION BODY[0x5d6b80]: 1
	// <0x5d6b81>|0x001|+0x014:'65'
	// ******
}

// STATE[STUB]
float4x4 game_camera::get_projection_matrix( float2 const& window_size ) const
{
	return vostok::math::float4x4();

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
