////////////////////////////////////////////////////////////////////////////
//	Created		: 03.03.2009
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/base/world.h>
#include <vostok/render/base/platform.h>
#include "command_update_visual.h"

using vostok::render::base_world;
using vostok::render::command_update_visual;

command_update_visual::command_update_visual( base_world& world, u32 id, vostok::math::float4x4 const& transform, bool selected )
:m_world		(world),
m_id		(id),
m_transform	(transform),
m_selected	(selected)
{

}

command_update_visual::~command_update_visual	(  )
{

}

void command_update_visual::execute( )
{
	m_world.platform().update_visual( m_id, m_transform, m_selected);
}

