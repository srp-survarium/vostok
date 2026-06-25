////////////////////////////////////////////////////////////////////////////
//	Created		: 30.01.2009
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "ui_world.h"
#include <vostok/render/world.h>
#include <vostok/render/facade/ui_renderer.h>

namespace vostok {
namespace ui {

ui_world::ui_world(
		engine& engine,
		render::ui::renderer& renderer,
		memory::base_allocator& allocator,
		input::world* input_world
	) :
	m_input_world		( input_world ),
	m_engine			( engine ),
	m_allocator			( allocator ),
	m_renderer			( renderer ),
	m_base_screen_size	( 1024.0f, 768.0f ),
	m_font_manager		( allocator )
{
	m_timer.start		( );
}

void ui_world::set_base_screen_size( u32 const size_x, u32 const size_y )
{
	m_base_screen_size.x	= (float)size_x;
	m_base_screen_size.y	= (float)size_y;
}

void ui_world::tick()
{
}

void ui_world::clear_resources		( )
{
}

void ui_world::on_device_reset()
{
	m_font_manager.on_device_reset();
}

} // namespace vostok
} // namespace ui
