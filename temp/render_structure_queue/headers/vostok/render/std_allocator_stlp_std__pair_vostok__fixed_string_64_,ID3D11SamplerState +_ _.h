////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class stlp_std::pair<vostok::fixed_string<64>,ID3D11SamplerState *>;

namespace vostok {
namespace render {

class std_allocator< std::pair< fixed_string< 64 >, ID3D11SamplerState* > > {
public:
	inline				std_allocator<stlp_std::pair<vostok::fixed_string<64>,ID3D11SamplerState *> >( ) { /* no source */ }

	inline	std::pair< fixed_string< 64 >, ID3D11SamplerState* >*	address		( std::pair< fixed_string< 64 >, ID3D11SamplerState* >& arg_0 ) const { /* no source */ }
	inline	std::pair< fixed_string< 64 >, ID3D11SamplerState* > const*	address		( std::pair< fixed_string< 64 >, ID3D11SamplerState* > const& arg_0 ) const { /* no source */ }

	inline	std::pair< fixed_string< 64 >, ID3D11SamplerState* >*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< fixed_string< 64 >, ID3D11SamplerState* >*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	std::pair< fixed_string< 64 >, ID3D11SamplerState* >*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	std::pair< fixed_string< 64 >, ID3D11SamplerState* >*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( std::pair< fixed_string< 64 >, ID3D11SamplerState* >* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							std::pair< fixed_string< 64 >, ID3D11SamplerState* >*	arg_0,
							std::pair< fixed_string< 64 >, ID3D11SamplerState* > const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( std::pair< fixed_string< 64 >, ID3D11SamplerState* >* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< std::pair< fixed_string< 64 >, ID3D11SamplerState* > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< fixed_string< 64 >, ID3D11SamplerState* > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
