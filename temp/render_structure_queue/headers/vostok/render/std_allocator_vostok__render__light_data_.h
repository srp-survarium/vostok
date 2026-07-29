////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::light_data;

namespace vostok {
namespace render {

class std_allocator< light_data > {
public:
	inline							std_allocator<vostok::render::light_data>( ) { /* no source */ }

	inline	light_data*				address		( light_data& arg_0 ) const { /* no source */ }
	inline	light_data const*		address		( light_data const& arg_0 ) const { /* no source */ }

	inline	light_data*				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	light_data*				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	light_data*				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	light_data*				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*					__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void					deallocate	( light_data* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void					deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void					construct	( light_data* arg_0, light_data const& arg_1 ) { /* no source */ }

	inline	void					destroy		( light_data* arg_0 ) { /* no source */ }

	inline	u32						max_size	( ) const { /* no source */ }
}; // class std_allocator< light_data >

STATIC_SIZE_ASSERT(std_allocator< light_data >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
