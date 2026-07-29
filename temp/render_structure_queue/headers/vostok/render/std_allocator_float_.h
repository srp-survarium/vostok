////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

namespace vostok {
namespace render {

class std_allocator< float > {
public:
	inline					std_allocator<float>( ) { /* no source */ }

	inline	float*			address				( float& arg_0 ) const { /* no source */ }
	inline	float const*	address				( float const& arg_0 ) const { /* no source */ }

	inline	float*			allocate			( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	float*			allocate			( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	float*			_M_allocate			( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	float*			_M_allocate			( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*			__charalloc			( u32 arg_0 ) { /* no source */ }

	inline	void			deallocate			( float* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void			deallocate			( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void			construct			( float* arg_0, float const& arg_1 ) { /* no source */ }

	inline	void			destroy				( float* arg_0 ) { /* no source */ }

	inline	u32				max_size			( ) const { /* no source */ }
}; // class std_allocator< float >

STATIC_SIZE_ASSERT(std_allocator< float >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
