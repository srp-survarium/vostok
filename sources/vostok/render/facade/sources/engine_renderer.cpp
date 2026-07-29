////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/facade/engine_renderer.h>
#include <vostok/render/engine/world.h>

namespace vostok {
namespace render {
namespace engine {

renderer::renderer( engine::world& engine_world ) :
	m_render_engine_world	( engine_world )
{
}

void renderer::initialize( bool const is_editor )
{
	m_render_engine_world.initialize	( is_editor );
}

u32 renderer::frame_id( )
{
	return	m_render_engine_world.frame_id( );
}

} // namespace engine
} // namespace render
} // namespace vostok
