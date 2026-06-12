////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NPC_STATS_H_INCLUDED
#define NPC_STATS_H_INCLUDED

/* INCLUDES */
enum const survarium::npc_stats::column_types_enum;
struct vostok::ui::window;
struct vostok::ui::world;
class vostok::render::base_scene_view;
class survarium::human_npc const* const;

/* FORWARD REFS */
class vostok::render::ui::renderer;
class vostok::ui::text;

namespace survarium {

class npc_stats : public boost::noncopyable {
public:
	explicit		npc_stats		( ui::world& ui_world );
					~npc_stats		( );

	void			draw			( render::ui::renderer& ui_renderer, render::base_scene_view_ptr const& scene_view );

	void			set_stats		( human_npc const* const owner );

	ui::text*		create_new_group(
						const npc_stats::column_types_enum		arg_0 /* npc_stats::column_types_enum column_number */,
						const u32								font_color,
						pcstr									text,
						ui::window const*						upper_window
					);

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	ui::world&		m_ui_world;
	/* 0x0004 */	ui::window*		m_main_window;
	/* 0x0008 */	const u32		m_caption_color;
	/* 0x000c */	const u32		m_text_color;
	/* 0x0010 */	const float		m_line_height;
	/* 0x0014 */	const float		m_medium_column_width;
	/* 0x0018 */	const float		m_wide_column_width;
}; // class npc_stats

STATIC_SIZE_ASSERT(npc_stats, 0x1C);

} // namespace survarium

#endif // #ifndef NPC_STATS_H_INCLUDED
