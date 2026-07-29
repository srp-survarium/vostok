////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LIGHTS_STATISTICS_GROUP_H_INCLUDED
#define RENDER_LIGHTS_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_cpu_gpu;
struct vostok::render::statistics_group;

namespace vostok {
namespace render {

struct lights_statistics_group : public statistics_group {
			explicit	lights_statistics_group	( pcstr group_name );
	inline				~lights_statistics_group( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0098 */	statistics_cpu_gpu		accumulate_lighting_time;
	/* 0x02e0 */	statistics_cpu_gpu		forward_lighting_time;
	/* 0x0528 */	statistics_cpu_gpu		shadow_map_time;
}; // struct lights_statistics_group

STATIC_SIZE_ASSERT(lights_statistics_group, 0x770);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LIGHTS_STATISTICS_GROUP_H_INCLUDED
