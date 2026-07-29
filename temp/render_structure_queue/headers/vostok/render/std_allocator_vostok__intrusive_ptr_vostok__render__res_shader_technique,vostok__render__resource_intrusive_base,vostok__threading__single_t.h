////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* INCLUDES */
class vostok::render::res_shader_technique;

namespace vostok {
namespace render {

class std_allocator< res_shader_technique_ptr > {
public:
	inline										std_allocator<vostok::intrusive_ptr<vostok::render::res_shader_technique,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> >( ) { /* no source */ }

	inline	res_shader_technique_ptr*			address		( res_shader_technique_ptr& arg_0 ) const { /* no source */ }
	inline	res_shader_technique_ptr const*		address		( res_shader_technique_ptr const& arg_0 ) const { /* no source */ }

	inline	res_shader_technique_ptr*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	res_shader_technique_ptr*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	res_shader_technique_ptr*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	res_shader_technique_ptr*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( res_shader_technique_ptr* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( res_shader_technique_ptr* arg_0, res_shader_technique_ptr const& arg_1 ) { /* no source */ }

	inline	void								destroy		( res_shader_technique_ptr* arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< res_shader_technique_ptr >

STATIC_SIZE_ASSERT(std_allocator< res_shader_technique_ptr >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
