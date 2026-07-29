////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GBUFFER_STATISTICS_GROUP_H_INCLUDED
#define RENDER_GBUFFER_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_cpu_gpu;
struct vostok::render::statistics_group;

namespace vostok {
namespace render {

struct gbuffer_statistics_group : public statistics_group {
			explicit	gbuffer_statistics_group	( pcstr group_name );
	inline				~gbuffer_statistics_group	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0098 */	statistics_cpu_gpu		pre_pass_execute_time;
	/* 0x02e0 */	statistics_cpu_gpu		material_pass_execute_time;
}; // struct gbuffer_statistics_group

STATIC_SIZE_ASSERT(gbuffer_statistics_group, 0x528);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GBUFFER_STATISTICS_GROUP_H_INCLUDED
