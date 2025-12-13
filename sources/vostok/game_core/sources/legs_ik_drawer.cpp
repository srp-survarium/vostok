////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/legs_ik_drawer.h>

namespace survarium {

// STATE[STUB]
// void survarium::legs_ik_drawer::draw_leg(vostok::math::float4x4 const&, vostok::math::float4x4 const&, vostok::math::float4x4 const&, vostok::math::float4x4 const&, vostok::math::color const&, vostok::math::color const&, vostok::math::color const&, vostok::math::color const&, float)
void legs_ik_drawer::draw_leg(
	float4x4 const&			up_leg,
	float4x4 const&			knee,
	float4x4 const&			leg,
	float4x4 const&			foot,
	math::color const&		up_leg_color,
	math::color const&		knee_color,
	math::color const&		leg_color,
	math::color const&		foot_color,
	float					cross_half_size
)
{
	// FUNCTION BODY
	// <0x7b1d78>|0x008|+0x01b:'28'
	// <0x7b1d93>|0x023|+0x01b:'29'
	// <0x7b1dae>|0x03e|+0x01b:'30'
	// <0x7b1dc9>|0x059|+0x01b:'31'
	// <0>
	// <0x7b1de4>|0x074|+0x034:'33'
	// <0x7b1e18>|0x0a8|+0x034:'34'
	// <0x7b1e4c>|0x0dc|+0x034:'35'
	// <0x7b1e80>|0x110|+0x034:'36'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_drawer::draw_cross(vostok::math::float3 const&, const float, vostok::math::color const&, bool)
void legs_ik_drawer::draw_cross(
	float3 const&			p,
	float					half_size,
	math::color const&		c,
	bool					use_depth
)
{
	// FUNCTION BODY
	// <0x7b1d47>|0x007|+0x022:'41'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_drawer::draw_origin(vostok::math::float4x4 const&, const float, bool)
void legs_ik_drawer::draw_origin( float4x4 const& matrix, float half_size, bool use_depth )
{
	// FUNCTION BODY
	// <0x7b1d17>|0x007|+0x01e:'46'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_drawer::draw_line_capsule(vostok::math::float4x4 const&, vostok::math::float3 const&, vostok::math::color const&, bool)
void legs_ik_drawer::draw_line_capsule(
	float4x4 const&			matrix,
	float3 const&			size,
	math::color const&		color,
	bool					use_depth
)
{
	// FUNCTION BODY
	// <0x7b1ef7>|0x007|+0x022:'51'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_drawer::draw_solid_capsule(vostok::math::float4x4 const&, vostok::math::float3 const&, vostok::math::color const&, bool)
void legs_ik_drawer::draw_solid_capsule(
	float4x4 const&			matrix,
	float3 const&			size,
	math::color const&		color,
	bool					use_depth
)
{
	// FUNCTION BODY
	// <0x7b1ec7>|0x007|+0x022:'56'
	// ******
}

} // namespace survarium
