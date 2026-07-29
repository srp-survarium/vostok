////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VECTOR_H_INCLUDED
#define RENDER_VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::render::render_output_window *,vostok::render::std_allocator<vostok::render::render_output_window *> >
	stlp_std::vector<vostok::render::render_output_window * >;
class vostok::render::render_output_window *;

/* FORWARD REFS */
class vostok::render::render_output_window;
class vostok::render::render_output_window*;

namespace vostok {
namespace render {

class vector< render_output_window* > : public std::vector< render_output_window* > {
public:
	inline									vector<vostok::render::render_output_window *>( ) { /* no source */ }
	inline									vector<vostok::render::render_output_window *>( u32 arg_0, render_output_window* const& arg_1 ) { /* no source */ }
	inline	explicit						vector<vostok::render::render_output_window *>( u32 arg_0 ) { /* no source */ }

	inline	render_output_window*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	render_output_window* const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<vostok::render::render_output_window *>( ) { /* no source */ }
}; // class vector< render_output_window* >

STATIC_SIZE_ASSERT(vector< render_output_window* >, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VECTOR_H_INCLUDED
