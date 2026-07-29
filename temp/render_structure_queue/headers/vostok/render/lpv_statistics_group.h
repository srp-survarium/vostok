////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LPV_STATISTICS_GROUP_H_INCLUDED
#define RENDER_LPV_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_cpu_gpu;
struct vostok::render::statistics_group;
struct vostok::render::statistics_int;

namespace vostok {
namespace render {

struct lpv_statistics_group : public statistics_group {
			explicit	lpv_statistics_group	( pcstr group_name );
	inline				~lpv_statistics_group	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0098 */	statistics_cpu_gpu		lpv_lookup_time;
	/* 0x02e0 */	statistics_cpu_gpu		propagation_time;
	/* 0x0528 */	statistics_cpu_gpu		gv_injection_time;
	/* 0x0770 */	statistics_cpu_gpu		vpl_injection_time;
	/* 0x09b8 */	statistics_cpu_gpu		rsm_downsample_time;
	/* 0x0c00 */	statistics_cpu_gpu		rsm_rendering_time;
	/* 0x0e48 */	statistics_int			num_dips;
	/* 0x0f04 */	statistics_int			num_dips_in_cascade_0;
	/* 0x0fc0 */	statistics_int			num_dips_in_cascade_1;
	/* 0x107c */	statistics_int			num_dips_in_cascade_2;
	/* 0x1138 */	statistics_int			num_clipped_dips;
}; // struct lpv_statistics_group

STATIC_SIZE_ASSERT(lpv_statistics_group, 0x11F8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LPV_STATISTICS_GROUP_H_INCLUDED
