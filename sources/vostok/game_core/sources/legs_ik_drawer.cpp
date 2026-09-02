// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/legs_ik_drawer.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/facade/debug_renderer.h>

namespace survarium {

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

void legs_ik_drawer::draw_cross(
	float3 const&			p,
	const float					half_size,
	math::color const&		c,
	bool					use_depth
)
{
	m_renderer.draw_cross( m_scene, p, half_size, c, use_depth );
}

void legs_ik_drawer::draw_origin( float4x4 const& matrix, const float half_size, bool use_depth )
{
	m_renderer.draw_origin( m_scene, matrix, half_size, use_depth );
}

void legs_ik_drawer::draw_line_capsule(
	float4x4 const&			matrix,
	float3 const&			size,
	math::color const&		color,
	bool					use_depth
)
{
	m_renderer.draw_line_capsule( m_scene, matrix, size, color, use_depth );
}

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
