////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\state_cache.h"

namespace vostok {
namespace render {

// STATE[STUB]
void state_cache<ID3D11RasterizerState,D3D11_RASTERIZER_DESC>::clear_state_array( )
{
	// CALL SITE INFO
	// <0x12dbf4> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x12dbd0]: 4
	// <0x12dbd0>|0x000|+0x010:'38'
	// <0x12dbe0>|0x010|+0x029:'39'
	// <0>
	// <0x12dc09>|0x039|+0x021:'41'
	// ******
}

// STATE[STUB]
void state_cache<ID3D11RasterizerState,D3D11_RASTERIZER_DESC>::create_state( D3D11_RASTERIZER_DESC desc, ID3D11RasterizerState** ppIState )
{
	// LOCALS
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266e4>;
	// ******

	// CALL SITE INFO
	// <0x797a51> -> HRESULT < unknown >( D3D11_RASTERIZER_DESC const*, ID3D11RasterizerState** )
	// <0x797a80> -> HRESULT < unknown >( D3D11_RASTERIZER_DESC const*, ID3D11RasterizerState** )
	// ******

	// FUNCTION BODY[0x797a30]: 1
	// <0x797a31>|0x001|+0x082:'48'
	// ******
}

// STATE[STUB]
void state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>::create_state( D3D11_DEPTH_STENCIL_DESC desc, ID3D11DepthStencilState** ppIState )
{
	// LOCALS
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266e5>;
	// ******

	// CALL SITE INFO
	// <0x7979c1> -> HRESULT < unknown >( D3D11_DEPTH_STENCIL_DESC const*, ID3D11DepthStencilState** )
	// <0x7979f0> -> HRESULT < unknown >( D3D11_DEPTH_STENCIL_DESC const*, ID3D11DepthStencilState** )
	// ******

	// FUNCTION BODY[0x7979a0]: 1
	// <0x7979a1>|0x001|+0x082:'55'
	// ******
}

// STATE[STUB]
void state_cache<ID3D11BlendState,D3D11_BLEND_DESC>::create_state( D3D11_BLEND_DESC desc, ID3D11BlendState** ppIState )
{
	// LOCALS
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266e6>;
	// ******

	// CALL SITE INFO
	// <0x797931> -> HRESULT < unknown >( D3D11_BLEND_DESC const*, ID3D11BlendState** )
	// <0x797960> -> HRESULT < unknown >( D3D11_BLEND_DESC const*, ID3D11BlendState** )
	// ******

	// FUNCTION BODY[0x797910]: 1
	// <0x797911>|0x001|+0x082:'62'
	// ******
}

// STATE[STUB]
void state_cache<ID3D11SamplerState,D3D11_SAMPLER_DESC>::create_state( D3D11_SAMPLER_DESC desc, ID3D11SamplerState** ppIState )
{
	// LOCALS
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266e7>;
	// ******

	// CALL SITE INFO
	// <0x7978a1> -> HRESULT < unknown >( D3D11_SAMPLER_DESC const*, ID3D11SamplerState** )
	// <0x7978d0> -> HRESULT < unknown >( D3D11_SAMPLER_DESC const*, ID3D11SamplerState** )
	// ******

	// FUNCTION BODY[0x797880]: 1
	// <0x797881>|0x001|+0x082:'69'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	vostok::render::state_cache< ID3D11BlendState, D3D11_BLEND_DESC >::state_record*
	// 	iterator_type;

	// typedef
	// 	vostok::render::state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record*
	// 	iterator_type;

	// typedef
	// 	vostok::render::state_cache< ID3D11RasterizerState, D3D11_RASTERIZER_DESC >::state_record*
	// 	iterator_type;

	// typedef
	// 	vostok::render::state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
