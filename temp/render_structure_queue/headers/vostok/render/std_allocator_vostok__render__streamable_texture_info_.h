////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::streamable_texture_info;

namespace vostok {
namespace render {

class std_allocator< streamable_texture_info > {
public:
	inline										std_allocator<vostok::render::streamable_texture_info>( ) { /* no source */ }

	inline	streamable_texture_info*			address		( streamable_texture_info& arg_0 ) const { /* no source */ }
	inline	streamable_texture_info const*		address		( streamable_texture_info const& arg_0 ) const { /* no source */ }

	inline	streamable_texture_info*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	streamable_texture_info*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	streamable_texture_info*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	streamable_texture_info*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( streamable_texture_info* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( streamable_texture_info* arg_0, streamable_texture_info const& arg_1 ) { /* no source */ }

	inline	void								destroy		( streamable_texture_info* arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< streamable_texture_info >

STATIC_SIZE_ASSERT(std_allocator< streamable_texture_info >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
