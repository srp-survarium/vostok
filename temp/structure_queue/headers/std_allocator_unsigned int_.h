////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

namespace survarium {

class std_allocator< u32 > {
public:
	inline					std_allocator<unsigned int>	( ) { /* no source */ }

	inline	u32*			address						( u32& arg_0 ) const { /* no source */ }
	inline	u32 const*		address						( u32 const& arg_0 ) const { /* no source */ }

	inline	u32*			allocate					( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	u32*			allocate					( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	u32*			_M_allocate					( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	u32*			_M_allocate					( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*			__charalloc					( u32 arg_0 ) { /* no source */ }

	inline	void			deallocate					( u32* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void			deallocate					( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void			construct					( u32* arg_0, u32 const& arg_1 ) { /* no source */ }

	inline	void			destroy						( u32* arg_0 ) { /* no source */ }

	inline	u32				max_size					( ) const { /* no source */ }
}; // class std_allocator< u32 >

STATIC_SIZE_ASSERT(std_allocator< u32 >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
