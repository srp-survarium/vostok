////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "debug_draw_triangles_command.h"
#include "debug_renderer.h"

using vostok::render::debug::draw_triangles_command;
using vostok::render::platform;
using vostok::render::vertex_colored;

draw_triangles_command::~draw_triangles_command	( )
{
}

void draw_triangles_command::execute			( )
{
	m_renderer.draw_triangles_impl	( m_vertices, m_indices );
}