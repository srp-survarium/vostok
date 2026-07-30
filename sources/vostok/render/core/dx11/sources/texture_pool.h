#ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_TEXTURE_POOL_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SOURCES_TEXTURE_POOL_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/core/render_include.h>

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
		D3D11_USAGE
	)
		: m_unoccupied_count( count ),
		  m_width( width ),
		  m_height( height ),
		  m_format( format ),
		  m_mips( mips ),
		  m_memory_usage( 0 )
	{
		// STATE[STUB]
	}

	~texture_pool( );

	res_texture* get( )
	{
		// STATE[STUB]
		return 0;
	}

	void release( res_texture const* )
	{
		// STATE[STUB]
	}

	u32 memory_usage( ) const { return m_memory_usage; }
	u32 unoccupied_count( ) const { return m_unoccupied_count; }

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
