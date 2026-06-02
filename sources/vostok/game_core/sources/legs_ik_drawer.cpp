////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/legs_ik_drawer.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/facade/debug_renderer.h>

namespace survarium {

// STATE[73.36%|PARTIAL]: structure exact (4 draw_origin + 4 draw_line, all .c.xyz()
// endpoints resolved). Residual is the call-boundary float-passing convention: the
// target's renderer::draw_origin receives `cross_half_size` in xmm0 (movss), but the
// base links against the unmatched render module's draw_origin, which receives it on
// the stack (fld/fstp [esp]) - x4. Same LTCG arg-passing cause as draw_origin below;
// the drawer source cannot influence the callee's float convention. See .md.
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
	// <0x7b1d78>|0x008|+0x01b:'28'  m_renderer.draw_origin( m_scene, up_leg, cross_half_size, false );
	// <0x7b1d93>|0x023|+0x01b:'29'  m_renderer.draw_origin( m_scene, knee, ... );
	// <0x7b1dae>|0x03e|+0x01b:'30'  m_renderer.draw_origin( m_scene, leg, ... );
	// <0x7b1dc9>|0x059|+0x01b:'31'  m_renderer.draw_origin( m_scene, foot, ... );
	// <0>
	// <0x7b1de4>|0x074|+0x034:'33'  m_renderer.draw_line( m_scene, up_leg.c.xyz(), knee.c.xyz(), up_leg_color, false );
	// <0x7b1e18>|0x0a8|+0x034:'34'  m_renderer.draw_line( m_scene, knee.c.xyz(), leg.c.xyz(), knee_color, false );
	// <0x7b1e4c>|0x0dc|+0x034:'35'  m_renderer.draw_line( m_scene, leg.c.xyz(), foot.c.xyz(), leg_color, false );
	// <0x7b1e80>|0x110|+0x034:'36'  m_renderer.draw_line( m_scene, up_leg.c.xyz(), foot.c.xyz(), foot_color, false );
	// ******
	// Residual: target uses `movss xmm0,[ebp+28h]` to pass cross_half_size; base uses
	// `fld [ebp+28h]; fstp [esp]`. Structure + endpoints otherwise identical.
	m_renderer.draw_origin( m_scene, up_leg, cross_half_size, false );
	m_renderer.draw_origin( m_scene, knee, cross_half_size, false );
	m_renderer.draw_origin( m_scene, leg, cross_half_size, false );
	m_renderer.draw_origin( m_scene, foot, cross_half_size, false );

	m_renderer.draw_line( m_scene, up_leg.c.xyz( ), knee.c.xyz( ), up_leg_color, false );
	m_renderer.draw_line( m_scene, knee.c.xyz( ), leg.c.xyz( ), knee_color, false );
	m_renderer.draw_line( m_scene, leg.c.xyz( ), foot.c.xyz( ), leg_color, false );
	m_renderer.draw_line( m_scene, up_leg.c.xyz( ), foot.c.xyz( ), foot_color, false );
}

// STATE[100%|DONE]
void legs_ik_drawer::draw_cross(
	float3 const&			p,
	float					half_size,
	math::color const&		c,
	bool					use_depth
)
{
	m_renderer.draw_cross( m_scene, p, half_size, c, use_depth );
}

// STATE[62.88%|PARTIAL]: single forwarder, source exact. Residual is the call-boundary
// float-passing convention: the target's renderer::draw_origin receives `half_size` in
// xmm0 (`movss xmm0,[ebp+0Ch]`); the base links the unmatched render module's draw_origin,
// which receives the float on the stack (`fld [ebp+0Ch]; fstp [esp]`). LTCG arg-passing,
// not influenceable from the drawer. Sibling draw_cross/draw_line_capsule (same forward
// pattern) hit 100% because their arg conventions coincide. See .md.
void legs_ik_drawer::draw_origin( float4x4 const& matrix, float half_size, bool use_depth )
{
	// FUNCTION BODY
	// <0x7b1d17>|0x007|+0x01e:'46'  m_renderer.draw_origin( m_scene, matrix, half_size, use_depth );
	// ******
	// Residual: target `movss xmm0,[ebp+0Ch]` vs base `fld/fstp [esp]`.
	m_renderer.draw_origin( m_scene, matrix, half_size, use_depth );
}

// STATE[100%|DONE]
void legs_ik_drawer::draw_line_capsule(
	float4x4 const&			matrix,
	float3 const&			size,
	math::color const&		color,
	bool					use_depth
)
{
	m_renderer.draw_line_capsule( m_scene, matrix, size, color, use_depth );
}

// STATE[79.43%|PARTIAL]: single forwarder, source identical in shape to
// draw_line_capsule (which matched 100%). Residual is purely call-boundary register
// allocation: the target receives the renderer as the __thiscall `this` in ecx and
// pushes `size`, while the base pushes the renderer as a stack arg and keeps `size` in
// eax - i.e. which arg ends in the register vs on the stack at the call. LTCG arg-passing
// choice dictated by the (unmatched) callee; not influenceable from the drawer source. See .md.
void legs_ik_drawer::draw_solid_capsule(
	float4x4 const&			matrix,
	float3 const&			size,
	math::color const&		color,
	bool					use_depth
)
{
	// FUNCTION BODY
	// <0x7b1ec7>|0x007|+0x022:'56'  m_renderer.draw_solid_capsule( m_scene, matrix, size, color, use_depth );
	// ******
	// Residual: target passes the renderer as the __thiscall `this` in ecx (mov ecx,[edx])
	// and pushes `size`; base pushes the renderer as a stack arg and keeps `size` in eax.
	// Reg-vs-stack call-boundary arg passing only.
	m_renderer.draw_solid_capsule( m_scene, matrix, size, color, use_depth );
}

} // namespace survarium
