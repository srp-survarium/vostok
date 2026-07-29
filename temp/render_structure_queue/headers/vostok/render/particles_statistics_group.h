////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_PARTICLES_STATISTICS_GROUP_H_INCLUDED
#define RENDER_PARTICLES_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_cpu_gpu;
struct vostok::render::statistics_group;
struct vostok::render::statistics_int;

namespace vostok {
namespace render {

struct particles_statistics_group : public statistics_group {
			explicit	particles_statistics_group	( pcstr group_name );
	inline				~particles_statistics_group	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0098 */	statistics_cpu_gpu		execute_time;
	/* 0x02e0 */	statistics_cpu_gpu		sprites_execute_time;
	/* 0x0528 */	statistics_cpu_gpu		beamtrails_execute_time;
	/* 0x0770 */	statistics_cpu_gpu		meshes_execute_time;
	/* 0x09b8 */	statistics_int			num_total_instances;
}; // struct particles_statistics_group

STATIC_SIZE_ASSERT(particles_statistics_group, 0xA78);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_PARTICLES_STATISTICS_GROUP_H_INCLUDED
