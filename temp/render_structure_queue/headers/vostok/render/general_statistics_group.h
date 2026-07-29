////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GENERAL_STATISTICS_GROUP_H_INCLUDED
#define RENDER_GENERAL_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_cpu_gpu;
struct vostok::render::statistics_float;
struct vostok::render::statistics_group;
struct vostok::render::statistics_int;

namespace vostok {
namespace render {

struct general_statistics_group : public statistics_group {
			explicit	general_statistics_group	( pcstr group_name );
	inline				~general_statistics_group	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0098 */	statistics_cpu_gpu		render_frame_time;
	/* 0x02e0 */	statistics_float		cpu_render_frame_time;
	/* 0x03b8 */	statistics_int			num_setted_shader_constants;
	/* 0x0474 */	statistics_int			fps;
	/* 0x0530 */	statistics_int			cpu_fps;
	/* 0x05f0 */	statistics_float		render_only_time;
}; // struct general_statistics_group

STATIC_SIZE_ASSERT(general_statistics_group, 0x6C8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GENERAL_STATISTICS_GROUP_H_INCLUDED
