////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATE_CACHE_STATE_RECORD_H_INCLUDED
#define RENDER_STATE_CACHE_STATE_RECORD_H_INCLUDED

/* INCLUDES */
struct ID3D11SamplerState;

namespace vostok {
namespace render {

struct state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record {

	/* 0x0000 */	u32						crc;
	/* 0x0004 */	ID3D11SamplerState*		state;
}; // struct state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record

STATIC_SIZE_ASSERT(state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATE_CACHE_STATE_RECORD_H_INCLUDED
