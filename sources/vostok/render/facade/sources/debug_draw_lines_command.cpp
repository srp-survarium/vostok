#include "pch.h"
#include "debug_draw_lines_command.h"
#include <vostok/render/engine/world.h>

namespace vostok {
namespace render {
namespace debug {

void draw_lines_command::execute( )
{
	// FUNCTION BODY[0x7573d0]
	m_renderer.draw_lines		( m_scene, m_vertices, m_indices );
}

} // namespace debug

void defer_execution( base_command& command, base_scene_ptr const& scene )
{
	// FUNCTION BODY[0x757360]
	command.deferred_next					= 0;

	if ( scene->last_command )
		scene->last_command->deferred_next	= &command;
	else
		scene->first_command				= &command;

	scene->last_command						= &command;
}

void defer_execution( base_command& command, base_scene_view_ptr const& scene_view )
{
	// FUNCTION BODY[0x757330]
	command.deferred_next					= 0;

	if ( scene_view->last_command )
		scene_view->last_command->deferred_next	= &command;
	else
		scene_view->first_command			= &command;

	scene_view->last_command				= &command;
}

namespace debug {

void draw_lines_command::defer_execution( )
{
	// FUNCTION BODY[0x757390]
	render::defer_execution		( *this, m_scene );
}

} // namespace debug
} // namespace render
} // namespace vostok
