#ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_TEXTURE_STORAGE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SOURCES_TEXTURE_STORAGE_H_INCLUDED

#include <functional>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {

class res_texture;
class texture_pool;

class texture_storage {
public:
	texture_storage( );
	~texture_storage( );

	res_texture* get( u32, u32, DXGI_FORMAT )
	{
		// STATE[STUB]
		return 0;
	}

	void release( res_texture const* )
	{
		// STATE[STUB]
	}

private:
	map<u32, texture_pool*, std::less<u32> > m_pools;
	bool m_initialized;
};

STATIC_SIZE_ASSERT( texture_storage, 0x1c );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_TEXTURE_STORAGE_H_INCLUDED
