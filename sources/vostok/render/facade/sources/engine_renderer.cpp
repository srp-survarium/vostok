#include "pch.h"
#include <vostok/render/facade/sources/engine_renderer.h>
#include <vostok/render/engine/world.h>

namespace vostok {
namespace render {
namespace engine {

renderer::renderer( engine::world& engine_world )
	: m_render_engine_world( engine_world )
{
	// FUNCTION BODY[0x6e3240]
}

void renderer::initialize( bool is_editor )
{
	// FUNCTION BODY[0x6e3250]
	m_render_engine_world.initialize( is_editor );
}

u32 renderer::frame_id( )
{
	// FUNCTION BODY[0x6e3230]
	return m_render_engine_world.frame_id( );
}

} // namespace engine
} // namespace render
} // namespace vostok
