////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::shader_constant;

namespace vostok {
namespace render {

class std_allocator< shader_constant > {
public:
	inline								std_allocator<vostok::render::shader_constant>( ) { /* no source */ }

	inline	shader_constant*			address		( shader_constant& arg_0 ) const { /* no source */ }
	inline	shader_constant const*		address		( shader_constant const& arg_0 ) const { /* no source */ }

	inline	shader_constant*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	shader_constant*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	shader_constant*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	shader_constant*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( shader_constant* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( shader_constant* arg_0, shader_constant const& arg_1 ) { /* no source */ }

	inline	void						destroy		( shader_constant* arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< shader_constant >

STATIC_SIZE_ASSERT(std_allocator< shader_constant >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
