#include "pch.h"
#include "debug_draw_triangles_command.h"
#include <vostok/render/engine/world.h>

namespace vostok {
namespace render {

void defer_execution( base_command& command, base_scene_ptr const& scene );
void defer_execution( base_command& command, base_scene_view_ptr const& scene_view );

namespace debug {

draw_triangles_command::~draw_triangles_command( )
{
}

void draw_triangles_command::execute( )
{
	m_renderer.draw_triangles	( m_scene, m_vertices, m_indices );
}

void draw_triangles_command::defer_execution( )
{
	render::defer_execution		( *this, m_scene );
}

} // namespace debug
} // namespace render
} // namespace vostok
