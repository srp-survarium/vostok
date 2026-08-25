////////////////////////////////////////////////////////////////////////////
//	Created		: 24.10.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_SOUND_DEBUG_STATS_H_INCLUDED
#define VOSTOK_SOUND_DEBUG_STATS_H_INCLUDED

#include <vostok/sound/sound.h>
#include <vostok/render/facade/game_renderer.h>

namespace vostok {

namespace render {
namespace ui { 
	class renderer; 
} // namespace ui
} // namespace render

namespace ui {
	struct window;
	struct world;
	struct progress_bar;
} // namespace ui

namespace strings {
class text_tree_item;
} // namespace strings

namespace sound {

class world_user;
class sound_scene;
struct sound_scene_statistic;

class VOSTOK_SOUND_API sound_debug_stats : private sound::noncopyable
{
public:
					sound_debug_stats			( memory::base_allocator* allocator, world_user& user, resources::unmanaged_resource_ptr const& scene, ui::world& ui_world );
					~sound_debug_stats			( );

			void	draw						( render::base_scene_ptr const& scene, render::base_scene_view_ptr const& scene_view );
	inline	bool	is_stats_available			( ) const { return m_actual_statistic != -1; }
			void	clear_resources				( world_user& user );
public:
	enum mode
	{
		none = 0,
		overall,
		detail,
		hdr,
		mode_count
	};
	static	void	set_debug_draw_mode			( mode debug_draw_mode );
	static	void	set_detail_view_proxy_id	( u32 proxy_id );
private:
			void	draw_overall_stats			( render::base_scene_ptr const&, render::base_scene_view_ptr const& scene_view  );
#ifdef MASTER_GOLD
	inline	void	draw_hdr_stats				( render::base_scene_ptr const&, render::base_scene_view_ptr const& ) { }
	inline	void	draw_detail_stats			( render::base_scene_ptr const&, render::base_scene_view_ptr const&, u32 ) { }
#else
			void	draw_hdr_stats				( render::base_scene_ptr const& scene, render::base_scene_view_ptr const& scene_view  );
			void	draw_detail_stats			( render::base_scene_ptr const& scene, render::base_scene_view_ptr const& scene_view, u32 proxy_id );
#endif

			void	create_statistic			( );
			void	update_statistic			( );
			void	on_statistic_updated		( );
			void	update_window				( strings::text_tree_item* item, render::base_scene_view_ptr const& scene_view );
private:
	sound_scene_statistic*			m_statistic[2];
	ui::progress_bar**				m_progress_bars;
	ui::world&						m_ui_world;
	ui::window*						m_main_window;
	memory::base_allocator&			m_allocator;
	threading::atomic32_type		m_actual_statistic;
	world_user&						m_world_user;
	sound_scene*					m_scene;

	static threading::atomic32_type	m_s_debug_draw_mode;
	static threading::atomic32_type	m_s_proxy_id;

	friend class sound_scene;
}; // class sound_debug_stats

} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_SOUND_DEBUG_STATS_H_INCLUDED
