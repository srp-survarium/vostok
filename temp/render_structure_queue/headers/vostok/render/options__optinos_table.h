////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_OPTIONS_OPTINOS_TABLE_H_INCLUDED
#define RENDER_OPTIONS_OPTINOS_TABLE_H_INCLUDED

namespace vostok {
namespace render {

struct options::optinos_table {

	/* 0x0000 */	float		m_radiance_volume_scale;
	/* 0x0004 */	float		m_lpv_flux_amplifier;
	/* 0x0008 */	float		m_lpv_interreflection_contribution;
	/* 0x000c */	float		m_test_float_option;
	/* 0x0010 */	float		m_lpv_occlusion_amplifier;
	/* 0x0014 */	float		m_clouds_noise_octaves;
	/* 0x0018 */	float		m_clouds_noise_frequency;
	/* 0x001c */	float		m_clouds_noise_amplitude;
	/* 0x0020 */	float		m_clouds_noise_power;
	/* 0x0024 */	float		m_clouds_height;
	/* 0x0028 */	float		m_clouds_scale;
	/* 0x002c */	float		m_clouds_moving_speed;
	/* 0x0030 */	float		m_clouds_scale_by_distance;
	/* 0x0034 */	float		m_clouds_scale_xy;
	/* 0x0038 */	float		m_fxaa_quality_subpix;
	/* 0x003c */	float		m_fxaa_quality_edge_threshold;
	/* 0x0040 */	float		m_fxaa_quality_edge_threshold_min;
	/* 0x0044 */	float		m_grass_lod1_distance;
	/* 0x0048 */	float		m_grass_lod2_distance;
	/* 0x004c */	float		m_ssao_screen_ratio;
	/* 0x0050 */	float		m_motion_blur_scale;
	/* 0x0054 */	float		m_gamma_correction_factor;
	/* 0x0058 */	u32			m_ssao_num_samples;
	/* 0x005c */	u32			m_clouds_num_evaluate_slices;
	/* 0x0060 */	u32			m_clouds_grid_width;
	/* 0x0064 */	u32			m_clouds_grid_height;
	/* 0x0068 */	u32			m_organic_irradiance_texture_size;
	/* 0x006c */	u32			m_shadow_map_size;
	/* 0x0070 */	u32			m_spot_shadow_map_size;
	/* 0x0074 */	u32			m_shadow_quality;
	/* 0x0078 */	u32			m_light_propagation_volumes_rsm_size;
	/* 0x007c */	u32			m_num_radiance_volume_cells;
	/* 0x0080 */	u32			m_num_propagate_iterations;
	/* 0x0084 */	u32			m_lpv_num_cascades;
	/* 0x0088 */	u32			m_lpv_refresh_once_per_frames;
	/* 0x008c */	u32			m_num_test_lights;
	/* 0x0090 */	u32			m_hiz_occlusion_culling_width;
	/* 0x0094 */	u32			m_hiz_occlusion_culling_height;
	/* 0x0098 */	u32			m_hiz_occlusion_culling_framerate;
	/* 0x009c */	u32			m_num_shadow_cascades;
	/* 0x00a0 */	u32			m_cascaded_shadow_map_size;
	/* 0x00a4 */	u32			m_num_max_light_instances;
	/* 0x00a8 */	u32			m_texture_quality;
	/* 0x00ac */	u32			m_max_anisotropic;
	/* 0x00b0 */	u32			m_monitor_index;
	/* 0x00b4 */	u32			m_geometry_quality;
	/* 0x00b8 */	u32			m_lighting_quality;
	/* 0x00bc */	u32			m_post_process_quality;
	/* 0x00c0 */	u32			m_particles_quality;
	/* 0x00c4 */	u32			m_motion_blur_quality;
	/* 0x00c8 */	u32			m_shading_quality;
	/* 0x00cc */	u32			m_ambient_occlusion_quality;
	/* 0x00d0 */	u32			m_antialiasing_method;
	/* 0x00d4 */	u32			m_decorations_quality;
	/* 0x00d8 */	u32			m_graphics_quality;
	/* 0x00dc */	u32			m_resolution_x;
	/* 0x00e0 */	u32			m_resolution_y;
	/* 0x00e4 */	bool		m_fullscreen;
	/* 0x00e5 */	bool		m_vsync;
	/* 0x00e6 */	bool		m_enabled_g_stage;
	/* 0x00e7 */	bool		m_enabled_g_stage_pre_pass;
	/* 0x00e8 */	bool		m_enabled_g_stage_material_pass;
	/* 0x00e9 */	bool		m_enabled_decals_accumulate_stage;
	/* 0x00ea */	bool		m_enabled_distortion_stage;
	/* 0x00eb */	bool		m_enabled_sun_shadows_stage;
	/* 0x00ec */	bool		m_enabled_sun_stage;
	/* 0x00ed */	bool		m_enabled_lighting_stage;
	/* 0x00ee */	bool		m_enabled_ambient_occlusion_stage;
	/* 0x00ef */	bool		m_enabled_forward_lighting_stage;
	/* 0x00f0 */	bool		m_enabled_deferred_lighting_stage;
	/* 0x00f1 */	bool		m_enabled_forward_stage;
	/* 0x00f2 */	bool		m_enabled_particles_stage;
	/* 0x00f3 */	bool		m_enabled_post_process_stage;
	/* 0x00f4 */	bool		m_enabled_sky_box_stage;
	/* 0x00f5 */	bool		m_enabled_clouds_stage;
	/* 0x00f6 */	bool		m_enabled_light_propagation_volumes_stage;
	/* 0x00f7 */	bool		m_enabled_terrain_shadows;
	/* 0x00f8 */	bool		m_enabled_draw_terrain;
	/* 0x00f9 */	bool		m_enabled_draw_speedtree;
	/* 0x00fa */	bool		m_enabled_draw_speedtree_billboards;
	/* 0x00fb */	bool		m_enabled_draw_speedtree_branches;
	/* 0x00fc */	bool		m_enabled_draw_speedtree_fronds;
	/* 0x00fd */	bool		m_enabled_draw_speedtree_leafcards;
	/* 0x00fe */	bool		m_enabled_draw_speedtree_leafmeshes;
	/* 0x00ff */	bool		m_enabled_local_light_shadows;
	/* 0x0100 */	bool		m_use_parallax;
	/* 0x0101 */	bool		m_enabled_fxaa;
	/* 0x0102 */	bool		m_enabled_mlaa;
	/* 0x0103 */	bool		m_use_cpu_mlaa;
	/* 0x0104 */	bool		m_enabled_sharpen;
	/* 0x0105 */	bool		m_enabled_lpv_occluders;
	/* 0x0106 */	bool		m_lpv_movable;
	/* 0x0107 */	bool		m_lpv_gather_occluders_from_light_view;
	/* 0x0108 */	bool		m_lpv_gather_occluders_from_camera_view;
	/* 0x0109 */	bool		m_lpv_disable_rsm_generating;
	/* 0x010a */	bool		m_lpv_disable_rsm_downsampling;
	/* 0x010b */	bool		m_lpv_disable_vpl_injection;
	/* 0x010c */	bool		m_lpv_disable_gv_injection;
	/* 0x010d */	bool		m_lpv_disable_propagation;
	/* 0x010e */	bool		m_lpv_disable_lpv_lookup;
	/* 0x010f */	bool		m_lpv_use_specular_reflection;
	/* 0x0110 */	bool		m_enabled_draw_models;
	/* 0x0111 */	bool		m_clouds_allow_moving;
	/* 0x0112 */	bool		m_clouds_debug_mode;
	/* 0x0113 */	bool		m_use_god_rays;
	/* 0x0114 */	bool		m_use_hiz_occlusion_culling;
	/* 0x0115 */	bool		m_enabled_sky_sphere_stage;
	/* 0x0116 */	bool		m_enabled_atmosphere_stage;
	/* 0x0117 */	bool		m_ssao_use_filtering;
	/* 0x0118 */	bool		m_ssao_use_temporal_filtering;
	/* 0x0119 */	bool		m_use_temporal_antialiasing;
	/* 0x011a */	bool		m_use_motion_blur;
	/* 0x011b */	bool		m_draw_grass;
	/* 0x011c */	bool		m_enabled_volume_fog_stage;
	/* 0x011d */	bool		m_use_16bit_rt;
	/* 0x011e */	bool		m_use_shader_lods;
	/* 0x011f */	bool		m_update_shadows_every_frame;
	/* 0x0120 */	bool		m_use_screenspace_reflections_mask;
	/* 0x0121 */	bool		m_use_poisson_disc_shadow_filter;
	/* 0x0122 */	bool		m_use_texture_streaming;
	/* 0x0123 */	bool		m_use_motion_vectors_in_taa;
	/* 0x0124 */	bool		m_use_vegetation_trample;
}; // struct options::optinos_table

STATIC_SIZE_ASSERT(options::optinos_table, 0x128);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_OPTIONS_OPTINOS_TABLE_H_INCLUDED
