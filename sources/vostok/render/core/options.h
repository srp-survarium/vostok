#ifndef VOSTOK_RENDER_CORE_OPTIONS_H_INCLUDED
#define VOSTOK_RENDER_CORE_OPTIONS_H_INCLUDED

#include <vostok/console_command.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/shader_macros.h>

namespace vostok {

namespace configs {
class binary_config_value;
} // namespace configs

namespace memory {
class reader;
} // namespace memory

namespace resources {
class queries_result;
} // namespace resources

namespace render {

enum enum_options_changes_result {
	ocr_need_nothing = 0x0001,
	ocr_need_resatart_application = 0x0002,
	ocr_need_reload_textures = 0x0004,
	ocr_need_reload_geometry = 0x0008,
	ocr_need_reload_shaders = 0x0010,
	ocr_need_recreate_samplers = 0x0020,
	ocr_need_update_vegetation = 0x0040,
	ocr_need_resize_window = 0x0080,
	ocr_need_recreate_device = 0x0100,
	ocr_need_reset_renderer = 0x0200,
	ocr_need_reset_postprocess = 0x0400,
	ocr_need_reset_lighting = 0x0800,
	ocr_need_reset_rain = 0x1000,
};

class options;

class render_cc {
public:
	render_cc(
		pcstr define_name,
		enum_options_changes_result changed_result
	);

	pcstr define_name( ) const { return m_define_name; }

private:
	friend class options;

	virtual bool fill_macro( shader_macro& out_macro ) const = 0;
	virtual bool is_changed( ) const = 0;

	enum_options_changes_result get_changes_result( ) const
	{
		return m_changes_result;
	}

