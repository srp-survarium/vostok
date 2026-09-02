// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "npc_stats.h"
#include <vostok/ui/ui.h>
#include <vostok/ui/world.h>
#include <vostok/ai/npc_statistics.h>
#include "human_npc.h"

namespace survarium {

npc_stats::npc_stats( ui::world& ui_world ) :
	m_ui_world( ui_world ),
	m_caption_color( 0xff80ffff ),
	m_text_color( 0xffffff80 ),
	m_line_height( 20.f ),
	m_medium_column_width( 180.f ),
	m_wide_column_width( 360.f )
{
	m_main_window						= m_ui_world.create_window();
	m_main_window->set_visible			( true );
	m_main_window->set_position			( float2( 0.f, 0.f ) );
	m_main_window->set_size				( float2( 1280.f, 720.f ) );
}

ui::text* npc_stats::create_new_group(
	const npc_stats::column_types_enum		column_number,
	const u32								font_color,
	pcstr									text,
	ui::window const*						upper_window
)
{
	const float offsets[]				= {
		0.f,
		m_medium_column_width,
		m_medium_column_width * 2.f,
		m_wide_column_width + m_medium_column_width * 2.f
	};

	const float column_width			= ( column_number == column_3 ) ? m_wide_column_width : m_medium_column_width;

	ui::text* title_text				= m_ui_world.create_text();
	title_text->w()->set_visible		( true );
	float2 position						(
		offsets[ column_number ],
		upper_window ? upper_window->get_position().y + upper_window->get_size().y : 0.f
	);
	title_text->w()->set_position		( position );
	title_text->w()->set_size			( float2( column_width, m_line_height ) );
	title_text->set_font				( vostok::ui::fnt_arial );
	title_text->set_text_mode			( vostok::ui::tm_default );
	title_text->set_color				( font_color );
	m_main_window->add_child			( title_text->w(), true );
	title_text->set_text				( text );
	return								title_text;
}

npc_stats::~npc_stats( )
{
	m_ui_world.destroy_window			( m_main_window );
}

void npc_stats::draw( render::ui::renderer& ui_renderer, render::scene_view_ptr const& scene_view )
{
	m_main_window->draw					( ui_renderer, scene_view );
}

void npc_stats::set_stats( human_npc const* const owner )
{
	m_main_window->remove_all_children	( );

	if ( !owner )
		return;

	ai::npc_statistics					stats;
	owner->fill_stats					( stats );

	ui::text* last_item					= 0;

	// 1st column: sensors and target selectors
	for ( u32 i = 0; i < stats.sensors_state.size(); ++i )
	{
		last_item						= create_new_group(
											column_1,
											m_caption_color,
											stats.sensors_state[i].caption.c_str(),
											last_item ? last_item->w() : 0
										);
		for ( u32 j = 0; j < stats.sensors_state[i].content.size(); ++j )
		{
			last_item					= create_new_group(
											column_1,
											m_text_color,
											stats.sensors_state[i].content[j].c_str(),
											last_item ? last_item->w() : 0
										);
		}
	}

	for ( u32 i = 0; i < stats.selectors_state.size(); ++i )
	{
		last_item						= create_new_group(
											column_1,
											m_caption_color,
											stats.selectors_state[i].caption.c_str(),
											last_item ? last_item->w() : 0
										);
		for ( u32 j = 0; j < stats.selectors_state[i].content.size(); ++j )
		{
			last_item					= create_new_group(
											column_1,
											m_text_color,
											stats.selectors_state[i].content[j].c_str(),
											last_item ? last_item->w() : 0
										);
		}
	}

	last_item							= 0;

	// 2nd column: working memory and blackboard
	last_item							= create_new_group(
											column_2,
											m_caption_color,
											stats.working_memory_state.caption.c_str(),
											last_item ? last_item->w() : 0
										);
	for ( u32 j = 0; j < stats.working_memory_state.content.size(); ++j )
	{
		last_item						= create_new_group(
											column_2,
											m_text_color,
											stats.working_memory_state.content[j].c_str(),
											last_item ? last_item->w() : 0
										);
	}

	last_item							= create_new_group(
											column_2,
											m_caption_color,
											stats.blackboard_state.caption.c_str(),
											last_item ? last_item->w() : 0
										);
	for ( u32 j = 0; j < stats.blackboard_state.content.size(); ++j )
	{
		last_item						= create_new_group(
											column_2,
											m_text_color,
											stats.blackboard_state.content[j].c_str(),
											last_item ? last_item->w() : 0
										);
	}

	last_item							= 0;

	// 3rd column: general info and planning
	last_item							= create_new_group(
											column_3,
											m_caption_color,
											stats.general_state.caption.c_str(),
											last_item ? last_item->w() : 0
										);
	for ( u32 j = 0; j < stats.general_state.content.size(); ++j )
	{
		last_item						= create_new_group(
											column_3,
											m_text_color,
											stats.general_state.content[j].c_str(),
											last_item ? last_item->w() : 0
										);
	}

	last_item							= 0;

	// 4th column: body state
	for ( u32 i = 0; i < stats.body_state.size(); ++i )
	{
		last_item						= create_new_group(
											column_4,
											m_caption_color,
											stats.body_state[i].caption.c_str(),
											last_item ? last_item->w() : 0
										);
		for ( u32 j = 0; j < stats.body_state[i].content.size(); ++j )
		{
			last_item					= create_new_group(
											column_4,
											m_text_color,
											stats.body_state[i].content[j].c_str(),
											last_item ? last_item->w() : 0
										);
		}
	}
}

} // namespace survarium
