////////////////////////////////////////////////////////////////////////////
//	Created		: 09.02.2009
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "command_projection_matrix.h"
#include <vostok/render/base/world.h>
#include <vostok/render/base/platform.h>

using vostok::render::command_projection_matrix;

command_projection_matrix::command_projection_matrix( vostok::render::platform& platform, vostok::float4x4 const& projection ) :
m_projection						( projection ),
m_platform					( platform )
{
}

void command_projection_matrix::execute		( )
{
	m_platform.set_projection_matrix	( m_projection );
}