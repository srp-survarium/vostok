////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::untyped_buffer;
class vostok::render::untyped_buffer*;

namespace vostok {
namespace render {

class std_allocator< untyped_buffer* > {
public:
	inline								std_allocator<vostok::render::untyped_buffer *>( ) { /* no source */ }

	inline	untyped_buffer**			address		( untyped_buffer*& arg_0 ) const { /* no source */ }
	inline	untyped_buffer* const*		address		( untyped_buffer* const& arg_0 ) const { /* no source */ }

	inline	untyped_buffer**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	untyped_buffer**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	untyped_buffer**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	untyped_buffer**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( untyped_buffer** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( untyped_buffer** arg_0, untyped_buffer* const& arg_1 ) { /* no source */ }

	inline	void						destroy		( untyped_buffer** arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< untyped_buffer* >

STATIC_SIZE_ASSERT(std_allocator< untyped_buffer* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
