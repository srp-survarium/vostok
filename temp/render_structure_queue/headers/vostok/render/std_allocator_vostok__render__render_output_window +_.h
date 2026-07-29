////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::render_output_window;
class vostok::render::render_output_window*;

namespace vostok {
namespace render {

class std_allocator< render_output_window* > {
public:
	inline									std_allocator<vostok::render::render_output_window *>( ) { /* no source */ }

	inline	render_output_window**			address		( render_output_window*& arg_0 ) const { /* no source */ }
	inline	render_output_window* const*	address		( render_output_window* const& arg_0 ) const { /* no source */ }

	inline	render_output_window**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	render_output_window**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	render_output_window**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	render_output_window**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate	( render_output_window** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct	( render_output_window** arg_0, render_output_window* const& arg_1 ) { /* no source */ }

	inline	void							destroy		( render_output_window** arg_0 ) { /* no source */ }

	inline	u32								max_size	( ) const { /* no source */ }
}; // class std_allocator< render_output_window* >

STATIC_SIZE_ASSERT(std_allocator< render_output_window* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
