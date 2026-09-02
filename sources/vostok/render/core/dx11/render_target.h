// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_RENDER_TARGET_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RENDER_TARGET_H_INCLUDED
#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/res_texture.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/shared_string.h>

namespace vostok {
namespace render {

class resource_manager;

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

enum enum_rt_usage {
	enum_rt_usage_depth_stencil,
	enum_rt_usage_render_target,
};

class render_target : public resource_intrusive_base {
private:
	friend class resource_manager;
	friend class resource_intrusive_base;

	render_target( );
	~render_target( );
	void destroy_impl( ) const;

public:
	void save_as( pcstr file_name );

	void create(
		pcstr name,
		u32 width,
		u32 height,
		DXGI_FORMAT format,
		enum_rt_usage usage,
		D3D11_USAGE memory_usage,
		u32 sample_count
	);

	void create(
		pcstr name,
		u32 width,
		u32 height,
		DXGI_FORMAT format,
		enum_rt_usage usage,
		res_texture_ptr texture,
		u32 first_array_slice_index,
		u32 mip_slice
	);

	void create_3d(
		pcstr name,
		u32 width,
		u32 height,
		u32 depth,
		DXGI_FORMAT format,
		enum_rt_usage usage,
		D3D11_USAGE memory_usage
	);

	void destroy( );

	ID3D11RenderTargetView* get_target_view( ) const { return m_rt; }
	ID3D11DepthStencilView* get_depth_stencil_view( ) const { return m_zrt; }
	bool valid( ) { return !!m_texture; }
	u32 width( ) { return m_width; }
	u32 height( ) { return m_height; }
	DXGI_FORMAT format( ) { return m_format; }
	enum_rt_usage usage( ) { return m_usage; }
	shared_string name( ) const { return m_name; }

	void set_name( pcstr const name )
	{
		R_ASSERT( !m_name.c_str( ) );
		m_name = name;
	}

	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( )
	{
		R_ASSERT( !m_is_registered );
		m_is_registered = true;
	}

	res_texture_ptr get_texture( ) const { return m_texture; }

private:
	shared_string m_name;
	ID3D11Texture2D* m_surface;
	ID3D11Texture3D* m_surface_3d;
	ID3D11RenderTargetView* m_rt;
	ID3D11DepthStencilView* m_zrt;
	res_texture_ptr m_texture;
	u32 m_width;
	u32 m_height;
	DXGI_FORMAT m_format;
	enum_rt_usage m_usage;
	u32 m_memory_usage;
	u64 m_order;
	bool m_is_registered;
};

STATIC_SIZE_ASSERT( render_target, 0x40 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RENDER_TARGET_H_INCLUDED
