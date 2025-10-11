////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "legs_ik_drawer.h"

namespace survarium {

// STATE[STUB]
// void survarium::legs_ik_drawer::draw_leg(vostok::math::float4x4 const&, vostok::math::float4x4 const&, vostok::math::float4x4 const&, vostok::math::float4x4 const&, vostok::math::color const&, vostok::math::color const&, vostok::math::color const&, vostok::math::color const&, float)
void legs_ik_drawer::draw_leg(
	vostok::math::float4x4 const&		up_leg,
	vostok::math::float4x4 const&		knee,
	vostok::math::float4x4 const&		leg,
	vostok::math::float4x4 const&		foot,
	vostok::math::color const&			up_leg_color,
	vostok::math::color const&			knee_color,
	vostok::math::color const&			leg_color,
	vostok::math::color const&			foot_color,
	float								cross_half_size)
{
	// FUNCTION BODY
	// <0x7b1d78>|0x000|0x000:'28'
	// <0x7b1d93>|0x01b|0x01b:'29'
	// <0x7b1dae>|0x036|0x01b:'30'
	// <0x7b1dc9>|0x051|0x01b:'31'
	// 1
	// <0x7b1de4>|0x06c|0x01b:'33'
	// <0x7b1e18>|0x0a0|0x034:'34'
	// <0x7b1e4c>|0x0d4|0x034:'35'
	// <0x7b1e80>|0x108|0x034:'36'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_drawer::draw_cross(vostok::math::float3 const&, const float, vostok::math::color const&, bool)
void legs_ik_drawer::draw_cross(
	vostok::math::float3 const&		p,
	float							half_size,
	vostok::math::color const&		c,
	bool							use_depth)
{
	// FUNCTION BODY
	// <0x7b1d47>|0x000|0x000:'41'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_drawer::draw_origin(vostok::math::float4x4 const&, const float, bool)
void legs_ik_drawer::draw_origin( vostok::math::float4x4 const& matrix, float half_size, bool use_depth )
{
	// FUNCTION BODY
	// <0x7b1d17>|0x000|0x000:'46'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_drawer::draw_line_capsule(vostok::math::float4x4 const&, vostok::math::float3 const&, vostok::math::color const&, bool)
void legs_ik_drawer::draw_line_capsule(
	vostok::math::float4x4 const&		matrix,
	vostok::math::float3 const&			size,
	vostok::math::color const&			color,
	bool								use_depth)
{
	// FUNCTION BODY
	// <0x7b1ef7>|0x000|0x000:'51'
	// ******
}

// STATE[STUB]
// void survarium::legs_ik_drawer::draw_solid_capsule(vostok::math::float4x4 const&, vostok::math::float3 const&, vostok::math::color const&, bool)
void legs_ik_drawer::draw_solid_capsule(
	vostok::math::float4x4 const&		matrix,
	vostok::math::float3 const&			size,
	vostok::math::color const&			color,
	bool								use_depth)
{
	// FUNCTION BODY
	// <0x7b1ec7>|0x000|0x000:'56'
	// ******
}

	/* TYPEDEFS

	typedef
		vostok::render::terrain_data*
		iterator_type;

} // namespace survarium
