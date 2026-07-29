////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DISTORTION_PASS_STATISTICS_GROUP_H_INCLUDED
#define RENDER_DISTORTION_PASS_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_cpu_gpu;
struct vostok::render::statistics_group;

namespace vostok {
namespace render {

struct distortion_pass_statistics_group : public statistics_group {
			explicit	distortion_pass_statistics_group	( pcstr group_name );
	inline				~distortion_pass_statistics_group	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0098 */	statistics_cpu_gpu		accumulate_time;
	/* 0x02e0 */	statistics_cpu_gpu		apply_time;
}; // struct distortion_pass_statistics_group

STATIC_SIZE_ASSERT(distortion_pass_statistics_group, 0x528);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DISTORTION_PASS_STATISTICS_GROUP_H_INCLUDED
