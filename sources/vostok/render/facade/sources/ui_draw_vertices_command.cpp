#include "pch.h"
#include <vostok/render/facade/sources/ui_draw_vertices_command.h>

namespace vostok {
namespace render {

// STATE[STUB]
ui::draw_vertices_command::draw_vertices_command(
	engine::world&					render_engine_world,
	base_scene_view_ptr const&		scene_view,
	ui::vertex const* const			begin,
	ui::vertex const* const			end,
	memory::base_allocator&			allocator,
	const u32						primitives_type,
	const u32						points_type
)
	: base_command( true ),
	  m_vertices( allocator, begin, end ),
	  m_render_engine_world( render_engine_world ),
	  m_scene_view( scene_view ),
	  m_primitives_type( primitives_type ),
	  m_points_type( points_type )
{
	// FUNCTION BODY[0x7711b0]: 1
	// <0>
	// ******
}

// STATE[STUB]
void ui::draw_vertices_command::execute( )
{
	// FUNCTION BODY[0x771220]: 1
	// <0x771222>|0x002|+0x03b:'34'
	// ******
}

// STATE[STUB]
void ui::draw_vertices_command::defer_execution( )
{
	// FUNCTION BODY[0x771170]: 1
	// <0x771170>|0x000|+0x021:'48'
	// <0x771191>|0x021|-0x001:'48'
	// <0x771190>|0x020|+0x010:'49'
	// ******
}

} // namespace render
} // namespace vostok
