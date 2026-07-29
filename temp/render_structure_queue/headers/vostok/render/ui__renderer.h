////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_UI_RENDERER_H_INCLUDED
#define RENDER_UI_RENDERER_H_INCLUDED

/* INCLUDES */
class vostok::core::noncopyable;
class vostok::memory::base_allocator;
class vostok::render::engine::world;
class vostok::render::one_way_render_channel;
class vostok::render::base_scene_view;

/* FORWARD REFS */
class vostok::math::color;
class vostok::render::ui::vertex;
class vostok::ui::font;

namespace vostok {
namespace render {

class ui::renderer : public core::noncopyable {
					renderer		(
						one_way_render_channel&		channel,
						memory::base_allocator&		allocator,
						engine::world&				engine_world
					);

public:
			void	draw_vertices	(
						base_scene_view_ptr const&		scene_view,
						ui::vertex const*				begin,
						ui::vertex const*				end,
						u32								primitives_type,
						u32								points_type
					);
			void	draw_text		(
						base_scene_view_ptr const&		scene_view,
						pcstr const&					text,
						ui::font const&					font,
						float2 const&					position,
						math::color const&				text_color,
						math::color const&				selection_color,
						u32								max_line_size,
						bool							is_multiline,
						u32								start_selection,
						u32								end_selection
					);

	inline			~renderer		( ) { /* no source */ }

	/* 0x0000 */	/* core::noncopyable */
private:
	/* 0x0000 */	one_way_render_channel&		m_channel;
	/* 0x0004 */	engine::world&				m_render_engine_world;
	/* 0x0008 */	memory::base_allocator&		m_allocator;
}; // class ui::renderer

STATIC_SIZE_ASSERT(ui::renderer, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_UI_RENDERER_H_INCLUDED
