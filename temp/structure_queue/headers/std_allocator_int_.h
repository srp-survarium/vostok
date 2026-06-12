////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

namespace survarium {

class std_allocator< int > {
public:
	inline					std_allocator<int>	( ) { /* no source */ }

	inline	s32*			address				( s32& arg_0 ) const { /* no source */ }
	inline	s32 const*		address				( s32 const& arg_0 ) const { /* no source */ }

	inline	s32*			allocate			( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	s32*			allocate			( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	s32*			_M_allocate			( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	s32*			_M_allocate			( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*			__charalloc			( u32 arg_0 ) { /* no source */ }

	inline	void			deallocate			( s32* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void			deallocate			( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void			construct			( s32* arg_0, s32 const& arg_1 ) { /* no source */ }

	inline	void			destroy				( s32* arg_0 ) { /* no source */ }

	inline	u32				max_size			( ) const { /* no source */ }
}; // class std_allocator< int >

STATIC_SIZE_ASSERT(std_allocator< int >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
