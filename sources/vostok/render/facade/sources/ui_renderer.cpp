#include "pch.h"
#include <vostok/render/facade/sources/ui_renderer.h>

namespace vostok {
namespace render {

// STATE[STUB]
ui::renderer::renderer(
	one_way_render_channel&		channel,
	memory::base_allocator&		allocator,
	engine::world&				engine_world
)
	: m_channel( channel ),
	  m_render_engine_world( engine_world ),
	  m_allocator( allocator )
{
	// FUNCTION BODY[0x7372b0]: 0
	// ******
}

// STATE[STUB]
void ui::renderer::draw_vertices(
	base_scene_view_ptr const&		scene_view,
	ui::vertex const* const			begin,
	ui::vertex const* const			end,
	const u32						primitives_type,
	const u32						points_type
)
{
	// FUNCTION BODY[0x7373e0]: 1
	// <0x7373e4>|0x004|+0x069:'34'
	// ******
}

// STATE[STUB]
void ui::renderer::draw_text(
	base_scene_view_ptr const&		scene_view,
	pcstr const&					text,
	vostok::ui::font const&			font,
	float2 const&					position,
	math::color const&				text_color,
	math::color const&				selection_color,
	const u32						max_line_size,
	const bool						is_multiline,
	const u32						start_selection,
	const u32						end_selection
)
{
	// LOCALS
	// vectora< ui::vertex > 			vertices
	// ******

	// FUNCTION BODY[0x7372d0]: 18
	// <0>
	// <0x7372d3>|0x003|+0x051:'51'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x737324>|0x054|+0x034:'63'
	// <0>
	// <1>
	// <2>
	// <0x737358>|0x088|+0x05f:'67'
	// ******
}

} // namespace render
} // namespace vostok
