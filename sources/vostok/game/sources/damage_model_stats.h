// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DAMAGE_MODEL_STATS_H_INCLUDED
#define DAMAGE_MODEL_STATS_H_INCLUDED

#include <vostok/game_core/damage_model.h>	// damage_model_ptr (by-value param)
#include <vostok/game_core/damage_info_type.h>
#include <vostok/render/engine/base_classes.h>	// base_scene_view_ptr
#include <vostok/ui/world.h>

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
	// sushi@TODO: Sibling-panel draw model; recover this panel's original draw consumer and any guard.
	inline	void			draw				( render::ui::renderer& renderer, render::scene_view_ptr const& scene_view )
	{
		m_main_window->draw( renderer, scene_view );
	}

	// sushi@TODO: NPC-style caption/content model; recover original caller, reset/null policy and player-stat formatting.
	inline	void			set_stats			( const damage_model_ptr owner, u32 current_time )
	{
		m_main_window->remove_all_children( );
		if ( !owner )
			return;

		damage_info_type stats;
		owner->fill_stats( stats, current_time );

		ui::text* last_item = NULL;
		for ( u32 i = 0; i < stats.damage_info.size( ); ++i )
		{
			last_item = create_new_group( m_caption_color, stats.damage_info[i].caption.c_str( ), last_item ? last_item->w( ) : NULL );
			for ( u32 j = 0; j < stats.damage_info[i].content.size( ); ++j )
				last_item = create_new_group( m_text_color, stats.damage_info[i].content[j].c_str( ), last_item ? last_item->w( ) : NULL );
		}
	}

private:
	// sushi@TODO: NPC first-column layout model; verify argument roles, column width/placement and original consumer.
	inline	ui::text*		create_new_group	( const u32 font_color, pcstr text, ui::window const* upper_window )
	{
		ui::text* title_text = m_ui_world.create_text( );
		title_text->w( )->set_visible( true );
		float2 position( 0.f, upper_window ? upper_window->get_position( ).y + upper_window->get_size( ).y : 0.f );
		title_text->w( )->set_position( position );
		title_text->w( )->set_size( float2( m_medium_column_width, m_line_height ) );
		title_text->set_font( vostok::ui::fnt_arial );
		title_text->set_text_mode( vostok::ui::tm_default );
		title_text->set_color( font_color );
		m_main_window->add_child( title_text->w( ), true );
		title_text->set_text( text );
		return title_text;
	}

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
