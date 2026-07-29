////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCREEN_IMAGE_VERTEX_H_INCLUDED
#define RENDER_SCREEN_IMAGE_VERTEX_H_INCLUDED

namespace vostok {
namespace render {

struct screen_image_vertex {
	inline		screen_image_vertex	(
					float		arg_0,
					float		arg_1,
					float		arg_2,
					float		arg_3
				) { /* no source */ }

	/* 0x0000 */	float2		position;
	/* 0x0008 */	float2		text_coords;
}; // struct screen_image_vertex

STATIC_SIZE_ASSERT(screen_image_vertex, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCREEN_IMAGE_VERTEX_H_INCLUDED
