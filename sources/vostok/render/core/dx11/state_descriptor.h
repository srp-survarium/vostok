// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_STATE_DESCRIPTOR_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_STATE_DESCRIPTOR_H_INCLUDED
#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {

class resource_manager;

class state_descriptor {
	friend class resource_manager;

public:
	state_descriptor( );

	void reset( );
	void set_depth( bool enable, bool write_enable, D3D11_COMPARISON_FUNC cmp_func );
	void set_stencil( s32 enable, u32 ref, u8 read_mask, u8 write_mask );
	void set_stencil(
		s32 enable,
		u32 ref,
		u8 read_mask,
		u8 write_mask,
		D3D11_COMPARISON_FUNC func,
		D3D11_STENCIL_OP fail,
		D3D11_STENCIL_OP pass,
		D3D11_STENCIL_OP zfail
	)
	{
		set_stencil( enable, ref, read_mask, write_mask );
		set_stencil_frontface( func, fail, pass, zfail );
		set_stencil_backface( func, fail, pass, zfail );
	}
	void set_stencil_frontface(
		D3D11_COMPARISON_FUNC func,
		D3D11_STENCIL_OP fail,
		D3D11_STENCIL_OP pass,
		D3D11_STENCIL_OP zfail
	);
	void set_stencil_backface(
		D3D11_COMPARISON_FUNC func,
		D3D11_STENCIL_OP fail,
		D3D11_STENCIL_OP pass,
		D3D11_STENCIL_OP zfail
	);
	void set_stencil_ref( u32 ref ) { m_stencil_ref = ref; }
	void set_fill_mode( D3D11_FILL_MODE fill_mode );
	void set_alpha_blend(
		s32 blend_enable,
		D3D11_BLEND src_blend,
		D3D11_BLEND dest_blend,
		D3D11_BLEND_OP blend_op,
		D3D11_BLEND src_alpha_blend,
		D3D11_BLEND dest_alpha_blend,
		D3D11_BLEND_OP blend_alpha_op
	);
	void set_alpha_to_coverage( s32 enabled )
	{
		m_effect_desc.AlphaToCoverageEnable = enabled;
		m_effect_desc_updated = true;
	}
	void color_write_enable( D3D11_COLOR_WRITE_ENABLE mode );
	void set_cull_mode( D3D11_CULL_MODE mode );

private:
	D3D11_RASTERIZER_DESC		m_rasterizer_desc;
	D3D11_DEPTH_STENCIL_DESC	m_depth_stencil_desc;
	D3D11_BLEND_DESC			m_effect_desc;
	u32							m_stencil_ref;
	bool						m_rasterizer_desc_updated;
	bool						m_depth_stencil_desc_updated;
	bool						m_effect_desc_updated;
};

STATIC_SIZE_ASSERT( state_descriptor, 0x16C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_STATE_DESCRIPTOR_H_INCLUDED
