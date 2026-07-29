////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_DEBUG_STATISTICS_GROUP_H_INCLUDED
#define RENDER_DEBUG_STATISTICS_GROUP_H_INCLUDED

/* INCLUDES */
struct vostok::render::statistics_float;
struct vostok::render::statistics_group;
struct vostok::render::statistics_int;

namespace vostok {
namespace render {

struct debug_statistics_group : public statistics_group {
			explicit	debug_statistics_group	( pcstr group_name );
	inline				~debug_statistics_group	( ) { /* no source */ }

	/* 0x0000 */	/* statistics_group */
	/* 0x0094 */	statistics_int		avaliable_video_memory;
	/* 0x0150 */	statistics_int		texture_video_memory;
	/* 0x020c */	statistics_int		render_tergets_video_memory;
	/* 0x02c8 */	statistics_int		gbuffer_video_memory;
	/* 0x0384 */	statistics_int		num_dips_in_lpv;
	/* 0x0440 */	statistics_int		num_vertex_shader_changes;
	/* 0x04fc */	statistics_int		num_pixel_shader_changes;
	/* 0x05b8 */	statistics_int		num_vs_textures_changes;
	/* 0x0674 */	statistics_int		num_vs_constants_changes;
	/* 0x0730 */	statistics_int		num_vs_samplers_changes;
	/* 0x07ec */	statistics_int		num_ps_textures_changes;
	/* 0x08a8 */	statistics_int		num_ps_constants_changes;
	/* 0x0964 */	statistics_int		num_ps_samplers_changes;
	/* 0x0a20 */	statistics_int		num_input_layout_changes;
	/* 0x0ae0 */	statistics_float	textures_compression_duration;
	/* 0x0bb8 */	statistics_float	dxt_rt_tex_creation_duration;
	/* 0x0c90 */	statistics_float	cpu_textures_compression_duration;
	/* 0x0d68 */	statistics_int		gpu_num_compressed_textures;
	/* 0x0e24 */	statistics_int		cpu_num_compressed_textures;
}; // struct debug_statistics_group

STATIC_SIZE_ASSERT(debug_statistics_group, 0xEE0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_DEBUG_STATISTICS_GROUP_H_INCLUDED
