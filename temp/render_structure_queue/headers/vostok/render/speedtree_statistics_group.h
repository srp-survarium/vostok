////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_STATISTICS_GROUP_H_INCLUDED
#define RENDER_SPEEDTREE_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_cpu_gpu;
struct vostok::render::statistics_float;
struct vostok::render::statistics_group;
struct vostok::render::statistics_int;

namespace vostok {
namespace render {

struct speedtree_statistics_group : public statistics_group {
			explicit	speedtree_statistics_group	( pcstr group_name );
	inline				~speedtree_statistics_group	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0098 */	statistics_cpu_gpu		render_time;
	/* 0x02e0 */	statistics_float		culling_time;
	/* 0x03b8 */	statistics_int			num_instances;
}; // struct speedtree_statistics_group

STATIC_SIZE_ASSERT(speedtree_statistics_group, 0x478);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_STATISTICS_GROUP_H_INCLUDED
