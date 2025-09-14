////////////////////////////////////////////////////////////////////////////
//	Created		: 11.03.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "command_terrain_exchange_texture.h"
#include <vostok/render/base/world.h>
#include <vostok/render/base/platform.h>
#include <vostok/memory_macros.h>

using vostok::render::base_world;
using vostok::render::buffer_fragment;
using vostok::render::command_terrain_exchange_texture;
using vostok::render::texture_string;


command_terrain_exchange_texture::command_terrain_exchange_texture( base_world& world, texture_string const& old_texture, texture_string const& new_texture)
:m_world		(world),
m_old			(old_texture),
m_new			(new_texture)

{
}

void command_terrain_exchange_texture::execute()
{
	m_world.platform().terrain_exchange_texture( m_old.c_str(), m_new.c_str());
}