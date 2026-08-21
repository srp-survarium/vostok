#include "pch.h"
#include <vostok/render/facade/sources/ui_draw_vertices_command.h>
#include <vostok/render/engine/world.h>

namespace vostok {
namespace render {

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
	R_ASSERT( m_scene_view );
}

void ui::draw_vertices_command::execute( )
{
	m_render_engine_world.draw_ui_vertices	( &*m_vertices.begin(), m_vertices.size(), m_primitives_type, m_points_type );
}

void defer_execution( base_command& command, base_scene_ptr const& scene );
void defer_execution( base_command& command, base_scene_view_ptr const& scene_view );

void ui::draw_vertices_command::defer_execution( )
{
	render::defer_execution	( *this, m_scene_view );
}

} // namespace render
} // namespace vostok
