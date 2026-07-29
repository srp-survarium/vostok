////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKY_STATISTICS_GROUP_H_INCLUDED
#define RENDER_SKY_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_cpu_gpu;
struct vostok::render::statistics_group;

namespace vostok {
namespace render {

struct sky_statistics_group : public statistics_group {
			explicit	sky_statistics_group	( pcstr group_name );
	inline				~sky_statistics_group	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0098 */	statistics_cpu_gpu		execute_time;
}; // struct sky_statistics_group

STATIC_SIZE_ASSERT(sky_statistics_group, 0x2E0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKY_STATISTICS_GROUP_H_INCLUDED
