// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_STATE_CACHE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_STATE_CACHE_H_INCLUDED
#include <vostok/render/core/memory.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/state_utils.h>

namespace vostok {
namespace render {

using state_utils::operator==;

template < typename device_state, typename state_desc >
class state_cache {
public:
	state_cache( );
	~state_cache( );

	void clear_state_array( );

	device_state* get_state( state_desc const& desc );

private:
	struct state_record {
		u32 crc;
		device_state* state;
	};

	void create_state( state_desc desc, device_state** state );

	device_state* find( state_desc const& desc, u32 crc );

private:
	vector<state_record> states;
};

STATIC_SIZE_ASSERT( (state_cache<ID3D11RasterizerState, D3D11_RASTERIZER_DESC>), 0xC );
STATIC_SIZE_ASSERT( (state_cache<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC>), 0xC );
STATIC_SIZE_ASSERT( (state_cache<ID3D11BlendState, D3D11_BLEND_DESC>), 0xC );
STATIC_SIZE_ASSERT( (state_cache<ID3D11SamplerState, D3D11_SAMPLER_DESC>), 0xC );

} // namespace render
} // namespace vostok

#include <vostok/render/core/state_cache_inline.h>

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_STATE_CACHE_H_INCLUDED
