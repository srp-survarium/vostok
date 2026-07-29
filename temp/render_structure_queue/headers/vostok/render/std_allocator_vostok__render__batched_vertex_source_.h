////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::batched_vertex_source;

namespace vostok {
namespace render {

class std_allocator< batched_vertex_source > {
public:
	inline									std_allocator<vostok::render::batched_vertex_source>( ) { /* no source */ }

	inline	batched_vertex_source*			address		( batched_vertex_source& arg_0 ) const { /* no source */ }
	inline	batched_vertex_source const*	address		( batched_vertex_source const& arg_0 ) const { /* no source */ }

	inline	batched_vertex_source*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	batched_vertex_source*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	batched_vertex_source*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	batched_vertex_source*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate	( batched_vertex_source* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct	( batched_vertex_source* arg_0, batched_vertex_source const& arg_1 ) { /* no source */ }

	inline	void							destroy		( batched_vertex_source* arg_0 ) { /* no source */ }

	inline	u32								max_size	( ) const { /* no source */ }
}; // class std_allocator< batched_vertex_source >

STATIC_SIZE_ASSERT(std_allocator< batched_vertex_source >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
