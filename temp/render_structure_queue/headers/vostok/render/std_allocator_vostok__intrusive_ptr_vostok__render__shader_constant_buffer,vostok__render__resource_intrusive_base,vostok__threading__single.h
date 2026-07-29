////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_buffer;

namespace vostok {
namespace render {

class std_allocator< shader_constant_buffer_ptr > {
public:
	inline											std_allocator<vostok::intrusive_ptr<vostok::render::shader_constant_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> >( ) { /* no source */ }

	inline	shader_constant_buffer_ptr*				address		( shader_constant_buffer_ptr& arg_0 ) const { /* no source */ }
	inline	shader_constant_buffer_ptr const*		address		( shader_constant_buffer_ptr const& arg_0 ) const { /* no source */ }

	inline	shader_constant_buffer_ptr*				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	shader_constant_buffer_ptr*				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	shader_constant_buffer_ptr*				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	shader_constant_buffer_ptr*				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*									__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void									deallocate	( shader_constant_buffer_ptr* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void									deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void									construct	( shader_constant_buffer_ptr* arg_0, shader_constant_buffer_ptr const& arg_1 ) { /* no source */ }

	inline	void									destroy		( shader_constant_buffer_ptr* arg_0 ) { /* no source */ }

	inline	u32										max_size	( ) const { /* no source */ }
}; // class std_allocator< shader_constant_buffer_ptr >

STATIC_SIZE_ASSERT(std_allocator< shader_constant_buffer_ptr >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
