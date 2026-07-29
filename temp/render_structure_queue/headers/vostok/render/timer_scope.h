////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TIMER_SCOPE_H_INCLUDED
#define RENDER_TIMER_SCOPE_H_INCLUDED

/* INCLUDES */
class vostok::timing::timer;
struct vostok::render::statistics_float;

namespace vostok {
namespace render {

struct timer_scope : public boost::noncopyable {
	inline	explicit	timer_scope	( statistics_float& arg_0 ) { /* no source */ }
	inline				~timer_scope( ) { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	statistics_float&		m_counter;
	/* 0x0008 */	timing::timer			m_cpu_tumer;
}; // struct timer_scope

STATIC_SIZE_ASSERT(timer_scope, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TIMER_SCOPE_H_INCLUDED
