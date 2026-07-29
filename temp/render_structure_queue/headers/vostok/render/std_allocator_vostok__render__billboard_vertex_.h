////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::billboard_vertex;

namespace vostok {
namespace render {

class std_allocator< billboard_vertex > {
public:
	inline								std_allocator<vostok::render::billboard_vertex>( ) { /* no source */ }

	inline	billboard_vertex*			address		( billboard_vertex& arg_0 ) const { /* no source */ }
	inline	billboard_vertex const*		address		( billboard_vertex const& arg_0 ) const { /* no source */ }

	inline	billboard_vertex*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	billboard_vertex*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	billboard_vertex*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	billboard_vertex*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( billboard_vertex* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( billboard_vertex* arg_0, billboard_vertex const& arg_1 ) { /* no source */ }

	inline	void						destroy		( billboard_vertex* arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< billboard_vertex >

STATIC_SIZE_ASSERT(std_allocator< billboard_vertex >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
