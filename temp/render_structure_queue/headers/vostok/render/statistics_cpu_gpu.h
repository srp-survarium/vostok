////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATISTICS_CPU_GPU_H_INCLUDED
#define RENDER_STATISTICS_CPU_GPU_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_base;
struct vostok::render::statistics_float;

/* FORWARD REFS */
class vostok::fs_new::virtual_path_string;
class vostok::render::statistics_group;

namespace vostok {
namespace render {

struct statistics_cpu_gpu : public statistics_base {
	inline			statistics_cpu_gpu	( statistics_group* arg_0, pcstr arg_1 ) { /* no source */ }

	virtual	void	start				( ) override;

	virtual	void	print				( fs_new::virtual_path_string& out_result ) override;

	inline			~statistics_cpu_gpu	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_base */
	/* 0x0098 */	statistics_float	cpu_time;
	/* 0x0170 */	statistics_float	gpu_time;
}; // struct statistics_cpu_gpu

STATIC_SIZE_ASSERT(statistics_cpu_gpu, 0x248);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATISTICS_CPU_GPU_H_INCLUDED
