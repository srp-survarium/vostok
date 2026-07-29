////////////////////////////////////////////////////////////////////////////
//	Created		: 21.01.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/base/world.h>
#include <vostok/render/base/platform.h>
#include "command_remove_visual.h"

using vostok::render::base_world;
using vostok::render::command_remove_visual;

command_remove_visual::command_remove_visual( base_world& world, u32 id ):
m_world(world),
m_id(id)
{}
command_remove_visual::~command_remove_visual	(  )
{

}

void command_remove_visual::execute( )
{
	m_world.platform().remove_visual( m_id );
}

