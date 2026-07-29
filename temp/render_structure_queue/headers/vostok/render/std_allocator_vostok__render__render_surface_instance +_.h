////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::render_surface_instance;
class vostok::render::render_surface_instance*;

namespace vostok {
namespace render {

class std_allocator< render_surface_instance* > {
public:
	inline										std_allocator<vostok::render::render_surface_instance *>( ) { /* no source */ }

	inline	render_surface_instance**			address		( render_surface_instance*& arg_0 ) const { /* no source */ }
	inline	render_surface_instance* const*		address		( render_surface_instance* const& arg_0 ) const { /* no source */ }

	inline	render_surface_instance**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	render_surface_instance**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	render_surface_instance**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	render_surface_instance**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( render_surface_instance** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( render_surface_instance** arg_0, render_surface_instance* const& arg_1 ) { /* no source */ }

	inline	void								destroy		( render_surface_instance** arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< render_surface_instance* >

STATIC_SIZE_ASSERT(std_allocator< render_surface_instance* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
