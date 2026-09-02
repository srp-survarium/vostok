// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "damage_model_stats.h"

#include <vostok/ui/world.h>

namespace survarium {

damage_model_stats::damage_model_stats( ui::world& ui_world ) :
	m_ui_world( ui_world ),
	m_caption_color( 0xff80ffff ),
	m_text_color( 0xffffff80 ),
	m_line_height( 20.f ),
	m_medium_column_width( 180.f ),
	m_wide_column_width( 360.f )
{
	m_main_window = m_ui_world.create_window( );
	m_main_window->set_visible( true );
	m_main_window->set_position( float2( 0.f, 0.f ) );
	m_main_window->set_size( float2( 1280.f, 720.f ) );
}

damage_model_stats::~damage_model_stats( )
{
	m_ui_world.destroy_window( m_main_window );
}

} // namespace survarium
