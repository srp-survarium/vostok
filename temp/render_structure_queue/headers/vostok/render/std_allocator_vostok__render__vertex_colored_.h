////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::vertex_colored;

namespace vostok {
namespace render {

class std_allocator< vertex_colored > {
public:
	inline								std_allocator<vostok::render::vertex_colored>( ) { /* no source */ }

	inline	vertex_colored*				address		( vertex_colored& arg_0 ) const { /* no source */ }
	inline	vertex_colored const*		address		( vertex_colored const& arg_0 ) const { /* no source */ }

	inline	vertex_colored*				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	vertex_colored*				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	vertex_colored*				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	vertex_colored*				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( vertex_colored* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( vertex_colored* arg_0, vertex_colored const& arg_1 ) { /* no source */ }

	inline	void						destroy		( vertex_colored* arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< vertex_colored >

STATIC_SIZE_ASSERT(std_allocator< vertex_colored >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
