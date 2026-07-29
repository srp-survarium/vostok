////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::trample_desc;

namespace vostok {
namespace render {

class std_allocator< trample_desc > {
public:
	inline							std_allocator<vostok::render::trample_desc>( ) { /* no source */ }

	inline	trample_desc*			address		( trample_desc& arg_0 ) const { /* no source */ }
	inline	trample_desc const*		address		( trample_desc const& arg_0 ) const { /* no source */ }

	inline	trample_desc*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	trample_desc*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	trample_desc*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	trample_desc*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*					__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void					deallocate	( trample_desc* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void					deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void					construct	( trample_desc* arg_0, trample_desc const& arg_1 ) { /* no source */ }

	inline	void					destroy		( trample_desc* arg_0 ) { /* no source */ }

	inline	u32						max_size	( ) const { /* no source */ }
}; // class std_allocator< trample_desc >

STATIC_SIZE_ASSERT(std_allocator< trample_desc >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
