////////////////////////////////////////////////////////////////////////////
//	Created		: 11.03.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "command_terrain_cell_update_buffer.h"
#include <vostok/render/base/world.h>
#include <vostok/render/base/platform.h>

using vostok::render::base_world;
using vostok::render::buffer_fragment;
using vostok::render::command_terrain_cell_update_buffer;
using vostok::render::buffer_fragment_NEW;

command_terrain_cell_update_buffer::command_terrain_cell_update_buffer(	base_world& world, 
																		vostok::render::visual_ptr v, 
																		vostok::vectora<buffer_fragment_NEW> & fragments,
																		vostok::math::float4x4 const& transform,
																		vostok::render::command_finished_callback const& finish_callback,
																		u32 finish_callback_argument)
:m_world					(world),
m_visual					(v),
m_fragments					(fragments.get_allocator()),
m_transform					(transform),
m_finish_callback			(finish_callback),
m_finish_callback_argument	(finish_callback_argument)
{
	m_fragments.swap(fragments);
}

command_terrain_cell_update_buffer::~command_terrain_cell_update_buffer	()
{
	if( m_finish_callback)
		m_finish_callback( m_finish_callback_argument);
}

void command_terrain_cell_update_buffer::execute()
{
	m_world.platform().terrain_update_cell_buffer( m_visual, m_fragments, m_transform );
}

