////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::signature_layout_pair;

namespace vostok {
namespace render {

class std_allocator< signature_layout_pair > {
public:
	inline									std_allocator<vostok::render::signature_layout_pair>( ) { /* no source */ }

	inline	signature_layout_pair*			address		( signature_layout_pair& arg_0 ) const { /* no source */ }
	inline	signature_layout_pair const*	address		( signature_layout_pair const& arg_0 ) const { /* no source */ }

	inline	signature_layout_pair*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	signature_layout_pair*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	signature_layout_pair*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	signature_layout_pair*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate	( signature_layout_pair* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct	( signature_layout_pair* arg_0, signature_layout_pair const& arg_1 ) { /* no source */ }

	inline	void							destroy		( signature_layout_pair* arg_0 ) { /* no source */ }

	inline	u32								max_size	( ) const { /* no source */ }
}; // class std_allocator< signature_layout_pair >

STATIC_SIZE_ASSERT(std_allocator< signature_layout_pair >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
