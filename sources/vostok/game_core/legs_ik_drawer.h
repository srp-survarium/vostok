// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LEGS_IK_DRAWER_H_INCLUDED
#define LEGS_IK_DRAWER_H_INCLUDED

#include <vostok/render/engine/base_classes.h>
#include <vostok/render/facade/debug_renderer.h>

namespace vostok {
namespace render {
namespace debug {
	class renderer;
}
}
}

// resources::resource_ptr<render::base_scene,resources::unmanaged_intrusive_base>

namespace survarium {

class legs_ik_drawer : private core::noncopyable {
public:
	// sushi@TODO: field-copy constructor model; identify the original drawer construction site.
	inline	explicit	legs_ik_drawer		( render::debug::renderer& renderer, render::scene_ptr scene ) : m_renderer( renderer ), m_scene( scene ) { }

			void		draw_leg			(
							float4x4 const&			up_leg,
							float4x4 const&			knee,
							float4x4 const&			leg,
							float4x4 const&			foot,
							math::color const&		up_leg_color,
							math::color const&		knee_color,
							math::color const&		leg_color,
							math::color const&		foot_color,
							float					cross_half_size
						);

			void		draw_cross			(
							float3 const&			p,
							const float					half_size,
							math::color const&		c,
							bool					use_depth
						);

			void		draw_origin			( float4x4 const& matrix, const float half_size, bool use_depth );

			void		draw_line_capsule	(
							float4x4 const&			matrix,
							float3 const&			size,
							math::color const&		color,
							bool					use_depth
						);

			void		draw_solid_capsule	(
							float4x4 const&			matrix,
							float3 const&			size,
							math::color const&		color,
							bool					use_depth
						);

	// sushi@TODO: renderer forwarding is verified in draw_leg; confirm the original inline boundary.
	inline	void		draw_line			(
							float3 const&			start,
							float3 const&			end,
							math::color const&		color,
							bool					use_depth
						) { m_renderer.draw_line( m_scene, start, end, color, use_depth ); }

private:
	/* 0x0000 */	render::debug::renderer&		m_renderer;
	/* 0x0004 */	render::scene_ptr				m_scene;
}; // class legs_ik_drawer

STATIC_SIZE_ASSERT(legs_ik_drawer, 0x8);

} // namespace survarium

#endif // #ifndef LEGS_IK_DRAWER_H_INCLUDED
