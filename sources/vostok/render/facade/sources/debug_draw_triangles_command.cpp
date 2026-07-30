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
	// FUNCTION BODY[0x757460]
}

void draw_triangles_command::execute( )
{
	// FUNCTION BODY[0x7574c0]
	m_renderer.draw_triangles	( m_scene, m_vertices, m_indices );
}

void draw_triangles_command::defer_execution( )
{
	// FUNCTION BODY[0x757420]
	render::defer_execution		( *this, m_scene );
}

} // namespace debug
} // namespace render
} // namespace vostok
