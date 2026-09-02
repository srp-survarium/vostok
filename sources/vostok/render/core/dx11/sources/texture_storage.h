// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_TEXTURE_STORAGE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SOURCES_TEXTURE_STORAGE_H_INCLUDED
#include <functional>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/res_texture.h>
#include <vostok/render/core/utils.h>

#include "texture_pool.h"

namespace vostok {
namespace render {

class res_texture;
class texture_pool;

inline u32 get_hash( u32 width, u32 height, DXGI_FORMAT format )
{
	return (utils::log_2( width) | (utils::log_2( height)<<5) | ( (u32)format<<10));
}

class texture_storage {
public:
	texture_storage( );
	~texture_storage( );

	res_texture* get( u32 width, u32 height, DXGI_FORMAT format )
	{
		map<u32, texture_pool*, std::less<u32> >::iterator it = m_pools.find( get_hash( width, height, format));

		if( it == m_pools.end())
			return NULL;

		return it->second->get( );
	}

	void release( res_texture const* texture )
	{
		map<u32, texture_pool*, std::less<u32> >::iterator it = m_pools.find( get_hash( texture->width(), texture->height(), texture->format()));

		if( it == m_pools.end())
		{
			ASSERT( false, "There is no initialized pool with specified parameters.");
			return;
		}

		it->second->release( texture);
	}

private:
	map<u32, texture_pool*, std::less<u32> > m_pools;
	bool m_initialized;
};

STATIC_SIZE_ASSERT( texture_storage, 0x1c );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_TEXTURE_STORAGE_H_INCLUDED
