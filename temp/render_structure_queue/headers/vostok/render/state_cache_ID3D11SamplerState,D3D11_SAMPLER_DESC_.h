////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATE_CACHE_H_INCLUDED
#define RENDER_STATE_CACHE_H_INCLUDED

/* INCLUDES */
class vostok::render::vector<vostok::render::state_cache<ID3D11SamplerState,D3D11_SAMPLER_DESC>::state_record>;

namespace vostok {
namespace render {

class state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC > {
public:
	inline							state_cache<ID3D11SamplerState,D3D11_SAMPLER_DESC>( ) { /* no source */ }
	inline							~state_cache<ID3D11SamplerState,D3D11_SAMPLER_DESC>( ) { /* no source */ }

	inline	void					clear_state_array	( ) { /* no source */ }

	inline	ID3D11SamplerState*		get_state			( D3D11_SAMPLER_DESC const& arg_0 ) { /* no source */ }
private:
			void					create_state		( D3D11_SAMPLER_DESC desc, ID3D11SamplerState** ppIState );

	inline	ID3D11SamplerState*		find				( D3D11_SAMPLER_DESC const& arg_0, u32 arg_1 ) { /* no source */ }

	/* 0x0000 */	vector< state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >::state_record >	states;
}; // class state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >

STATIC_SIZE_ASSERT(state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATE_CACHE_H_INCLUDED
