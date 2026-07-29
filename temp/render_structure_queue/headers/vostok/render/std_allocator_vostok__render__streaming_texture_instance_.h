////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::streaming_texture_instance;

namespace vostok {
namespace render {

class std_allocator< streaming_texture_instance > {
public:
	inline											std_allocator<vostok::render::streaming_texture_instance>( ) { /* no source */ }

	inline	streaming_texture_instance*				address		( streaming_texture_instance& arg_0 ) const { /* no source */ }
	inline	streaming_texture_instance const*		address		( streaming_texture_instance const& arg_0 ) const { /* no source */ }

	inline	streaming_texture_instance*				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	streaming_texture_instance*				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	streaming_texture_instance*				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	streaming_texture_instance*				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*									__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void									deallocate	( streaming_texture_instance* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void									deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void									construct	( streaming_texture_instance* arg_0, streaming_texture_instance const& arg_1 ) { /* no source */ }

	inline	void									destroy		( streaming_texture_instance* arg_0 ) { /* no source */ }

	inline	u32										max_size	( ) const { /* no source */ }
}; // class std_allocator< streaming_texture_instance >

STATIC_SIZE_ASSERT(std_allocator< streaming_texture_instance >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
