////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\FACADE\SOURCES\DEBUG_DRAW_TRIANGLES_COMMAND_INLINE_H_INCLUDED
#define VOSTOK\RENDER\FACADE\SOURCES\DEBUG_DRAW_TRIANGLES_COMMAND_INLINE_H_INCLUDED

namespace vostok {
namespace render {
namespace debug {

// STATE[STUB]
inline  draw_triangles_command::draw_triangles_command(
	base_scene_ptr const&				scene,
	engine::world&						renderer,
	vectora< vertex_colored > const&	vertices,
	vectora< u16 > const&				indices,
	bool								use_depth
)
{
	// FUNCTION BODY[0x16b80]: 1
	// <0x16c17>|0x097|-0x008:'26'
	// <0>
	// <0x16c0f>|0x08f|+0x00c:'28'
	// ******
}

// STATE[STUB]
inline  draw_triangles_command::draw_triangles_command<3,3>(
	base_scene_ptr const&		scene,
	engine::world&				renderer,
	memory::base_allocator&		allocator,
	const vertex_colored[3]&	vertices,
	const u16[3]&				indices,
	bool						use_depth
)
{
	// FUNCTION BODY[0x16e70]: 1
	// <0x16eef>|0x07f|-0x006:'44'
	// <0>
	// <0x16ee9>|0x079|+0x00f:'46'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	u16*
	// 	iterator_type;

	// typedef
	// 	vostok::render::vertex_colored*
	// 	iterator_type;

	// typedef
	// 	vostok::vectora< u16 >
	// 	indices_type;

	// typedef
	// 	vostok::vectora< vostok::render::vertex_colored >
	// 	vertices_type;

	// ******

} // namespace debug
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK\RENDER\FACADE\SOURCES\DEBUG_DRAW_TRIANGLES_COMMAND_INLINE_H_INCLUDED
