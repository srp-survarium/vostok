#ifndef VOSTOK_RENDER_CORE_DX11_STATE_CACHE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_STATE_CACHE_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {

namespace state_utils {
	bool operator==( D3D11_RASTERIZER_DESC const& left, D3D11_RASTERIZER_DESC const& right );
	bool operator==( D3D11_DEPTH_STENCIL_DESC const& left, D3D11_DEPTH_STENCIL_DESC const& right );
	bool operator==( D3D11_BLEND_DESC const& left, D3D11_BLEND_DESC const& right );
	bool operator==( D3D11_SAMPLER_DESC const& left, D3D11_SAMPLER_DESC const& right );

	u32 get_hash( D3D11_RASTERIZER_DESC const& desc );
	u32 get_hash( D3D11_DEPTH_STENCIL_DESC const& desc );
	u32 get_hash( D3D11_BLEND_DESC const& desc );
	u32 get_hash( D3D11_SAMPLER_DESC const& desc );
} // namespace state_utils

using state_utils::operator==;

template < typename device_state, typename state_desc >
class state_cache {
public:
	state_cache( );
	~state_cache( );

	void clear_state_array( );

	device_state* get_state( state_desc const& desc )
	{
		// FUNCTION BODY[0x1250c0] for rasterizer state
		u32 const crc = state_utils::get_hash( desc );
		device_state* result = find( desc, crc );
		if ( result )
			return result;

		state_record record;
		record.crc = crc;
		create_state( desc, &record.state );
		states.push_back( record );
		return record.state;
	}

private:
	struct state_record {
		u32 crc;
		device_state* state;
	};

	void create_state( state_desc desc, device_state** state );

	device_state* find( state_desc const& desc, u32 crc )
	{
		// FUNCTION BODY[0x127150] for rasterizer state
		for ( u32 index = 0; index < states.size( ); ++index ) {
			if ( states[index].crc != crc )
				continue;

			state_desc candidate;
			states[index].state->GetDesc( &candidate );
			if ( candidate == desc )
				return states[index].state;
		}
		return 0;
	}

private:
	vector<state_record> states;
};

STATIC_SIZE_ASSERT( (state_cache<ID3D11RasterizerState, D3D11_RASTERIZER_DESC>), 0xC );
STATIC_SIZE_ASSERT( (state_cache<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC>), 0xC );
STATIC_SIZE_ASSERT( (state_cache<ID3D11BlendState, D3D11_BLEND_DESC>), 0xC );
STATIC_SIZE_ASSERT( (state_cache<ID3D11SamplerState, D3D11_SAMPLER_DESC>), 0xC );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_STATE_CACHE_H_INCLUDED
