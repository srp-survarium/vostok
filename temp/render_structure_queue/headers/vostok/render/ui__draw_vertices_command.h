////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_UI_DRAW_VERTICES_COMMAND_H_INCLUDED
#define RENDER_UI_DRAW_VERTICES_COMMAND_H_INCLUDED

/* INCLUDES */
class vostok::render::base_command;
class vostok::render::engine::world;
class vostok::vectora<vostok::render::ui::vertex>;
typedef vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_scene_view_ptr;
class vostok::render::base_scene_view;

/* FORWARD REFS */
class vostok::memory::base_allocator;
class vostok::render::ui::vertex;

namespace vostok {
namespace render {

class ui::draw_vertices_command : public base_command , public boost::noncopyable {
public:
					draw_vertices_command	(
						engine::world&					render_engine_world,
						base_scene_view_ptr const&		scene_view,
						ui::vertex const*				begin,
						ui::vertex const*				end,
						memory::base_allocator&			allocator,
						u32								primitives_type,
						u32								points_type
					);

	virtual	void	execute					( ) override;

	virtual	void	defer_execution			( ) override;

	virtual			~draw_vertices_command	( ) { /* no source */ }

	/* 0x0000 */	/* base_command */
	/* 0x0054 */	/* boost::noncopyable */
private:
	/* 0x0054 */	vectora< ui::vertex >		m_vertices;
	/* 0x0064 */	engine::world&				m_render_engine_world;
	/* 0x0068 */	base_scene_view_ptr			m_scene_view;
	/* 0x006c */	u32							m_primitives_type;
	/* 0x0070 */	u32							m_points_type;
}; // class ui::draw_vertices_command

STATIC_SIZE_ASSERT(ui::draw_vertices_command, 0x74);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_UI_DRAW_VERTICES_COMMAND_H_INCLUDED
