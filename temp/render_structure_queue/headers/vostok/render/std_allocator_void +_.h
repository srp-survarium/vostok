////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

namespace vostok {
namespace render {

class std_allocator< void* > {
public:
	inline					std_allocator<void *>	( ) { /* no source */ }

	inline	void**			address					( void*& arg_0 ) const { /* no source */ }
	inline	void* const*	address					( void* const& arg_0 ) const { /* no source */ }

	inline	void**			allocate				( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	void**			allocate				( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	void**			_M_allocate				( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	void**			_M_allocate				( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*			__charalloc				( u32 arg_0 ) { /* no source */ }

	inline	void			deallocate				( void** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void			deallocate				( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void			construct				( void** arg_0, void* const& arg_1 ) { /* no source */ }

	inline	void			destroy					( void** arg_0 ) { /* no source */ }

	inline	u32				max_size				( ) const { /* no source */ }
}; // class std_allocator< void* >

STATIC_SIZE_ASSERT(std_allocator< void* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
