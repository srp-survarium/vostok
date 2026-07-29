////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

namespace vostok {
namespace render {

class std_allocator< ID3D11SamplerState* > {
public:
	inline									std_allocator<ID3D11SamplerState *>	( ) { /* no source */ }

	inline	ID3D11SamplerState**			address								( ID3D11SamplerState*& arg_0 ) const { /* no source */ }
	inline	ID3D11SamplerState* const*		address								( ID3D11SamplerState* const& arg_0 ) const { /* no source */ }

	inline	ID3D11SamplerState**			allocate							( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	ID3D11SamplerState**			allocate							( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	ID3D11SamplerState**			_M_allocate							( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	ID3D11SamplerState**			_M_allocate							( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc							( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate							( ID3D11SamplerState** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate							( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct							( ID3D11SamplerState** arg_0, ID3D11SamplerState* const& arg_1 ) { /* no source */ }

	inline	void							destroy								( ID3D11SamplerState** arg_0 ) { /* no source */ }

	inline	u32								max_size							( ) const { /* no source */ }
}; // class std_allocator< ID3D11SamplerState* >

STATIC_SIZE_ASSERT(std_allocator< ID3D11SamplerState* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
