// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "game.h"

#include <vostok/input/api.h>	// input::create_world / destroy_world
#include <vostok/ui/api.h>	// ui::create_world / destroy_world
#include <vostok/render/facade/game_renderer.h>	// m_renderer.ui() (initialize_ui)
#include "game_memory.h"	// g_allocator (initialize_ui)

namespace survarium {

void game::initialize_input( )
{
	ASSERT							( !m_initialized );

	ASSERT							( !m_input_world );
	m_input_world					= input::create_world( *this, m_engine.get_main_window_handle() );
	ASSERT							( m_input_world );
}

void game::initialize_ui( )
{
	ASSERT							( !m_initialized );

	ASSERT							( !m_ui_world );
	m_ui_world						= ui::create_world( *this, m_renderer.ui(), *g_allocator, m_input_world );
	ASSERT							( m_ui_world );
}

void game::initialize_modules( )
{
	initialize_input				( );
	initialize_ui					( );
}

void game::deinitialize_modules( )
{
	ui::destroy_world				( m_ui_world );
	input::destroy_world			( m_input_world );
}


} // namespace survarium
