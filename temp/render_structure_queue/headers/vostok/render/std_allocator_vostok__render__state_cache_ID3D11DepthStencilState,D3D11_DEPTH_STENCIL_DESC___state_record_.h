////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>::state_record;

namespace vostok {
namespace render {

class std_allocator< state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record > {
public:
	inline				std_allocator<vostok::render::state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>::state_record>( ) { /* no source */ }

	inline	state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record*	address		(
							state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record&	arg_0
						) const { /* no source */ }
	inline	state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record const*	address		(
							state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record const&	arg_0
						) const { /* no source */ }

	inline	state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	(
							state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record*	arg_0,
							u32		arg_1
						) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record*	arg_0,
							state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record const&	arg_1
						) { /* no source */ }

	inline	void		destroy		(
							state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record*	arg_0
						) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record >

STATIC_SIZE_ASSERT(std_allocator< state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >::state_record >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
