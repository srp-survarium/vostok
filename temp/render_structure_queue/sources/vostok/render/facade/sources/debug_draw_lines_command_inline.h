////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\FACADE\SOURCES\DEBUG_DRAW_LINES_COMMAND_INLINE_H_INCLUDED
#define VOSTOK\RENDER\FACADE\SOURCES\DEBUG_DRAW_LINES_COMMAND_INLINE_H_INCLUDED

namespace vostok {
namespace render {
namespace debug {

// STATE[STUB]
inline  draw_lines_command::draw_lines_command(
	memory::base_allocator&				allocator,
	base_scene_ptr const&				scene,
	engine::world&						renderer,
	vectora< vertex_colored > const&	vertices,
	vectora< u16 > const&				indices,
	bool								use_depth
)
{
	// FUNCTION BODY[0x16a70]: 1
	// <0x16af3>|0x083|-0x008:'27'
	// <0>
	// <0x16aeb>|0x07b|+0x00c:'29'
	// ******
}

// STATE[STUB]
inline  draw_lines_command::draw_lines_command<2,2>(
	base_scene_ptr const&		scene,
	engine::world&				renderer,
	memory::base_allocator&		allocator,
	const vertex_colored[2]&	vertices,
	const u16[2]&				indices,
	bool						use_depth
)
{
	// FUNCTION BODY[0x16d50]: 1
	// <0x16dce>|0x07e|-0x008:'63'
	// <0>
	// <0x16dc6>|0x076|+0x00c:'65'
	// ******
}

// STATE[STUB]
inline  draw_lines_command::draw_lines_command<vostok::buffer_vector>(
	base_scene_ptr const&			scene,
	engine::world&					renderer,
	memory::base_allocator&			allocator,
	buffer_vector< vertex_colored > const&	vertices,
	buffer_vector< u16 > const&		indices,
	bool							use_depth
)
{
	// FUNCTION BODY[0x16f70]: 0
	// <0x16ff3>|0x083|-0x008:'81'
	// <0x16feb>|0x07b|+0x00c:'82'
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

#endif // #ifndef VOSTOK\RENDER\FACADE\SOURCES\DEBUG_DRAW_LINES_COMMAND_INLINE_H_INCLUDED
