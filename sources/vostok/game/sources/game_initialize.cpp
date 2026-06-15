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

// STATE[STUB]
void game::initialize_input( )
{
	ASSERT							( !m_initialized );

	ASSERT							( !m_input_world );
	m_input_world					= input::create_world( *this, m_engine.get_main_window_handle() );
	ASSERT							( m_input_world );

	// CALL SITE INFO
	// <0x705694> -> HWND__* < unknown >() const
	// ******

	// FUNCTION BODY[0x705680]: 5
	// <0>
	// <1>
	// <2>
	// <0x705681>|0x001|+0x024:'44'
	// <0>
	// ******
}

// STATE[STUB]
void game::initialize_ui( )
{
	ASSERT							( !m_initialized );

	ASSERT							( !m_ui_world );
	m_ui_world						= ui::create_world( *this, m_renderer.ui(), *g_allocator );
	ASSERT							( m_ui_world );

	// FUNCTION BODY[0x705610]: 5
	// <0x705610>|0x000|+0x003:'49'	{
	// <0>
	// <1>
	// <2>
	// <0x705613>|0x003|+0x05c:'53'
	// <0x70566f>|0x05f|-0x004:'53'
	// <0>
	// <0x70566b>|0x05b|+0x00f:'55'
	// <0x70567a>|0x06a|      :'55'	}
	// ******
}

// STATE[STUB]
void game::initialize_modules( )
{
	// legacy also called initialize_animation() between these two; no canonical
	// m_animation_world member - that removed-subsystem call is dropped
	initialize_input				( );
	initialize_ui					( );

	// CALL SITE INFO
	// <0x7056c8> -> HWND__* < unknown >() const
	// ******

	// FUNCTION BODY[0x7056b0]: 5
	// <0>
	// <0x7056b5>|0x005|+0x024:'61'
	// <0x7056d9>|0x029|+0x005:'62'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void game::deinitialize_modules( )
{
	// legacy destroyed animation world between ui and input; no canonical
	// m_animation_world member - that removed-subsystem call is dropped
	ui::destroy_world				( m_ui_world );
	input::destroy_world			( m_input_world );

	// FUNCTION BODY[0x7055b0]: 4
	// <0>
	// <1>
	// <0x7055b4>|0x004|+0x037:'71'
	// <0x7055eb>|0x03b|+0x01d:'72'
	// ******
}


} // namespace survarium
