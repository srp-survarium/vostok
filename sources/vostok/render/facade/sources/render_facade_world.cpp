#include "pch.h"
#include <vostok/render/facade/sources/render_facade_world.h>

namespace vostok {
namespace render {

world::world(
	memory::base_allocator& logic_allocator,
	memory::base_allocator* editor_allocator,
	configs::binary_config_ptr const&,
	bool is_editor
)
	: m_logic_channel( logic_allocator ),
	  m_editor_channel( editor_allocator ? *editor_allocator : logic_allocator ),
	  m_render_engine_world( 0 ),
	  m_engine_renderer( 0 ),
	  m_game_renderer( 0 ),
	  m_editor_renderer( 0 ),
	  m_is_logic_enabled( 1 ),
	  m_is_logic_frame_ended( 0 ),
	  m_is_editor_frame_ended( !is_editor ),
	  m_is_editor( is_editor )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7380d0]
}

world::~world( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7381b0]
}

engine::renderer& world::engine_renderer( )
{
	// FUNCTION BODY[0x738020]
	return *m_engine_renderer;
}

game::renderer& world::game_renderer( )
{
	// FUNCTION BODY[0x738010]
	return *m_game_renderer;
}

void world::end_frame( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x737fe0]
}

void world::end_frame_logic( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x738080]
}

void world::tick( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x738030]
}

void world::clear_resources( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x738060]
}

void world::enable_logic_thread_safety( bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x737fb0]
}

} // namespace render
} // namespace vostok
