// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/core/dx11/state_descriptor.h>
#include <vostok/render/core/state_utils.h>

namespace vostok {
namespace render {

state_descriptor::state_descriptor( )
{
	reset	( );
}

void state_descriptor::reset( )
{
	m_rasterizer_desc_updated		= false;
	m_depth_stencil_desc_updated	= false;
	m_effect_desc_updated			= false;

	state_utils::reset( m_rasterizer_desc);
	state_utils::reset( m_depth_stencil_desc);
	state_utils::reset( m_effect_desc);

	m_stencil_ref		 = 0x00;
}

void state_descriptor::set_depth( bool enable, bool write_enable, D3D11_COMPARISON_FUNC cmp_func )
{
	m_depth_stencil_desc.DepthEnable	= enable;
	m_depth_stencil_desc.DepthWriteMask	= write_enable ? D3D_DEPTH_WRITE_MASK_ALL : D3D_DEPTH_WRITE_MASK_ZERO;
	m_depth_stencil_desc.DepthFunc		= cmp_func;

	m_depth_stencil_desc_updated		= true;
}

void state_descriptor::set_stencil( s32 enable, u32 ref, u8 read_mask, u8 write_mask )
{
	m_depth_stencil_desc.StencilEnable		= enable;
	m_depth_stencil_desc.StencilReadMask	= read_mask;
	m_depth_stencil_desc.StencilWriteMask	= write_mask;
	m_stencil_ref							= ref;

	m_depth_stencil_desc_updated		= true;
}

void state_descriptor::set_stencil_frontface(
	D3D11_COMPARISON_FUNC func,
	D3D11_STENCIL_OP fail,
	D3D11_STENCIL_OP pass,
	D3D11_STENCIL_OP zfail
)
{
	m_depth_stencil_desc.FrontFace.StencilFailOp 		= fail;
	m_depth_stencil_desc.FrontFace.StencilDepthFailOp	= zfail;
	m_depth_stencil_desc.FrontFace.StencilPassOp		= pass;
	m_depth_stencil_desc.FrontFace.StencilFunc			= func;

	m_depth_stencil_desc_updated		= true;
}

void state_descriptor::set_stencil_backface(
	D3D11_COMPARISON_FUNC func,
	D3D11_STENCIL_OP fail,
	D3D11_STENCIL_OP pass,
	D3D11_STENCIL_OP zfail
)
{
	m_depth_stencil_desc.BackFace.StencilFailOp 		= fail;
	m_depth_stencil_desc.BackFace.StencilDepthFailOp	= zfail;
	m_depth_stencil_desc.BackFace.StencilPassOp			= pass;
	m_depth_stencil_desc.BackFace.StencilFunc			= func;

	m_depth_stencil_desc_updated		= true;
}

void state_descriptor::set_alpha_blend(
	s32 blend_enable,
	D3D11_BLEND src_blend,
	D3D11_BLEND dest_blend,
	D3D11_BLEND_OP blend_op,
	D3D11_BLEND src_alpha_blend,
	D3D11_BLEND dest_alpha_blend,
	D3D11_BLEND_OP blend_alpha_op
)
{
	// Temporary !!!
	for( int i = 0; i< 8; ++i)
	{
		m_effect_desc.RenderTarget[i].BlendEnable		=  blend_enable;
		m_effect_desc.RenderTarget[i].SrcBlend			= src_blend;
		m_effect_desc.RenderTarget[i].DestBlend			= dest_blend;
		m_effect_desc.RenderTarget[i].BlendOp			= blend_op;
		m_effect_desc.RenderTarget[i].SrcBlendAlpha		= src_alpha_blend;
		m_effect_desc.RenderTarget[i].DestBlendAlpha	= dest_alpha_blend;
		m_effect_desc.RenderTarget[i].BlendOpAlpha		= blend_alpha_op;
	}
}

void state_descriptor::set_cull_mode( D3D11_CULL_MODE mode )
{
	m_rasterizer_desc_updated |= m_rasterizer_desc.CullMode != mode;
	m_rasterizer_desc.CullMode = mode;
}

void state_descriptor::color_write_enable( D3D11_COLOR_WRITE_ENABLE mode )
{
	m_effect_desc_updated |= m_effect_desc.RenderTarget[0].RenderTargetWriteMask != mode;

	for( int i = 0; i< 8; ++i)
		m_effect_desc.RenderTarget[i].RenderTargetWriteMask = (u8)mode;
}

void state_descriptor::set_fill_mode( D3D11_FILL_MODE fill_mode )
{
	m_rasterizer_desc_updated		|= m_rasterizer_desc.FillMode != fill_mode;
	m_rasterizer_desc.FillMode		 = fill_mode;
}

} // namespace render
} // namespace vostok
