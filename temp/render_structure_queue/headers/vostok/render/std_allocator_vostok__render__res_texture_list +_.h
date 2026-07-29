////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::res_texture_list;
class vostok::render::res_texture_list*;

namespace vostok {
namespace render {

class std_allocator< res_texture_list* > {
public:
	inline								std_allocator<vostok::render::res_texture_list *>( ) { /* no source */ }

	inline	res_texture_list**			address		( res_texture_list*& arg_0 ) const { /* no source */ }
	inline	res_texture_list* const*	address		( res_texture_list* const& arg_0 ) const { /* no source */ }

	inline	res_texture_list**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	res_texture_list**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	res_texture_list**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	res_texture_list**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( res_texture_list** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( res_texture_list** arg_0, res_texture_list* const& arg_1 ) { /* no source */ }

	inline	void						destroy		( res_texture_list** arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< res_texture_list* >

STATIC_SIZE_ASSERT(std_allocator< res_texture_list* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
