// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_RES_RENDER_OUTPUT_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_RENDER_OUTPUT_H_INCLUDED
#include <vostok/fixed_string.h>
#include <vostok/intrusive_ptr.h>
#include <vostok/math_uint2.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/resource_intrusive_base.h>

namespace vostok {
namespace render {

class res_texture;

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

class res_render_output : public resource_intrusive_base {
public:
	res_render_output( HWND window, bool windowed );
	~res_render_output( );

	void destroy_impl( ) const;

	void present( );
	void resize( bool force_resize );
	void resize( bool windowed, const u32 size_x, const u32 size_y, bool force_resize );
	void set_size( const u32 in_width, const u32 in_height, const bool in_fullscreen, bool force_resize );

	math::uint2 size( ) const
	{
		return math::uint2(
			m_swap_chain_desc.BufferDesc.Width,
			m_swap_chain_desc.BufferDesc.Height
		);
	}

	u32 width( ) const { return m_swap_chain_desc.BufferDesc.Width; }
	u32 height( ) const { return m_swap_chain_desc.BufferDesc.Height; }
	HWND window( ) const { return m_window; }
	bool equal( HWND window ) const { return m_window == window; }
	bool valid_present( ) const { return m_valid_previous_present; }
	void goto_fullscreen( );

	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( )
	{
		R_ASSERT( !m_is_registered );
		m_is_registered = true;
	}

private:
	void initialize_swap_chain( IDXGISwapChain* swap_chain );
	void select_resolution( u32& width, u32& height, bool windowed, HWND window ) const;
	DXGI_RATIONAL select_refresh( u32 width, u32 height, DXGI_FORMAT format ) const;
	void update_targets( );
	void update_depth_stencil_buffer( );
	void update_window_properties( );
	u32 select_presentation_interval( );

public:
	fixed_string<128>			m_depth_rexture_name;
	DXGI_SWAP_CHAIN_DESC		m_swap_chain_desc;
	IDXGISwapChain*				m_swap_chain;
	ID3D11RenderTargetView*	m_base_rt;
	ID3D11DepthStencilView*	m_base_zb;
	res_texture_ptr				m_texture_zb;
	HWND						m_window;
	u32							m_present_sync_mode;
	bool						m_valid_previous_present;
	bool						m_windowed;
	bool						m_is_registered;
};

typedef intrusive_ptr<
	res_render_output,
	resource_intrusive_base,
	threading::single_threading_policy
> res_render_output_ptr;

typedef intrusive_ptr<
	res_render_output const,
	resource_intrusive_base const,
	threading::single_threading_policy
> res_render_output_const_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_RENDER_OUTPUT_H_INCLUDED
