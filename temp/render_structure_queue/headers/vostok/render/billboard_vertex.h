////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BILLBOARD_VERTEX_H_INCLUDED
#define RENDER_BILLBOARD_VERTEX_H_INCLUDED

/* INCLUDES */
class const vostok::math::float3;

namespace vostok {
namespace render {

struct billboard_vertex {
	inline		billboard_vertex(
					float3 const&		arg_0,
					float				arg_1,
					const float3		arg_2,
					float				arg_3,
					u8					arg_4
				) { /* no source */ }

	/* 0x0000 */	float4		position_and_scale;
	/* 0x0010 */	float4		direction_and_rotation;
	/* 0x0020 */	u8			corner_index;
}; // struct billboard_vertex

STATIC_SIZE_ASSERT(billboard_vertex, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BILLBOARD_VERTEX_H_INCLUDED
