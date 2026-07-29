////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::streaming_ready_texture;

namespace vostok {
namespace render {

class std_allocator< streaming_ready_texture > {
public:
	inline										std_allocator<vostok::render::streaming_ready_texture>( ) { /* no source */ }

	inline	streaming_ready_texture*			address		( streaming_ready_texture& arg_0 ) const { /* no source */ }
	inline	streaming_ready_texture const*		address		( streaming_ready_texture const& arg_0 ) const { /* no source */ }

	inline	streaming_ready_texture*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	streaming_ready_texture*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	streaming_ready_texture*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	streaming_ready_texture*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( streaming_ready_texture* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( streaming_ready_texture* arg_0, streaming_ready_texture const& arg_1 ) { /* no source */ }

	inline	void								destroy		( streaming_ready_texture* arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< streaming_ready_texture >

STATIC_SIZE_ASSERT(std_allocator< streaming_ready_texture >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
