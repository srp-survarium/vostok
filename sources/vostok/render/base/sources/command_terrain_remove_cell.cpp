////////////////////////////////////////////////////////////////////////////
//	Created		: 11.03.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "command_terrain_remove_cell.h"
#include <vostok/render/base/world.h>
#include <vostok/render/base/platform.h>

using vostok::render::base_world;
using vostok::render::buffer_fragment;
using vostok::render::command_terrain_remove_cell;


command_terrain_remove_cell::command_terrain_remove_cell( base_world& world, vostok::render::visual_ptr v, bool beditor )
:m_world		(world),
m_visual		(v),
m_beditor		(beditor)
{
}

command_terrain_remove_cell::~command_terrain_remove_cell	()
{
}

void command_terrain_remove_cell::execute()
{
	m_world.platform().terrain_remove_cell( m_visual, m_beditor );
}