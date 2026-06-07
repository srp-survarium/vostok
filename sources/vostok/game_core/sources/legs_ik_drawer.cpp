////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/legs_ik_drawer.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/facade/debug_renderer.h>

namespace survarium {

// STATE[73.36%|PARTIAL]: call-boundary float passing, non-steerable. trail: legs_ik_drawer.md
// STRUCTURE DIFF[target 0x7a1d70 | base 0x59b540]: target 9 / base 9 stmts
// 0x008 <0x1b> | 0x008 <0x1e> | m_renderer.draw_origin( m_scene, up_leg, cross_half_size, false );   SIZE
// 0x023 <0x1b> | 0x026 <0x1e> | m_renderer.draw_origin( m_scene, knee, cross_half_size, false );   SIZE
// 0x03e <0x1b> | 0x044 <0x1e> | m_renderer.draw_origin( m_scene, leg, cross_half_size, false );   SIZE
// 0x059 <0x1b> | 0x062 <0x1e> | m_renderer.draw_origin( m_scene, foot, cross_half_size, false );   SIZE
// 0x074 <0x34> | 0x080 <0x32> | m_renderer.draw_line( m_scene, up_leg.c.xyz( ), knee.c.xyz( ), up_leg_color, false );   SIZE
// 0x0a8 <0x34> | 0x0b2 <0x32> | m_renderer.draw_line( m_scene, knee.c.xyz( ), leg.c.xyz( ), knee_color, false );   SIZE
// 0x0dc <0x34> | 0x0e4 <0x32> | m_renderer.draw_line( m_scene, leg.c.xyz( ), foot.c.xyz( ), leg_color, false );   SIZE
// 0x110 <0x34> | 0x116 <0x32> | m_renderer.draw_line( m_scene, up_leg.c.xyz( ), foot.c.xyz( ), foot_color, false );   SIZE
// ; aligned 1, size-diffs 8, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - all SIZE: draw_origin/draw_line link the unmatched render module, whose float arg convention differs (movss xmm0 vs fld/fstp [esp]); call-boundary, non-steerable.
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

// STATE[62.88%|PARTIAL]: call-boundary float passing, non-steerable. trail: legs_ik_drawer.md
// STRUCTURE DIFF[target 0x7a1d10 | base 0x59b4e0]: target 1 / base 1 stmts
// 0x007 <0x1e> | 0x007 <0x21> | m_renderer.draw_origin( m_scene, matrix, half_size, use_depth );   SIZE
// ; aligned 0, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE: half_size passed in xmm0 (target) vs fld/fstp [esp] (base, unmatched render module's draw_origin); call-boundary, non-steerable.
void legs_ik_drawer::draw_origin( float4x4 const& matrix, float half_size, bool use_depth )
{
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

// STATE[79.43%|PARTIAL]: call-boundary reg-vs-stack arg passing, non-steerable. trail: legs_ik_drawer.md
// STRUCTURE DIFF[target 0x7a1ec0 | base 0x59b690]: target 1 / base 1 stmts
// .. same ..
// ; aligned 1, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - statement aligns; residual is purely call-boundary register allocation (renderer in ecx + push size vs renderer pushed + size in eax) into the unmatched render module; non-steerable.
void legs_ik_drawer::draw_solid_capsule(
	float4x4 const&			matrix,
	float3 const&			size,
	math::color const&		color,
	bool					use_depth
)
{
	m_renderer.draw_solid_capsule( m_scene, matrix, size, color, use_depth );
}

} // namespace survarium
