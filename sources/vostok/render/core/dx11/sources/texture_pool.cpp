// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "texture_pool.h"
namespace vostok {
namespace render {

texture_pool::~texture_pool( )
{
	for ( u32 i = 0; i < m_textures.size( ); ++i )
		resource_manager::ref( ).release_impl( m_textures[i].texture );
}

} // namespace render
} // namespace vostok
