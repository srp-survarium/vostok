////////////////////////////////////////////////////////////////////////////
//	Created		: 05.04.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/render/core/state_utils.h>
#include <boost/crc.hpp>

namespace vostok{
namespace render {
namespace state_utils{

// REMAINDER: reset(D3D_SAMPLER_DESC&) (canonical moved sampler defaults into
// sampler_state_descriptor.h) + check_validity family (no canonical home).




inline void process( processor & proc, bool value)
{
	proc.process_byte( value);
}

inline void process( processor & proc, u32 value)
{
	proc.process_byte( (u8)((value) & 0xff));
	proc.process_byte( (u8)((value>>8) & 0xff));
	proc.process_byte( (u8)((value>>16) & 0xff));
	proc.process_byte( (u8)((value>>24) & 0xff));
}

inline void process( processor & proc, int value)
{
	process( proc, (u32)value);
}


inline void process( processor & proc, float value)
{
	process( proc, horrible_cast<float,u32>(value).second);
}

} //namespace crc






void reset( D3D_SAMPLER_DESC &desc )
{
	ZeroMemory(&desc, sizeof(desc));

	desc.Filter = D3D_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D_COMPARISON_NEVER;
	desc.BorderColor[0] = 1.0f;
	desc.BorderColor[1] = 1.0f;
	desc.BorderColor[2] = 1.0f;
	desc.BorderColor[3] = 1.0f;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;
}











void check_validity(D3D_RASTERIZER_DESC &desc)
{
	UNREFERENCED_PARAMETER (desc);
}

void check_validity(D3D_DEPTH_STENCIL_DESC &desc)
{
	ASSERT( (desc.DepthEnable==0) || (desc.DepthEnable==1));
	ASSERT( (desc.StencilEnable==0) || (desc.StencilEnable==1));

	if (!desc.DepthEnable)
	{
		desc.DepthFunc = D3D_COMPARISON_NEVER;
	}

	if (!desc.StencilEnable)
	{
		desc.StencilReadMask = 0xFF;
		desc.StencilWriteMask = 0xFF;

		desc.FrontFace.StencilFailOp = D3D_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D_STENCIL_OP_KEEP;
		desc.FrontFace.StencilPassOp = D3D_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D_COMPARISON_ALWAYS;

		desc.BackFace.StencilFailOp = D3D_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D_STENCIL_OP_KEEP;
		desc.BackFace.StencilPassOp = D3D_STENCIL_OP_KEEP;
		desc.BackFace.StencilFunc = D3D_COMPARISON_ALWAYS;
	}
}

#if USE_DX10
void check_validity(D3D_BLEND_DESC &desc)
{
	BOOL	bBlendEnable = FALSE;

	for ( int i=0; i<8; ++i)
	{
		ASSERT( (desc.BlendEnable[i]==0) || (desc.BlendEnable[i]==1));
		bBlendEnable |= desc.BlendEnable[i];
	}

	if (!bBlendEnable)
	{
		desc.SrcBlend = D3D_BLEND_ONE;
		desc.DestBlend = D3D_BLEND_ZERO;
		desc.BlendOp = D3D_BLEND_OP_ADD;
		desc.SrcBlendAlpha = D3D_BLEND_ONE;
		desc.DestBlendAlpha = D3D_BLEND_ZERO;
		desc.BlendOpAlpha = D3D_BLEND_OP_ADD;
	}
	else
	{
		switch(desc.SrcBlendAlpha)
		{
		case D3D_BLEND_SRC_COLOR:
			desc.SrcBlendAlpha = D3D_BLEND_SRC_ALPHA;
			break;
		case D3D_BLEND_INV_SRC_COLOR:
			desc.SrcBlendAlpha = D3D_BLEND_INV_SRC_ALPHA;
			break;
		case D3D_BLEND_DEST_COLOR:
			desc.SrcBlendAlpha = D3D_BLEND_DEST_ALPHA;
			break;
		case D3D_BLEND_INV_DEST_COLOR:
			desc.SrcBlendAlpha = D3D_BLEND_INV_DEST_ALPHA;
			break;
		case D3D_BLEND_SRC1_COLOR:
			desc.SrcBlendAlpha = D3D_BLEND_SRC1_ALPHA;
			break;
		case D3D_BLEND_INV_SRC1_COLOR:
			desc.SrcBlendAlpha = D3D_BLEND_INV_SRC1_ALPHA;
			break;
		}

		switch(desc.DestBlendAlpha)
		{
		case D3D_BLEND_SRC_COLOR:
			desc.DestBlendAlpha = D3D_BLEND_SRC_ALPHA;
			break;
		case D3D_BLEND_INV_SRC_COLOR:
			desc.DestBlendAlpha = D3D_BLEND_INV_SRC_ALPHA;
			break;
		case D3D_BLEND_DEST_COLOR:
			desc.DestBlendAlpha = D3D_BLEND_DEST_ALPHA;
			break;
		case D3D_BLEND_INV_DEST_COLOR:
			desc.DestBlendAlpha = D3D_BLEND_INV_DEST_ALPHA;
			break;
		case D3D_BLEND_SRC1_COLOR:
			desc.DestBlendAlpha = D3D_BLEND_SRC1_ALPHA;
			break;
		case D3D_BLEND_INV_SRC1_COLOR:
			desc.DestBlendAlpha = D3D_BLEND_INV_SRC1_ALPHA;
			break;
		}
	}

}

#else

void check_validity(D3D_BLEND_DESC &desc)
{
	BOOL	bBlendEnable = FALSE;

	for ( int i=0; i<8; ++i)
	{
		ASSERT( (desc.RenderTarget[i].BlendEnable==0) || (desc.RenderTarget[i].BlendEnable==1));
		bBlendEnable |= desc.RenderTarget[i].BlendEnable;
	}

	for ( int i=0; i<8; ++i)
	{
		if (!bBlendEnable)
		{
			desc.RenderTarget[i].SrcBlend = D3D_BLEND_ONE;
			desc.RenderTarget[i].DestBlend = D3D_BLEND_ZERO;
			desc.RenderTarget[i].BlendOp = D3D_BLEND_OP_ADD;
			desc.RenderTarget[i].SrcBlendAlpha = D3D_BLEND_ONE;
			desc.RenderTarget[i].DestBlendAlpha = D3D_BLEND_ZERO;
			desc.RenderTarget[i].BlendOpAlpha = D3D_BLEND_OP_ADD;
		}
		else
		{
			switch(desc.RenderTarget[i].SrcBlendAlpha)
			{
			case D3D_BLEND_SRC_COLOR:
				desc.RenderTarget[i].SrcBlendAlpha = D3D_BLEND_SRC_ALPHA;
				break;
			case D3D_BLEND_INV_SRC_COLOR:
				desc.RenderTarget[i].SrcBlendAlpha = D3D_BLEND_INV_SRC_ALPHA;
				break;
			case D3D_BLEND_DEST_COLOR:
				desc.RenderTarget[i].SrcBlendAlpha = D3D_BLEND_DEST_ALPHA;
				break;
			case D3D_BLEND_INV_DEST_COLOR:
				desc.RenderTarget[i].SrcBlendAlpha = D3D_BLEND_INV_DEST_ALPHA;
				break;
			case D3D_BLEND_SRC1_COLOR:
				desc.RenderTarget[i].SrcBlendAlpha = D3D_BLEND_SRC1_ALPHA;
				break;
			case D3D_BLEND_INV_SRC1_COLOR:
				desc.RenderTarget[i].SrcBlendAlpha = D3D_BLEND_INV_SRC1_ALPHA;
				break;
			}

			switch(desc.RenderTarget[i].DestBlendAlpha)
			{
			case D3D_BLEND_SRC_COLOR:
				desc.RenderTarget[i].DestBlendAlpha = D3D_BLEND_SRC_ALPHA;
				break;
			case D3D_BLEND_INV_SRC_COLOR:
				desc.RenderTarget[i].DestBlendAlpha = D3D_BLEND_INV_SRC_ALPHA;
				break;
			case D3D_BLEND_DEST_COLOR:
				desc.RenderTarget[i].DestBlendAlpha = D3D_BLEND_DEST_ALPHA;
				break;
			case D3D_BLEND_INV_DEST_COLOR:
				desc.RenderTarget[i].DestBlendAlpha = D3D_BLEND_INV_DEST_ALPHA;
				break;
			case D3D_BLEND_SRC1_COLOR:
				desc.RenderTarget[i].DestBlendAlpha = D3D_BLEND_SRC1_ALPHA;
				break;
			case D3D_BLEND_INV_SRC1_COLOR:
				desc.RenderTarget[i].DestBlendAlpha = D3D_BLEND_INV_SRC1_ALPHA;
				break;
			}
		}
	}
}

#endif

void check_validity(D3D_SAMPLER_DESC &desc)
{
	if (	(desc.AddressU != D3D_TEXTURE_ADDRESS_BORDER)
		 &&	(desc.AddressV != D3D_TEXTURE_ADDRESS_BORDER)
		 &&	(desc.AddressW != D3D_TEXTURE_ADDRESS_BORDER))
	{
		for (int i=0; i<4; ++i)
		{
			desc.BorderColor[i] = 0.0f;
		}
	}

	if (	(desc.Filter != D3D_FILTER_ANISOTROPIC)
		 && (desc.Filter != D3D_FILTER_COMPARISON_ANISOTROPIC))
	{
		desc.MaxAnisotropy = 1;
	}
}

} // namespace state_utils

} // namespace render
} // namespace vostok
