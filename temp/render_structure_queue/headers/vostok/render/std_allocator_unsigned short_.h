////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

namespace vostok {
namespace render {

class std_allocator< u16 > {
public:
	inline					std_allocator<unsigned short>	( ) { /* no source */ }

	inline	u16*			address							( u16& arg_0 ) const { /* no source */ }
	inline	u16 const*		address							( u16 const& arg_0 ) const { /* no source */ }

	inline	u16*			allocate						( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	u16*			allocate						( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	u16*			_M_allocate						( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	u16*			_M_allocate						( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*			__charalloc						( u32 arg_0 ) { /* no source */ }

	inline	void			deallocate						( u16* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void			deallocate						( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void			construct						( u16* arg_0, u16 const& arg_1 ) { /* no source */ }

	inline	void			destroy							( u16* arg_0 ) { /* no source */ }

	inline	u32				max_size						( ) const { /* no source */ }
}; // class std_allocator< u16 >

STATIC_SIZE_ASSERT(std_allocator< u16 >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
