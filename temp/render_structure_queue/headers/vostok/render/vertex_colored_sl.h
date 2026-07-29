////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERTEX_COLORED_SL_H_INCLUDED
#define RENDER_VERTEX_COLORED_SL_H_INCLUDED

namespace vostok {
namespace render {

struct vertex_colored_sl {
	inline		vertex_colored_sl	( ) { /* no source */ }

	/* 0x0000 */	float3		position;
	/* 0x000c */	u32			color;
	/* 0x0010 */	float3		begin_line_position;
	/* 0x001c */	float		shift;
	/* 0x0020 */	u32			pattern;
}; // struct vertex_colored_sl

STATIC_SIZE_ASSERT(vertex_colored_sl, 0x24);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERTEX_COLORED_SL_H_INCLUDED
