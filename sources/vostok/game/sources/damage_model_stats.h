// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DAMAGE_MODEL_STATS_H_INCLUDED
#define DAMAGE_MODEL_STATS_H_INCLUDED

#include <vostok/game_core/damage_model.h>	// damage_model_ptr (by-value param)
#include <vostok/game_core/damage_info_type.h>
#include <vostok/render/engine/base_classes.h>	// base_scene_view_ptr

namespace vostok {
namespace render {
	namespace ui {
		class renderer;
	} // namespace ui
} // namespace render
namespace ui {
	class text;
	struct window;
	struct world;
} // namespace ui
} // namespace vostok

namespace survarium {

class damage_model_stats : private boost::noncopyable {
public:
			explicit		damage_model_stats	( ui::world& ui_world );
							~damage_model_stats	( );

	// PDB spells arg_1 vostok::render::base_scene_view_ptr - the same
	// resource_ptr type our render tree typedefs as scene_view_ptr
	inline	void			draw				( render::ui::renderer& arg_0, render::scene_view_ptr const& arg_1 ) { /* no source */ }

	inline	void			set_stats			( const damage_model_ptr arg_0, u32 arg_1 ) { /* no source */ }

private:
	inline	ui::text*		create_new_group	( const u32 arg_0, pcstr arg_1, ui::window const* arg_2 ) { /* no source */ return NULL; }

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
