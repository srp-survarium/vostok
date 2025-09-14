////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/facade/engine_renderer.h>
#include <vostok/render/engine/world.h>

using vostok::render::engine::renderer;

renderer::renderer					( vostok::render::engine::world& engine_world ) :
	m_render_engine_world			( engine_world )
{
}

void renderer::initialize			( )
{
	m_render_engine_world.initialize( );
}

u32 renderer::frame_id				( )
{
	return							( m_render_engine_world.frame_id( ) );
}