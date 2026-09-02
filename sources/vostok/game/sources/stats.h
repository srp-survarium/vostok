// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef STATS_H_INCLUDED
#define STATS_H_INCLUDED

// PDB spells the draw param vostok::render::base_scene_view_ptr - the same
// resource_ptr type our render tree typedefs as scene_view_ptr
#include <vostok/render/engine/base_classes.h>

namespace vostok {
namespace ui {
	struct text;
	struct window;
	struct world;
} // namespace ui
namespace render {
namespace ui {
	class renderer;
} // namespace ui
} // namespace render
} // namespace vostok

namespace survarium {

class stats : private boost::noncopyable {
public:
			explicit	stats						( ui::world& ui_world );
						~stats						( );

			void		draw						( render::ui::renderer& w, render::scene_view_ptr const& scene_view );

			void		set_fps_stats				( const float fps );
	inline	void		set_active_scene			( pcstr arg_0 ) { /* no source */ }
			void		set_player_linear_speed		( const float speed );
			void		set_player_angular_speed	( const float speed );
	inline	void		set_player_stamina			( const float arg_0 ) { /* no source */ }
			void		set_camera_stats			( float3 const& pos, float3 const& dir );
			void		set_resources_stats			( pcstr str );
			void		set_crosshair_info			( const float dist );
	inline	void		set_can_use_value			( const bool arg_0 ) { /* no source */ }
	inline	void		set_dispersion_components	( pcstr arg_0 ) { /* no source */ }
	inline	void		set_player_logic_state		( pcstr arg_0 ) { /* no source */ }
	inline	void		set_weapon_logic_state		( pcstr arg_0 ) { /* no source */ }

private:
			void		create						( );

	/* 0x0000 */	/* boost::noncopyable */
protected:
	/* 0x0000 */	ui::world&		m_ui_world;
	/* 0x0004 */	ui::window*		m_main_window;
	/* 0x0008 */	ui::text*		m_fps;
	/* 0x000c */	ui::text*		m_camera_position;
	/* 0x0010 */	ui::text*		m_camera_direction;
	/* 0x0014 */	ui::text*		m_crosshair_distance;
	/* 0x0018 */	ui::text*		m_resources_activity;
	/* 0x001c */	ui::text*		m_active_scene_info;
	/* 0x0020 */	ui::text*		m_player_linear_speed;
	/* 0x0024 */	ui::text*		m_player_angular_speed;
	/* 0x0028 */	ui::text*		m_player_stamina;
	/* 0x002c */	ui::text*		m_can_use;
	/* 0x0030 */	ui::text*		m_dispersion_components;
	/* 0x0034 */	ui::text*		m_player_logic_state;
	/* 0x0038 */	ui::text*		m_weapon_logic_state;
	/* 0x003c */	float			m_crosshair_dist;
	/* 0x0040 */	const u32		m_odd_row_color;
	/* 0x0044 */	const u32		m_even_row_color;
}; // class stats

STATIC_SIZE_ASSERT(stats, 0x48);

} // namespace survarium

#endif // #ifndef STATS_H_INCLUDED
