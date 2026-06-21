////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
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

// claude@NOTE: STRUCTURE MATCH (3 stmts, same lines); residual is a cross-module
// ui_world layout wall. Gold inlines ui::create_world here; the inlined body
// allocates sizeof(ui_world) - target pushes 0x58, our base pushes 0x50. Our
// ui/sources/ui_world.h drops m_scene_view (commented out) and our ui/api.h
// create_world has 3 params not the shipped 4 (engine&,renderer&,allocator&,
// input::world*). Fixing it belongs to the ui module's TU, not here.
void game::initialize_ui( )
{
	ASSERT							( !m_initialized );

	ASSERT							( !m_ui_world );
	m_ui_world						= ui::create_world( *this, m_renderer.ui(), *g_allocator );
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
