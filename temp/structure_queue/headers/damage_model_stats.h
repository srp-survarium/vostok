////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_MODEL_STATS_H_INCLUDED
#define DAMAGE_MODEL_STATS_H_INCLUDED

/* INCLUDES */
struct vostok::ui::window;
struct vostok::ui::world;
class vostok::render::base_scene_view;
class survarium::damage_model;

/* FORWARD REFS */
class vostok::render::ui::renderer;
class vostok::ui::text;

namespace survarium {

class damage_model_stats : public boost::noncopyable {
public:
			explicit		damage_model_stats	( ui::world& ui_world );
							~damage_model_stats	( );

	inline	void			draw				( render::ui::renderer& arg_0, render::base_scene_view_ptr const& arg_1 ) { /* no source */ }

	inline	void			set_stats			( const damage_model_ptr arg_0, u32 arg_1 ) { /* no source */ }

	inline	ui::text*		create_new_group	( const u32 arg_0, pcstr arg_1, ui::window const* arg_2 ) { /* no source */ }

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	ui::world&		m_ui_world;
	/* 0x0004 */	ui::window*		m_main_window;
	/* 0x0008 */	const u32		m_caption_color;
	/* 0x000c */	const u32		m_text_color;
	/* 0x0010 */	const float		m_line_height;
	/* 0x0014 */	const float		m_medium_column_width;
	/* 0x0018 */	const float		m_wide_column_width;
}; // class damage_model_stats

STATIC_SIZE_ASSERT(damage_model_stats, 0x1C);

} // namespace survarium

#endif // #ifndef DAMAGE_MODEL_STATS_H_INCLUDED
