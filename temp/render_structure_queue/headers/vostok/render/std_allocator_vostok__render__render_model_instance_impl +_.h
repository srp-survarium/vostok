////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::render_model_instance_impl;
class vostok::render::render_model_instance_impl*;

namespace vostok {
namespace render {

class std_allocator< render_model_instance_impl* > {
public:
	inline											std_allocator<vostok::render::render_model_instance_impl *>( ) { /* no source */ }

	inline	render_model_instance_impl**			address		( render_model_instance_impl*& arg_0 ) const { /* no source */ }
	inline	render_model_instance_impl* const*		address		( render_model_instance_impl* const& arg_0 ) const { /* no source */ }

	inline	render_model_instance_impl**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	render_model_instance_impl**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	render_model_instance_impl**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	render_model_instance_impl**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*									__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void									deallocate	( render_model_instance_impl** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void									deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void									construct	( render_model_instance_impl** arg_0, render_model_instance_impl* const& arg_1 ) { /* no source */ }

	inline	void									destroy		( render_model_instance_impl** arg_0 ) { /* no source */ }

	inline	u32										max_size	( ) const { /* no source */ }
}; // class std_allocator< render_model_instance_impl* >

STATIC_SIZE_ASSERT(std_allocator< render_model_instance_impl* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
