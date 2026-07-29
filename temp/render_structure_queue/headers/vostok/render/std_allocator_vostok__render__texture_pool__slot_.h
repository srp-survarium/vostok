////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::texture_pool::slot;

namespace vostok {
namespace render {

class std_allocator< texture_pool::slot > {
public:
	inline									std_allocator<vostok::render::texture_pool::slot>( ) { /* no source */ }

	inline	texture_pool::slot*				address		( texture_pool::slot& arg_0 ) const { /* no source */ }
	inline	texture_pool::slot const*		address		( texture_pool::slot const& arg_0 ) const { /* no source */ }

	inline	texture_pool::slot*				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	texture_pool::slot*				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	texture_pool::slot*				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	texture_pool::slot*				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate	( texture_pool::slot* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct	( texture_pool::slot* arg_0, texture_pool::slot const& arg_1 ) { /* no source */ }

	inline	void							destroy		( texture_pool::slot* arg_0 ) { /* no source */ }

	inline	u32								max_size	( ) const { /* no source */ }
}; // class std_allocator< texture_pool::slot >

STATIC_SIZE_ASSERT(std_allocator< texture_pool::slot >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
