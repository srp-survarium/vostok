#include "pch.h"
#include <vostok/render/facade/sources/render_facade_world.h>
#include <vostok/render/engine/api.h>
#include <vostok/render/engine/world.h>
#include <vostok/render/facade/sources/engine_renderer.h>
#include <vostok/render/facade/sources/game_renderer.h>

namespace vostok {
namespace render {

world::world(
	memory::base_allocator& logic_allocator,
	memory::base_allocator* editor_allocator,
	configs::binary_config_ptr const& in_config,
	bool is_editor
)
	: m_logic_channel( logic_allocator ),
	  m_editor_channel( editor_allocator ? *editor_allocator : logic_allocator ),
	  m_editor_renderer( 0 ),
	  m_is_logic_enabled( 1 ),
	  m_is_logic_frame_ended( 0 ),
	  m_is_editor_frame_ended( !is_editor ),
	  m_is_editor( is_editor )
{
	m_render_engine_world		= engine::create_world( in_config, is_editor );
	R_ASSERT					( m_render_engine_world );

	m_engine_renderer			= NEW( engine::renderer )( *m_render_engine_world );
	m_game_renderer				= NEW( game::renderer )( *this, *m_render_engine_world );
#ifndef VOSTOK_STATIC_LIBRARIES
	if ( is_editor )
		m_editor_renderer		= NEW( editor::renderer )( *this, *m_render_engine_world );
#endif
}

world::~world( )
{
#ifndef VOSTOK_STATIC_LIBRARIES
	DELETE						( m_editor_renderer );
#endif
	DELETE						( m_game_renderer );
	DELETE						( m_engine_renderer );
	engine::destroy				( m_render_engine_world );
}

engine::renderer& world::engine_renderer( )
{
	R_ASSERT	( m_engine_renderer );
	return		*m_engine_renderer;
}

game::renderer& world::game_renderer( )
{
	R_ASSERT	( m_game_renderer );
	return		*m_game_renderer;
}

void world::end_frame( )
{
	R_ASSERT					( m_is_logic_frame_ended );
	R_ASSERT					( m_is_editor_frame_ended );
	m_render_engine_world->end_frame( );

	if ( m_is_logic_enabled )
		m_is_logic_frame_ended	= false;

	if ( m_is_editor )
		m_is_editor_frame_ended	= false;
}

void world::end_frame_logic( )
{
	m_logic_channel.render_on_end_frame( );

	R_ASSERT					( !m_is_logic_frame_ended || !m_is_logic_enabled );
	m_is_logic_frame_ended		= true;

	if ( m_is_editor_frame_ended )
		end_frame				( );
}

void world::tick( )
{
	if ( !m_is_logic_frame_ended )
		m_logic_channel.render_process_commands( true );

	if ( !m_is_editor_frame_ended )
		m_editor_channel.render_process_commands( true );
}

void world::clear_resources( )
{
	m_render_engine_world->clear_resources( );
}

void world::enable_logic_thread_safety( bool const value )
{
	R_ASSERT_CMP					( m_is_logic_enabled == 1, !=, value );
	threading::interlocked_exchange	( m_is_logic_enabled, value ? 1 : 0 );
	if ( !value )
		threading::interlocked_exchange( m_is_logic_frame_ended, 1 );
}

} // namespace render
} // namespace vostok