	render_cc* render_next;
	pcstr m_define_name;
	enum_options_changes_result m_changes_result;
};

class render_cc_bool :
	public render_cc,
	public console_commands::cc_bool
{
public:
	render_cc_bool(
		pcstr name,
		enum_options_changes_result changed_result,
		pcstr define_name,
		bool& value,
		bool& prev_value,
		bool serializable,
		console_commands::command_type command_type
	);

	virtual ~render_cc_bool( ) { }
	virtual void execute( pcstr args );

private:
	virtual bool is_changed( ) const;
	virtual bool fill_macro( shader_macro& out_macro ) const;

	bool& m_prev_value;
};

class render_cc_float :
	public render_cc,
	public console_commands::cc_float
{
public:
	render_cc_float(
		pcstr name,
		enum_options_changes_result changed_result,
		pcstr define_name,
		float& value,
		float& prev_value,
		float min,
		float max,
		bool serializable,
		console_commands::command_type command_type
	);

	virtual ~render_cc_float( ) { }
	virtual void execute( pcstr args );

private:
	virtual bool is_changed( ) const;
	virtual bool fill_macro( shader_macro& out_macro ) const;

	float& m_prev_value;
};

class render_cc_u32 :
	public render_cc,
	public console_commands::cc_u32
{
public:
	render_cc_u32(
		pcstr name,
		enum_options_changes_result changed_result,
		pcstr define_name,
		u32& value,
		u32& prev_value,
		u32 min,
		u32 max,
		bool serializable,
		console_commands::command_type command_type
	);

	virtual ~render_cc_u32( ) { }
	virtual void execute( pcstr args );

private:
	virtual bool is_changed( ) const;
	virtual bool fill_macro( shader_macro& out_macro ) const;

	u32& m_prev_value;
};

class options : public quasi_singleton<options> {
public:
	struct optinos_table {
		float m_radiance_volume_scale;
		float m_lpv_flux_amplifier;
		float m_lpv_interreflection_contribution;
		float m_test_float_option;
		float m_lpv_occlusion_amplifier;
		float m_clouds_noise_octaves;
		float m_clouds_noise_frequency;
		float m_clouds_noise_amplitude;
		float m_clouds_noise_power;
		float m_clouds_height;
		float m_clouds_scale;
		float m_clouds_moving_speed;
		float m_clouds_scale_by_distance;
		float m_clouds_scale_xy;
		float m_fxaa_quality_subpix;
		float m_fxaa_quality_edge_threshold;
		float m_fxaa_quality_edge_threshold_min;
		float m_grass_lod1_distance;
		float m_grass_lod2_distance;
		float m_ssao_screen_ratio;
		float m_motion_blur_scale;
		float m_gamma_correction_factor;
		u32 m_ssao_num_samples;
		u32 m_clouds_num_evaluate_slices;
		u32 m_clouds_grid_width;
		u32 m_clouds_grid_height;
		u32 m_organic_irradiance_texture_size;
		u32 m_shadow_map_size;
		u32 m_spot_shadow_map_size;
		u32 m_shadow_quality;
		u32 m_light_propagation_volumes_rsm_size;
		u32 m_num_radiance_volume_cells;
		u32 m_num_propagate_iterations;
		u32 m_lpv_num_cascades;
		u32 m_lpv_refresh_once_per_frames;
		u32 m_num_test_lights;
		u32 m_hiz_occlusion_culling_width;
		u32 m_hiz_occlusion_culling_height;
		u32 m_hiz_occlusion_culling_framerate;
		u32 m_num_shadow_cascades;
		u32 m_cascaded_shadow_map_size;
		u32 m_num_max_light_instances;
		u32 m_texture_quality;
		u32 m_max_anisotropic;
		u32 m_monitor_index;
		u32 m_geometry_quality;
		u32 m_lighting_quality;
		u32 m_post_process_quality;
		u32 m_particles_quality;
		u32 m_motion_blur_quality;
		u32 m_shading_quality;
		u32 m_ambient_occlusion_quality;
		u32 m_antialiasing_method;
		u32 m_decorations_quality;
		u32 m_graphics_quality;
		u32 m_resolution_x;
		u32 m_resolution_y;
		bool m_fullscreen;
		bool m_vsync;
		bool m_enabled_g_stage;
		bool m_enabled_g_stage_pre_pass;
		bool m_enabled_g_stage_material_pass;
		bool m_enabled_decals_accumulate_stage;
		bool m_enabled_distortion_stage;
		bool m_enabled_sun_shadows_stage;
		bool m_enabled_sun_stage;
		bool m_enabled_lighting_stage;
		bool m_enabled_ambient_occlusion_stage;
		bool m_enabled_forward_lighting_stage;
		bool m_enabled_deferred_lighting_stage;
		bool m_enabled_forward_stage;
		bool m_enabled_particles_stage;
		bool m_enabled_post_process_stage;
		bool m_enabled_sky_box_stage;
		bool m_enabled_clouds_stage;
		bool m_enabled_light_propagation_volumes_stage;
		bool m_enabled_terrain_shadows;
		bool m_enabled_draw_terrain;
		bool m_enabled_draw_speedtree;
		bool m_enabled_draw_speedtree_billboards;
		bool m_enabled_draw_speedtree_branches;
		bool m_enabled_draw_speedtree_fronds;
		bool m_enabled_draw_speedtree_leafcards;
		bool m_enabled_draw_speedtree_leafmeshes;
		bool m_enabled_local_light_shadows;
		bool m_use_parallax;
		bool m_enabled_fxaa;
		bool m_enabled_mlaa;
		bool m_use_cpu_mlaa;
		bool m_enabled_sharpen;
		bool m_enabled_lpv_occluders;
		bool m_lpv_movable;
		bool m_lpv_gather_occluders_from_light_view;
		bool m_lpv_gather_occluders_from_camera_view;
		bool m_lpv_disable_rsm_generating;
		bool m_lpv_disable_rsm_downsampling;
		bool m_lpv_disable_vpl_injection;
		bool m_lpv_disable_gv_injection;
		bool m_lpv_disable_propagation;
		bool m_lpv_disable_lpv_lookup;
		bool m_lpv_use_specular_reflection;
		bool m_enabled_draw_models;
		bool m_clouds_allow_moving;
		bool m_clouds_debug_mode;
		bool m_use_god_rays;
		bool m_use_hiz_occlusion_culling;
		bool m_enabled_sky_sphere_stage;
		bool m_enabled_atmosphere_stage;
		bool m_ssao_use_filtering;
		bool m_ssao_use_temporal_filtering;
		bool m_use_temporal_antialiasing;
		bool m_use_motion_blur;
		bool m_draw_grass;
		bool m_enabled_volume_fog_stage;
		bool m_use_16bit_rt;
		bool m_use_shader_lods;
		bool m_update_shadows_every_frame;
		bool m_use_screenspace_reflections_mask;
		bool m_use_poisson_disc_shadow_filter;
		bool m_use_texture_streaming;
		bool m_use_motion_vectors_in_taa;
		bool m_use_vegetation_trample;
	};

public:
	options( );
	~options( ) { }

	void set_default_values( );
	void register_console_commands( );
	void save( pcstr ) { /* STATE[STUB] */ }
	void load( pcstr ) { /* STATE[STUB] */ }
	void begin_render_options_changing( long volatile* waiting_for );
	enum_options_changes_result end_render_options_changing(
		vector<fs_new::virtual_path_string>& out_changed_defines
	);
	void load_from_config( configs::binary_config_value const& config );
	fs_new::virtual_path_string get_current_configuration( )
	{
		// STATE[STUB]
		return fs_new::virtual_path_string( );
	}
	void save_current_configuration( ) { /* STATE[STUB] */ }
	void fill_global_macros( shader_defines_list& out_defines );

private:
	friend class render_cc;

	void load_impl( memory::reader& reader );
	void on_config_loaded( resources::queries_result& data );
	void on_config_loaded2( resources::queries_result& )
	{
		// STATE[STUB]
	}

	render_cc* first_render_command;

public:
	console_commands::console_command* first_command;
	console_commands::console_command* last_command;
	optinos_table current;
	optinos_table previous;
};

STATIC_SIZE_ASSERT( render_cc, 0x10 );
STATIC_SIZE_ASSERT( render_cc_bool, 0x60 );
STATIC_SIZE_ASSERT( render_cc_float, 0x68 );
STATIC_SIZE_ASSERT( render_cc_u32, 0x68 );
STATIC_SIZE_ASSERT( options::optinos_table, 0x128 );
STATIC_SIZE_ASSERT( options, 0x25c );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_OPTIONS_H_INCLUDED
