#include "pch.h"
#include <vostok/render/core/state_cache.h>

namespace vostok {
namespace render {

template < typename device_state, typename state_desc >
state_cache<device_state, state_desc>::state_cache( )
{
	// STATE[STUB]
}

template < typename device_state, typename state_desc >
state_cache<device_state, state_desc>::~state_cache( )
{
	// STATE[STUB]
}

template < typename device_state, typename state_desc >
void state_cache<device_state, state_desc>::clear_state_array( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12dbd0] for rasterizer state
}

template <>
void state_cache<ID3D11RasterizerState, D3D11_RASTERIZER_DESC>::create_state(
	D3D11_RASTERIZER_DESC,
	ID3D11RasterizerState**
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x797a30]
}

template <>
void state_cache<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC>::create_state(
	D3D11_DEPTH_STENCIL_DESC,
	ID3D11DepthStencilState**
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7979a0]
}

template <>
void state_cache<ID3D11BlendState, D3D11_BLEND_DESC>::create_state(
	D3D11_BLEND_DESC,
	ID3D11BlendState**
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x797910]
}

template <>
void state_cache<ID3D11SamplerState, D3D11_SAMPLER_DESC>::create_state(
	D3D11_SAMPLER_DESC,
	ID3D11SamplerState**
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x797880]
}

template class state_cache<ID3D11RasterizerState, D3D11_RASTERIZER_DESC>;
template class state_cache<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC>;
template class state_cache<ID3D11BlendState, D3D11_BLEND_DESC>;
template class state_cache<ID3D11SamplerState, D3D11_SAMPLER_DESC>;

} // namespace render
} // namespace vostok
