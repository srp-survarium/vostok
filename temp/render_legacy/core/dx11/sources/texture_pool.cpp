////////////////////////////////////////////////////////////////////////////
//	Created		: 04.06.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "texture_pool.h"
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/utils.h>

namespace vostok {
namespace render {

// HARVEST REMAINDER (phase 3): get/release ported into canonical
// dx11/sources/texture_pool.h. Remaining: ctor and dtor - BLOCKED on canonical
// resource_manager access (create_texture2d_impl / release_impl are private
// there with no texture_pool friendship); see claude@NOTEs in the canonical
// texture_pool.h/.cpp stubs.

texture_pool::texture_pool	( u32 width, u32 height, DXGI_FORMAT format, u32 mips, u32 count, D3D_USAGE usage):
m_width				(width),
m_height			(height),
m_format			(format),
m_mips				(mips),
m_memory_usage		(0),
m_unoccupied_count	(count)
{
	ASSERT( count > 0 );

	m_textures.reserve( count);
	for( u32 i = 0; i < count; ++i)	
		m_textures.push_back( slot( resource_manager::ref().create_texture2d_impl( m_width, m_height, NULL, m_format, usage, m_mips)));

	m_memory_usage = u32(0.3f*m_width*m_height*utils::get_format_4x4_pixel_size(m_format)/16);
}

texture_pool::~texture_pool	()
{
	for( u32 i = 0; i < m_textures.size(); ++i)	
		resource_manager::ref().release_impl( m_textures[i].texture);
}

} // namespace render
} // namespace vostok

