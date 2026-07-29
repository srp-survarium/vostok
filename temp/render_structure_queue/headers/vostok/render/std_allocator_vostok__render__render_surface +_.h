////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::render_surface;
class vostok::render::render_surface*;

namespace vostok {
namespace render {

class std_allocator< render_surface* > {
public:
	inline								std_allocator<vostok::render::render_surface *>( ) { /* no source */ }

	inline	render_surface**			address		( render_surface*& arg_0 ) const { /* no source */ }
	inline	render_surface* const*		address		( render_surface* const& arg_0 ) const { /* no source */ }

	inline	render_surface**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	render_surface**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	render_surface**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	render_surface**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( render_surface** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( render_surface** arg_0, render_surface* const& arg_1 ) { /* no source */ }

	inline	void						destroy		( render_surface** arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< render_surface* >

STATIC_SIZE_ASSERT(std_allocator< render_surface* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
