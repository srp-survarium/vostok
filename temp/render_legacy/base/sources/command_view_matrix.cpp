////////////////////////////////////////////////////////////////////////////
//	Created 	: 11.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "command_view_matrix.h"
#include <vostok/render/base/world.h>
#include <vostok/render/base/platform.h>

using vostok::render::command_view_matrix;

command_view_matrix::command_view_matrix( vostok::render::platform& platform, vostok::float4x4 const& view ) :
	m_view						( view ),
	m_platform					( platform )
{
}

void command_view_matrix::execute		( )
{
	m_platform.set_view_matrix	( m_view );
}