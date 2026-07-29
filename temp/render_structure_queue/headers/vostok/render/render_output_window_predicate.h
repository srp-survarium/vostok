////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_OUTPUT_WINDOW_PREDICATE_H_INCLUDED
#define RENDER_RENDER_OUTPUT_WINDOW_PREDICATE_H_INCLUDED

/* INCLUDES */
struct HWND__;

/* FORWARD REFS */
class vostok::render::render_output_window;

namespace vostok {
namespace render {

struct render_output_window_predicate {
	inline	explicit	render_output_window_predicate	( HWND__* arg_0 ) { /* no source */ }

	inline	bool		operator()						( render_output_window* arg_0 ) { /* no source */ }

	/* 0x0000 */	HWND__*		m_window;
}; // struct render_output_window_predicate

STATIC_SIZE_ASSERT(render_output_window_predicate, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_OUTPUT_WINDOW_PREDICATE_H_INCLUDED
