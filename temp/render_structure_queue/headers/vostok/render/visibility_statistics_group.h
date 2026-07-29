////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VISIBILITY_STATISTICS_GROUP_H_INCLUDED
#define RENDER_VISIBILITY_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_float;
struct vostok::render::statistics_group;
struct vostok::render::statistics_int;

namespace vostok {
namespace render {

struct visibility_statistics_group : public statistics_group {
			explicit	visibility_statistics_group	( pcstr group_name );
	inline				~visibility_statistics_group( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0098 */	statistics_float	portal_culling_time;
	/* 0x0170 */	statistics_int		frustums_count;
	/* 0x0230 */	statistics_float	culling_time;
	/* 0x0308 */	statistics_float	models_updating_time;
	/* 0x03e0 */	statistics_int		num_draw_calls;
	/* 0x049c */	statistics_int		num_triangles;
	/* 0x0558 */	statistics_int		num_total_rendered_triangles;
	/* 0x0614 */	statistics_int		num_total_rendered_points;
	/* 0x06d0 */	statistics_int		num_surfaces;
	/* 0x078c */	statistics_int		num_lights;
	/* 0x0848 */	statistics_int		num_particle_instances;
	/* 0x0904 */	statistics_int		num_speedtree_instances;
	/* 0x09c0 */	statistics_int		num_environment_probes;
	/* 0x0a7c */	statistics_int		num_ambient_volumes;
	/* 0x0b38 */	statistics_int		num_occlusion_culled_surfaces;
	/* 0x0bf4 */	statistics_int		num_occlusion_culled_lights;
	/* 0x0cb0 */	statistics_int		num_occlusion_culled_grass_patches;
	/* 0x0d6c */	statistics_int		num_occlusion_culled_particle_instances;
	/* 0x0e28 */	statistics_int		num_occlusion_culled_decals;
	/* 0x0ee4 */	statistics_int		num_occlusion_culled_env_probes;
	/* 0x0fa0 */	statistics_int		num_occlusion_culled_portals;
	/* 0x105c */	statistics_int		num_occlusion_culled_ambient_volumes;
}; // struct visibility_statistics_group

STATIC_SIZE_ASSERT(visibility_statistics_group, 0x1118);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VISIBILITY_STATISTICS_GROUP_H_INCLUDED
