#include "pch.h"
#include <vostok/render/core/state_utils.h>

namespace vostok {
namespace render {
namespace state_utils {

namespace crc {

static inline void process( processor & proc, u8 value)
{
	proc.process_byte( value);
}

static inline void process( processor & proc, bool value)
{
	proc.process_byte( value);
}

void process( processor& proc, u32 value )
{
	// FUNCTION BODY[0x12f920]
	proc.process_byte( (u8)((value) & 0xff));
	proc.process_byte( (u8)((value>>8) & 0xff));
	proc.process_byte( (u8)((value>>16) & 0xff));
	proc.process_byte( (u8)((value>>24) & 0xff));
}

static inline void process( processor & proc, int value)
{
	process( proc, (u32)value);
}

static inline void process( processor & proc, float value)
{
	process( proc, horrible_cast<float,u32>(value).second);
}

} //namespace crc

void reset( D3D11_RASTERIZER_DESC& desc )
{
	// FUNCTION BODY[0x558290]

	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode				= D3D_FILL_SOLID;
	desc.CullMode				= D3D_CULL_BACK;
	desc.FrontCounterClockwise	= FALSE;
	desc.DepthBias				= 0;
	desc.DepthBiasClamp			= 0.0f;
	desc.SlopeScaledDepthBias	= 0.0f;
	desc.DepthClipEnable		= TRUE;
	desc.ScissorEnable			= FALSE;
	// --Porting to DX10_
	// 	if( RImplementation.o.dx10_msaa )
	// 		desc.MultisampleEnable	= TRUE;
	// 	else
	desc.MultisampleEnable	= FALSE;
	desc.AntialiasedLineEnable	= FALSE;
}

void reset( D3D11_DEPTH_STENCIL_DESC& desc )
{
	// FUNCTION BODY[0x558240]

	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable		= TRUE;
	desc.DepthWriteMask		= D3D_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc			= D3D_COMPARISON_LESS;
	desc.StencilEnable		= TRUE;

	// --Porting to DX10_
	// 	if( !RImplementation.o.dx10_msaa )
	// 	{
	// 		desc.StencilReadMask = 0xFF;
	// 		desc.StencilWriteMask = 0xFF;
	// 	}
	// 	else
	{
		desc.StencilReadMask = 0x7F;
		desc.StencilWriteMask = 0x7F;
	}

	desc.FrontFace.StencilFailOp		= D3D_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp	= D3D_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp		= D3D_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc			= D3D_COMPARISON_ALWAYS;

	desc.BackFace.StencilFailOp			= D3D_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp	= D3D_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp			= D3D_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc			= D3D_COMPARISON_ALWAYS;
}

void reset( D3D11_BLEND_DESC& desc )
{
	// FUNCTION BODY[0x5581e0]
	ZeroMemory(&desc, sizeof(desc));

	desc.AlphaToCoverageEnable	= FALSE;
	desc.IndependentBlendEnable = FALSE;

	for ( int i=0; i<8; ++i)
	{
		desc.RenderTarget[i].BlendEnable		= FALSE;
		desc.RenderTarget[i].SrcBlend			= D3D_BLEND_ONE;
		desc.RenderTarget[i].DestBlend			= D3D_BLEND_ZERO;
		desc.RenderTarget[i].BlendOp			= D3D_BLEND_OP_ADD;
		desc.RenderTarget[i].SrcBlendAlpha		= D3D_BLEND_ONE;
		desc.RenderTarget[i].DestBlendAlpha		= D3D_BLEND_ZERO;
		desc.RenderTarget[i].BlendOpAlpha		= D3D_BLEND_OP_ADD;
		desc.RenderTarget[i].RenderTargetWriteMask	= D3D_COLOR_WRITE_ENABLE_ALL;
	}
}

bool operator==(
	D3D11_RASTERIZER_DESC const& desc1,
	D3D11_RASTERIZER_DESC const& desc2
)
{
	// FUNCTION BODY[0x558170]

	if (desc1.FillMode != desc2.FillMode) return false;
	if (desc1.CullMode != desc2.CullMode) return false;
	if (desc1.FrontCounterClockwise != desc2.FrontCounterClockwise) return false;
	if (desc1.DepthBias != desc2.DepthBias) return false;
	if (desc1.DepthBiasClamp != desc2.DepthBiasClamp) return false;
	if (desc1.SlopeScaledDepthBias != desc2.SlopeScaledDepthBias) return false;
	if (desc1.DepthClipEnable != desc2.DepthClipEnable) return false;
	if (desc1.ScissorEnable != desc2.ScissorEnable) return false;
	if (desc1.MultisampleEnable != desc2.MultisampleEnable) return false;
	if (desc1.AntialiasedLineEnable != desc2.AntialiasedLineEnable) return false;

	return true;
}

bool operator==(
	D3D11_DEPTH_STENCIL_DESC const& desc1,
	D3D11_DEPTH_STENCIL_DESC const& desc2
)
{
	// FUNCTION BODY[0x5580f0]

	if ( desc1.DepthEnable != desc2.DepthEnable) return false;
	if ( desc1.DepthWriteMask != desc2.DepthWriteMask) return false;
	if ( desc1.DepthFunc != desc2.DepthFunc) return false;
	if ( desc1.StencilEnable != desc2.StencilEnable) return false;
	if ( desc1.StencilReadMask != desc2.StencilReadMask) return false;
	if ( desc1.StencilWriteMask != desc2.StencilWriteMask) return false;

	if ( desc1.FrontFace.StencilFailOp != desc2.FrontFace.StencilFailOp) return false;
	if ( desc1.FrontFace.StencilDepthFailOp != desc2.FrontFace.StencilDepthFailOp) return false;
	if ( desc1.FrontFace.StencilPassOp != desc2.FrontFace.StencilPassOp) return false;
	if ( desc1.FrontFace.StencilFunc != desc2.FrontFace.StencilFunc) return false;

	if ( desc1.BackFace.StencilFailOp != desc2.BackFace.StencilFailOp) return false;
	if ( desc1.BackFace.StencilDepthFailOp != desc2.BackFace.StencilDepthFailOp) return false;
	if ( desc1.BackFace.StencilPassOp != desc2.BackFace.StencilPassOp) return false;
	if ( desc1.BackFace.StencilFunc != desc2.BackFace.StencilFunc) return false;
	
	return true;
}

// claude@NOTE: legacy carried a 4-iteration USE_DX11 "quick fix" variant;
// USE_DX11 is not defined in this tree, so the shipped arm is the
// 8-iteration RenderTarget form - verify the loop bound against 0x558070.
bool operator==(
	D3D11_BLEND_DESC const& desc1,
	D3D11_BLEND_DESC const& desc2
)
{
	// FUNCTION BODY[0x558070]
	if ( desc1.AlphaToCoverageEnable != desc2.AlphaToCoverageEnable) return false;
	if ( desc1.IndependentBlendEnable != desc2.IndependentBlendEnable) return false;

	for ( int i=0; i<8; ++i)
	{
		if( desc1.RenderTarget[i].BlendEnable	!= desc2.RenderTarget[i].BlendEnable) return false;
		if( desc1.RenderTarget[i].SrcBlend		!= desc2.RenderTarget[i].SrcBlend) return false;
		if( desc1.RenderTarget[i].DestBlend		!= desc2.RenderTarget[i].DestBlend) return false;
		if( desc1.RenderTarget[i].BlendOp		!= desc2.RenderTarget[i].BlendOp) return false;
		if( desc1.RenderTarget[i].SrcBlendAlpha	!= desc2.RenderTarget[i].SrcBlendAlpha) return false;
		if( desc1.RenderTarget[i].DestBlendAlpha != desc2.RenderTarget[i].DestBlendAlpha) return false;
		if( desc1.RenderTarget[i].BlendOpAlpha	 != desc2.RenderTarget[i].BlendOpAlpha) return false;
		if( desc1.RenderTarget[i].RenderTargetWriteMask != desc2.RenderTarget[i].RenderTargetWriteMask) return false;
	}

	return true;
}

bool operator==(
	D3D11_SAMPLER_DESC const& desc1,
	D3D11_SAMPLER_DESC const& desc2
)
{
	// FUNCTION BODY[0x557fd0]

	if( desc1.Filter != desc2.Filter) return false;
	if( desc1.AddressU != desc2.AddressU) return false;
	if( desc1.AddressV != desc2.AddressV) return false;
	if( desc1.AddressW != desc2.AddressW) return false;
	if( desc1.MipLODBias != desc2.MipLODBias) return false;
//	Ignore anisotropy since it's set up automatically by the manager
//	if( desc1.MaxAnisotropy != desc2.MaxAnisotropy) return false;
	if( desc1.ComparisonFunc != desc2.ComparisonFunc) return false;
	if( desc1.BorderColor[0] != desc2.BorderColor[0]) return false;
	if( desc1.BorderColor[1] != desc2.BorderColor[1]) return false;
	if( desc1.BorderColor[2] != desc2.BorderColor[2]) return false;
	if( desc1.BorderColor[3] != desc2.BorderColor[3]) return false;
	if( desc1.MinLOD != desc2.MinLOD) return false;
	if( desc1.MaxLOD != desc2.MaxLOD) return false;

	return true;
}

u32 get_hash( D3D11_RASTERIZER_DESC const& desc )
{
	// FUNCTION BODY[0x5587a0]

	crc::processor	crc_hash;

	crc::process( crc_hash, desc.FillMode);
	crc::process( crc_hash, desc.CullMode);
	crc::process( crc_hash, desc.FrontCounterClockwise);
	crc::process( crc_hash, desc.DepthBias);
	crc::process( crc_hash, desc.DepthBiasClamp);
	crc::process( crc_hash, desc.SlopeScaledDepthBias);
	crc::process( crc_hash, desc.DepthClipEnable);
	crc::process( crc_hash, desc.ScissorEnable);
	crc::process( crc_hash, desc.MultisampleEnable);
	crc::process( crc_hash, desc.AntialiasedLineEnable);

// 	crc::process( crc_hash, desc.FillMode, sizeof(desc.FillMode) );
// 	crc::process( crc_hash, desc.CullMode, sizeof(desc.CullMode) );
// 	crc::process( crc_hash, desc.FrontCounterClockwise, sizeof(desc.FrontCounterClockwise) );
// 	crc::process( crc_hash, desc.DepthBias, sizeof(desc.DepthBias) );
// 	crc::process( crc_hash, desc.DepthBiasClamp, sizeof(desc.DepthBiasClamp) );
// 	crc::process( crc_hash, desc.SlopeScaledDepthBias, sizeof(desc.SlopeScaledDepthBias) );
// 	crc::process( crc_hash, desc.DepthClipEnable, sizeof(desc.DepthClipEnable) );
// 	crc::process( crc_hash, desc.ScissorEnable, sizeof(desc.ScissorEnable) );
// 	crc::process( crc_hash, desc.MultisampleEnable, sizeof(desc.MultisampleEnable) );
// 	crc::process( crc_hash, desc.AntialiasedLineEnable, sizeof(desc.AntialiasedLineEnable) );
	
	return crc_hash.checksum();
}

u32 get_hash( D3D11_DEPTH_STENCIL_DESC const& desc )
{
	// FUNCTION BODY[0x5586e0]

	crc::processor crc_hash;

	crc::process( crc_hash, desc.DepthEnable);
	crc::process( crc_hash, desc.DepthWriteMask);
	crc::process( crc_hash, desc.DepthFunc);
	crc::process( crc_hash, desc.StencilEnable);
	crc::process( crc_hash, desc.StencilReadMask);
	crc::process( crc_hash, desc.StencilWriteMask);

	crc::process( crc_hash, desc.FrontFace.StencilFailOp);
	crc::process( crc_hash, desc.FrontFace.StencilDepthFailOp);
	crc::process( crc_hash, desc.FrontFace.StencilPassOp);
	crc::process( crc_hash, desc.FrontFace.StencilFunc);

	crc::process( crc_hash, desc.BackFace.StencilFailOp);
	crc::process( crc_hash, desc.BackFace.StencilDepthFailOp);
	crc::process( crc_hash, desc.BackFace.StencilPassOp);
	crc::process( crc_hash, desc.BackFace.StencilFunc);

	return crc_hash.checksum();
}

u32 get_hash( D3D11_BLEND_DESC const& desc )
{
	// FUNCTION BODY[0x5583d0]
	crc::processor	crc_hash;

	crc::process( crc_hash, desc.AlphaToCoverageEnable);
	crc::process( crc_hash, desc.IndependentBlendEnable);

	for ( int i=0; i<8; ++i)
	{
		crc::process( crc_hash, desc.RenderTarget[i].BlendEnable);
		crc::process( crc_hash, desc.RenderTarget[i].SrcBlend);
		crc::process( crc_hash, desc.RenderTarget[i].DestBlend);
		crc::process( crc_hash, desc.RenderTarget[i].BlendOp);
		crc::process( crc_hash, desc.RenderTarget[i].SrcBlendAlpha);
		crc::process( crc_hash, desc.RenderTarget[i].DestBlendAlpha);
		crc::process( crc_hash, desc.RenderTarget[i].BlendOpAlpha);
		crc::process( crc_hash, desc.RenderTarget[i].RenderTargetWriteMask);
	}

	return crc_hash.checksum();
}

u32 get_hash( D3D11_SAMPLER_DESC const& desc )
{
	// FUNCTION BODY[0x5582e0]

	crc::processor	crc_hash;

	crc::process( crc_hash, desc.Filter);
	crc::process( crc_hash, desc.AddressU);
	crc::process( crc_hash, desc.AddressV);
	crc::process( crc_hash, desc.AddressW);
	crc::process( crc_hash, desc.MipLODBias);
//	Ignore anisotropy since it's set up automatically by the manager
//	crc::process( crc_hash, desc.MaxAnisotropy, sizeof(desc.MaxAnisotropy) );
	crc::process( crc_hash, desc.ComparisonFunc);
	crc::process( crc_hash, desc.BorderColor[0]);
	crc::process( crc_hash, desc.BorderColor[1]);
	crc::process( crc_hash, desc.BorderColor[2]);
	crc::process( crc_hash, desc.BorderColor[3]);
	crc::process( crc_hash, desc.MinLOD);
	crc::process( crc_hash, desc.MaxLOD);

	return crc_hash.checksum();
}

} // namespace state_utils
} // namespace render
} // namespace vostok
