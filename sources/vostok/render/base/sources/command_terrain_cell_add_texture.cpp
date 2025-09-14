////////////////////////////////////////////////////////////////////////////
//	Created		: 11.03.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "command_terrain_cell_add_texture.h"
#include <vostok/render/base/world.h>
#include <vostok/render/base/platform.h>
#include <vostok/memory_macros.h>

using vostok::render::base_world;
using vostok::render::buffer_fragment;
using vostok::render::command_terrain_cell_add_texture;
using vostok::render::texture_string;


command_terrain_cell_add_texture::command_terrain_cell_add_texture( base_world& world, vostok::render::visual_ptr v, texture_string const & texture, u32 tex_user_id)
:m_world		(world),
m_texture		(texture),
m_visual		(v),
m_tex_user_id	(tex_user_id)
{
}

command_terrain_cell_add_texture::~command_terrain_cell_add_texture	()
{
}

void command_terrain_cell_add_texture::execute()
{
	m_world.platform().terrain_add_cell_texture( m_visual, m_texture.c_str(), m_tex_user_id);
}

