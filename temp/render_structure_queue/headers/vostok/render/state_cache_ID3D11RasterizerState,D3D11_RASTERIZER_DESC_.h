////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATE_CACHE_H_INCLUDED
#define RENDER_STATE_CACHE_H_INCLUDED

/* INCLUDES */
class vostok::render::vector<vostok::render::state_cache<ID3D11RasterizerState,D3D11_RASTERIZER_DESC>::state_record>;

namespace vostok {
namespace render {

class state_cache< ID3D11RasterizerState, D3D11_RASTERIZER_DESC > {
public:
	inline								state_cache<ID3D11RasterizerState,D3D11_RASTERIZER_DESC>( ) { /* no source */ }
	inline								~state_cache<ID3D11RasterizerState,D3D11_RASTERIZER_DESC>( ) { /* no source */ }

			void						clear_state_array	( );

	inline	ID3D11RasterizerState*		get_state			( D3D11_RASTERIZER_DESC const& desc ) { /* no source */ }
private:
			void						create_state		( D3D11_RASTERIZER_DESC desc, ID3D11RasterizerState** ppIState );

	inline	ID3D11RasterizerState*		find				( D3D11_RASTERIZER_DESC const& desc, u32 CRC ) { /* no source */ }

	/* 0x0000 */	vector< state_cache< ID3D11RasterizerState, D3D11_RASTERIZER_DESC >::state_record >	states;
}; // class state_cache< ID3D11RasterizerState, D3D11_RASTERIZER_DESC >

STATIC_SIZE_ASSERT(state_cache< ID3D11RasterizerState, D3D11_RASTERIZER_DESC >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATE_CACHE_H_INCLUDED
