// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_FACADE_SOURCES_UI_DRAW_VERTICES_COMMAND_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_UI_DRAW_VERTICES_COMMAND_H_INCLUDED
#include <boost/noncopyable.hpp>
#include <vostok/render/facade/base_command.h>
#include <vostok/render/facade/sources/ui_renderer.h>
#include <vostok/vectora.h>

namespace vostok {

namespace memory {
class base_allocator;
}

namespace render {

namespace engine {
class world;
}

namespace ui {

class draw_vertices_command : public base_command, private boost::noncopyable {
public:
	draw_vertices_command(
		engine::world& render_engine_world,
		base_scene_view_ptr const& scene_view,
		vertex const* begin,
		vertex const* end,
		memory::base_allocator& allocator,
		u32 primitives_type,
		u32 points_type
	);

	virtual void execute( );
	virtual void defer_execution( );
	virtual ~draw_vertices_command( ) { /* no source */ }

private:
	vectora< vertex > m_vertices;
	engine::world& m_render_engine_world;
	base_scene_view_ptr m_scene_view;
	u32 m_primitives_type;
	u32 m_points_type;
};

STATIC_SIZE_ASSERT( draw_vertices_command, 0x74 );

} // namespace ui
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_UI_DRAW_VERTICES_COMMAND_H_INCLUDED
