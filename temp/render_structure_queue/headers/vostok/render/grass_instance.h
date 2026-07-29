////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_INSTANCE_H_INCLUDED
#define RENDER_GRASS_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::math::color;
struct vostok::render::grass_template;

namespace vostok {
namespace render {

struct grass_instance {
		grass_instance	(
			const u32				in_id,
			grass_template*			in_tempalate,
			math::color const&		in_color,
			float4x4 const&			in_transform,
			u8						in_layer,
			float					in_wind_scale
		);

	/* 0x0000 */	grass_template*		m_template;
	/* 0x0004 */	math::color			m_color;
	/* 0x0008 */	float4x4			m_transform;
	/* 0x0048 */	float				m_wind_scale;
	/* 0x004c */	u32					m_index;
	/* 0x0050 */	u8					m_layer_id;
}; // struct grass_instance

STATIC_SIZE_ASSERT(grass_instance, 0x54);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_INSTANCE_H_INCLUDED
