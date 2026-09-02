// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_TEXTURE_POOL_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SOURCES_TEXTURE_POOL_H_INCLUDED
#include <vostok/render/core/memory.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/utils.h>

namespace vostok {
namespace render {

class res_texture;

class texture_pool {
private:
	struct slot {
		explicit slot( res_texture* in_texture )
			: occupied( false ), texture( in_texture )
		{
		}

		bool occupied;
		res_texture* texture;
	};

public:
	texture_pool(
		u32 width,
		u32 height,
		DXGI_FORMAT format,
		u32 mips,
		u32 count,
		D3D11_USAGE usage
	)
		: m_unoccupied_count( count ),
		  m_width( width ),
		  m_height( height ),
		  m_format( format ),
		  m_mips( mips ),
		  m_memory_usage( 0 )
	{
		ASSERT( count > 0 );

		m_textures.reserve( count );
		for ( u32 i = 0; i < count; ++i )
			m_textures.push_back( slot( resource_manager::ref( ).create_texture2d_impl( m_width, m_height, NULL, m_format, usage, m_mips, 1, false ) ) );

		m_memory_usage = u32( 0.3f*m_width*m_height*utils::get_format_4x4_pixel_size( m_format )/16 );
	}

	~texture_pool( );

	res_texture* get( )
	{
		if( m_unoccupied_count <= 0)
			return NULL;

		--m_unoccupied_count;

		slot slot = m_textures[m_unoccupied_count];

		slot.occupied = true;

		return slot.texture;
	}

	void release( res_texture const* texture )
	{
		const u32 count = m_textures.size();

		for( u32 i = 0; i < count; ++i)
		{
			if( m_textures[i].texture != texture)
				continue;

			m_textures[i].occupied = false;
			std::swap( *(m_textures.begin() + i), *(m_textures.begin() + m_unoccupied_count));
			return;
		}

		ASSERT( false, "The texture could not be found in the pool!");
	}

	u32 memory_usage( ) { return m_memory_usage; }
	u32 unoccupied_count( ) { return m_unoccupied_count; }

private:
	vector<slot> m_textures;
	u32 m_unoccupied_count;
	u32 m_width;
	u32 m_height;
	DXGI_FORMAT m_format;
	u32 m_mips;
	u32 m_memory_usage;
};

STATIC_SIZE_ASSERT( texture_pool, 0x24 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_TEXTURE_POOL_H_INCLUDED
