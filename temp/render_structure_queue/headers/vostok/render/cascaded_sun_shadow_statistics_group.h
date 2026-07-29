////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CASCADED_SUN_SHADOW_STATISTICS_GROUP_H_INCLUDED
#define RENDER_CASCADED_SUN_SHADOW_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_cpu_gpu;
struct vostok::render::statistics_group;
struct vostok::render::statistics_int;

namespace vostok {
namespace render {

struct cascaded_sun_shadow_statistics_group : public statistics_group {
			explicit	cascaded_sun_shadow_statistics_group( pcstr group_name );
	inline				~cascaded_sun_shadow_statistics_group( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0098 */	statistics_cpu_gpu		execute_time_cascade_1;
	/* 0x02e0 */	statistics_cpu_gpu		execute_time_cascade_2;
	/* 0x0528 */	statistics_cpu_gpu		execute_time_cascade_3;
	/* 0x0770 */	statistics_cpu_gpu		execute_time_cascade_4;
	/* 0x09b8 */	statistics_int			num_dips_cascade_1;
	/* 0x0a74 */	statistics_int			num_dips_cascade_2;
	/* 0x0b30 */	statistics_int			num_dips_cascade_3;
	/* 0x0bec */	statistics_int			num_dips_cascade_4;
	/* 0x0ca8 */	statistics_int			num_dips;
	/* 0x0d64 */	statistics_int			num_clipped_dips;
	/* 0x0e20 */	statistics_int			num_triangles;
}; // struct cascaded_sun_shadow_statistics_group

STATIC_SIZE_ASSERT(cascaded_sun_shadow_statistics_group, 0xEE0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CASCADED_SUN_SHADOW_STATISTICS_GROUP_H_INCLUDED
