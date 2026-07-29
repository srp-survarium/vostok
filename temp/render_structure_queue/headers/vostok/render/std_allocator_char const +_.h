////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

namespace vostok {
namespace render {

class std_allocator< pcstr > {
public:
	inline					std_allocator<char const *>	( ) { /* no source */ }

	inline	pcstr*			address						( pcstr& arg_0 ) const { /* no source */ }
	inline	pcstr const*	address						( pcstr const& arg_0 ) const { /* no source */ }

	inline	pcstr*			allocate					( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	pcstr*			allocate					( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	pcstr*			_M_allocate					( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	pcstr*			_M_allocate					( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*			__charalloc					( u32 arg_0 ) { /* no source */ }

	inline	void			deallocate					( pcstr* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void			deallocate					( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void			construct					( pcstr* arg_0, pcstr const& arg_1 ) { /* no source */ }

	inline	void			destroy						( pcstr* arg_0 ) { /* no source */ }

	inline	u32				max_size					( ) const { /* no source */ }
}; // class std_allocator< pcstr >

STATIC_SIZE_ASSERT(std_allocator< pcstr >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
