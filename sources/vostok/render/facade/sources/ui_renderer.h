#ifndef VOSTOK_RENDER_FACADE_SOURCES_UI_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_UI_RENDERER_H_INCLUDED

#include <vostok/c_array_functions.h>
#include <vostok/detail_noncopyable.h>
#include <vostok/math_color.h>
#include <vostok/math_float2.h>
#include <vostok/math_float4.h>
#include <vostok/render/api.h>
#include <vostok/render/engine/base_classes.h>

namespace vostok {

namespace memory {
class base_allocator;
}

namespace ui {
struct font;
}

namespace render {

class one_way_render_channel;

namespace engine {
class world;
}

namespace editor {
class renderer;
}

namespace game {
class renderer;
}

namespace ui {

struct vertex {
	vertex( ) { }

	vertex( float const x, float const y, float const z, u32 const color, float const u, float const v )
		: m_position( x, y, z, 0.f ),
		  m_color( color ),
		  m_uv( u, v )
	{
	}

	float4 m_position;
	u32 m_color;
	float2 m_uv;
};

STATIC_SIZE_ASSERT( vertex, 0x1C );

class VOSTOK_RENDER_API renderer : private core::noncopyable {
private:
	friend class editor::renderer;
	friend class game::renderer;

	renderer( one_way_render_channel& channel, memory::base_allocator& allocator, engine::world& engine_world );

public:
	void draw_vertices(
		base_scene_view_ptr const& scene_view,
		vertex const* begin,
		vertex const* end,
		u32 primitives_type,
		u32 points_type
	);

	template < int Count >
	void draw_vertices(
		base_scene_view_ptr const& scene_view,
		vertex const (&vertices)[Count],
		u32 primitives_type,
		u32 points_type
	)
	{
		draw_vertices( scene_view, array_begin( vertices ), array_end( vertices ), primitives_type, points_type );
	}

	void draw_text(
		base_scene_view_ptr const& scene_view,
		pcstr const& text,
		vostok::ui::font const& font,
		float2 const& position,
		math::color const& text_color,
		math::color const& selection_color,
		u32 max_line_size,
		bool is_multiline,
		u32 start_selection,
		u32 end_selection
	);

private:
	one_way_render_channel& m_channel;
	engine::world& m_render_engine_world;
	memory::base_allocator& m_allocator;
};

STATIC_SIZE_ASSERT( renderer, 0xC );

} // namespace ui
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_UI_RENDERER_H_INCLUDED
