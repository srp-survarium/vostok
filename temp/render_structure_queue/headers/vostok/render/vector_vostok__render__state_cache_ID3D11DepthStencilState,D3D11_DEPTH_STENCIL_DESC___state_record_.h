////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>::state_record,vostok::render::std_allocator<vostok::render::state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>::state_record> >
	stlp_std::vector<vostok::render::state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>::state_record >;
class vostok::render::state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>::state_record;

namespace vostok {
namespace render {

class vector< state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record > : public std::vector< state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record > {
public:
	inline				vector<vostok::render::state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>::state_record>( ) { /* no source */ }
	inline				vector<vostok::render::state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>::state_record>(
							u32		arg_0,
							state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record const&	arg_1
						) { /* no source */ }
	inline	explicit	vector<vostok::render::state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>::state_record>( u32 arg_0 ) { /* no source */ }

	inline	state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record&	operator[]	( u32 arg_0 ) { /* no source */ }
	inline	state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32			size		( ) const { /* no source */ }

	inline				~vector<vostok::render::state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>::state_record>( ) { /* no source */ }
}; // class vector< state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record >

STATIC_SIZE_ASSERT(vector< state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
