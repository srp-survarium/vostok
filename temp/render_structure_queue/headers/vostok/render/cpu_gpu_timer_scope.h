////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CPU_GPU_TIMER_SCOPE_H_INCLUDED
#define RENDER_CPU_GPU_TIMER_SCOPE_H_INCLUDED

/* INCLUDES */
class vostok::timing::timer;
struct vostok::render::statistics_cpu_gpu;

namespace vostok {
namespace render {

struct cpu_gpu_timer_scope : public boost::noncopyable {
	inline	explicit	cpu_gpu_timer_scope	( statistics_cpu_gpu& arg_0 ) { /* no source */ }
	inline				~cpu_gpu_timer_scope( ) { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	statistics_cpu_gpu&		m_counter;
	/* 0x0008 */	timing::timer			m_cpu_tumer;
}; // struct cpu_gpu_timer_scope

STATIC_SIZE_ASSERT(cpu_gpu_timer_scope, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CPU_GPU_TIMER_SCOPE_H_INCLUDED
