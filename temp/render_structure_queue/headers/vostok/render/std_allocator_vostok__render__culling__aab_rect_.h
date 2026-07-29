////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::culling::aab_rect;

namespace vostok {
namespace render {

class std_allocator< culling::aab_rect > {
public:
	inline								std_allocator<vostok::render::culling::aab_rect>( ) { /* no source */ }

	inline	culling::aab_rect*			address		( culling::aab_rect& arg_0 ) const { /* no source */ }
	inline	culling::aab_rect const*	address		( culling::aab_rect const& arg_0 ) const { /* no source */ }

	inline	culling::aab_rect*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	culling::aab_rect*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	culling::aab_rect*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	culling::aab_rect*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( culling::aab_rect* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( culling::aab_rect* arg_0, culling::aab_rect const& arg_1 ) { /* no source */ }

	inline	void						destroy		( culling::aab_rect* arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< culling::aab_rect >

STATIC_SIZE_ASSERT(std_allocator< culling::aab_rect >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
