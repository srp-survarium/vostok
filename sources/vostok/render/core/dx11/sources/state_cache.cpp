#include "pch.h"
#include <vostok/render/core/state_cache.h>
#include <vostok/render/core/device.h>
#include "com_utils.h"

namespace vostok {
namespace render {

template < typename device_state, typename state_desc >
state_cache<device_state, state_desc>::state_cache( )
{
	static const int iMasRSStates = 10;
	states.reserve( iMasRSStates);
}

template < typename device_state, typename state_desc >
state_cache<device_state, state_desc>::~state_cache( )
{
	clear_state_array();
//	ASSERT( states.empty());
}

template < typename device_state, typename state_desc >
void state_cache<device_state, state_desc>::clear_state_array( )
{
	// FUNCTION BODY[0x12dbd0] for rasterizer state
	for ( u32 i=0; i<states.size(); ++i)
		safe_release( states[i].state);

	states.erase(states.begin(),states.end());
}

template <>
void state_cache<ID3D11RasterizerState, D3D11_RASTERIZER_DESC>::create_state(
	D3D11_RASTERIZER_DESC desc,
	ID3D11RasterizerState** ppIState
)
{
	// FUNCTION BODY[0x797a30]
	CHECK_RESULT( device::ref().d3d_device()->CreateRasterizerState( &desc, ppIState));
}

template <>
void state_cache<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC>::create_state(
	D3D11_DEPTH_STENCIL_DESC desc,
	ID3D11DepthStencilState** ppIState
)
{
	// FUNCTION BODY[0x7979a0]
	CHECK_RESULT( device::ref().d3d_device()->CreateDepthStencilState( &desc, ppIState));
}

template <>
void state_cache<ID3D11BlendState, D3D11_BLEND_DESC>::create_state(
	D3D11_BLEND_DESC desc,
	ID3D11BlendState** ppIState
)
{
	// FUNCTION BODY[0x797910]
	CHECK_RESULT( device::ref().d3d_device()->CreateBlendState( &desc, ppIState));
}

template <>
void state_cache<ID3D11SamplerState, D3D11_SAMPLER_DESC>::create_state(
	D3D11_SAMPLER_DESC desc,
	ID3D11SamplerState** ppIState
)
{
	// FUNCTION BODY[0x797880]
	CHECK_RESULT( device::ref().d3d_device()->CreateSamplerState( &desc, ppIState));
}

template class state_cache<ID3D11RasterizerState, D3D11_RASTERIZER_DESC>;
template class state_cache<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC>;
template class state_cache<ID3D11BlendState, D3D11_BLEND_DESC>;
template class state_cache<ID3D11SamplerState, D3D11_SAMPLER_DESC>;

} // namespace render
} // namespace vostok
