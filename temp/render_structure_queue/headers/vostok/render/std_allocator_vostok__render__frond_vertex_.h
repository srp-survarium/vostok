////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::frond_vertex;

namespace vostok {
namespace render {

class std_allocator< frond_vertex > {
public:
	inline							std_allocator<vostok::render::frond_vertex>( ) { /* no source */ }

	inline	frond_vertex*			address		( frond_vertex& arg_0 ) const { /* no source */ }
	inline	frond_vertex const*		address		( frond_vertex const& arg_0 ) const { /* no source */ }

	inline	frond_vertex*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	frond_vertex*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	frond_vertex*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	frond_vertex*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*					__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void					deallocate	( frond_vertex* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void					deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void					construct	( frond_vertex* arg_0, frond_vertex const& arg_1 ) { /* no source */ }

	inline	void					destroy		( frond_vertex* arg_0 ) { /* no source */ }

	inline	u32						max_size	( ) const { /* no source */ }
}; // class std_allocator< frond_vertex >

STATIC_SIZE_ASSERT(std_allocator< frond_vertex >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
