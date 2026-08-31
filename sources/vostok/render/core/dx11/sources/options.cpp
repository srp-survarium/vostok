#include "pch.h"
#include <vostok/math_int2.h>
#include <vostok/render/core/options.h>
#include <vostok/console_command.h>
#include <vostok/console_command_processor.h>

namespace vostok {
namespace render {

static fixed_string<16> s_r_resolution_value = "1280x720";
static console_commands::cc_string s_r_resolution_cc(
	"r_resolution",
	s_r_resolution_value.c_str( ),
	s_r_resolution_value.get_buffer_size( ),
	true,
	console_commands::command_type_user_specific
);

static float s_uro_fov_value = 65.f;
static console_commands::cc_float s_uro_fov_cc( "uro_fov", s_uro_fov_value, 0.0f, 100.0f, true, console_commands::command_type_user_specific );

render_cc::render_cc(
	pcstr define_name,
	enum_options_changes_result changed_result
)
	: m_define_name( define_name ),
	  m_changes_result( changed_result )
{
	render_next		= options::ref().first_render_command;
	options::ref().first_render_command = this;
}

render_cc_bool::render_cc_bool(
	pcstr name,
	enum_options_changes_result changed_result,
	pcstr define_name,
	bool& value,
	bool& prev_value,
	bool serializable,
	console_commands::command_type const command_type
)
	: render_cc( define_name, changed_result ),
	  console_commands::cc_bool(
		name,
		value,
		serializable,
		command_type
	  ),
	  m_prev_value( prev_value )
{
}

void render_cc_bool::execute( pcstr args )
{
	m_prev_value = cc_bool::m_value;
	cc_bool::execute( args );
}

bool render_cc_bool::is_changed( ) const
{
	return m_prev_value != cc_bool::m_value;
}

bool render_cc_bool::fill_macro( shader_macro& out_macro ) const
{
	if (define_name())
	{
		out_macro.definition = cc_bool::m_value ? "1" : "0";
		out_macro.name		 = define_name();
		return true;
	}
	else
		return false;
}

render_cc_float::render_cc_float(
	pcstr name,
	enum_options_changes_result changed_result,
	pcstr define_name,
	float& value,
	float& prev_value,
	float const min,
	float const max,
	bool serializable,
	console_commands::command_type const command_type
)
	: render_cc( define_name, changed_result ),
	  console_commands::cc_float(
		name,
		value,
		min,
		max,
		serializable,
		command_type
	  ),
	  m_prev_value( prev_value )
{
}

void render_cc_float::execute( pcstr args )
{
	m_prev_value = cc_float::m_value;
	cc_float::execute( args );
}

bool render_cc_float::is_changed( ) const
{
	return m_prev_value != cc_float::m_value;
}

bool render_cc_float::fill_macro( shader_macro& out_macro ) const
{
	if (define_name())
	{
		out_macro.definition.assignf("%f", cc_float::m_value);
		out_macro.name		 = define_name();
		return true;
	}
	else
		return false;
}

render_cc_u32::render_cc_u32(
	pcstr name,
	enum_options_changes_result changed_result,
	pcstr define_name,
	u32& value,
	u32& prev_value,
	u32 const min,
	u32 const max,
	bool serializable,
	console_commands::command_type const command_type
)
	: render_cc( define_name, changed_result ),
	  console_commands::cc_u32(
		name,
		value,
		min,
		max,
		serializable,
		command_type
	  ),
	  m_prev_value( prev_value )
{
}

void render_cc_u32::execute( pcstr args )
{
	m_prev_value = cc_u32::m_value;
	cc_u32::execute( args );
}

bool render_cc_u32::is_changed( ) const
{
	return m_prev_value != cc_u32::m_value;
}

bool render_cc_u32::fill_macro( shader_macro& out_macro ) const
{
	if (define_name())
	{
		if ((strings::equal(define_name(), "GLOBAL_SHADOWMAP_QUALITY") && cc_u32::m_value != 0 && cc_u32::m_value != 3) || (strings::equal(define_name(), "GLOBAL_LIGHTING_QUALITY") && cc_u32::m_value != 0 && cc_u32::m_value != 3) || (strings::equal(define_name(), "GLOBAL_SHADING_QUALITY") && cc_u32::m_value != 0 && cc_u32::m_value != 3) || (strings::equal(define_name(), "GLOBAL_POST_PROCESS_QUALITY") && cc_u32::m_value != 0 && cc_u32::m_value != 3))
			out_macro.definition.assignf("%d", 3);
		else
			out_macro.definition.assignf("%d", cc_u32::m_value);
		out_macro.name		 = define_name();
		return true;
	}
	else
		return false;
}

options::options( )
	: first_render_command( 0 ),
	  first_command( 0 ),
	  last_command( 0 )
{
	register_console_commands();
	set_default_values();
}

void options::set_default_values( )
{
	current.m_radiance_volume_scale = 10.0f;
	current.m_lpv_flux_amplifier = 0.17f;
	current.m_lpv_interreflection_contribution = 0.4f;
	current.m_test_float_option = 1.0f;
	current.m_lpv_occlusion_amplifier = 1.0f;
	current.m_clouds_noise_octaves = 8.0f;
	current.m_clouds_noise_frequency = 100.0f;
	current.m_clouds_noise_amplitude = 0.5f;
	current.m_clouds_noise_power = 1.0f;
	current.m_clouds_height = 0.0f;
	current.m_clouds_scale = 1.0f;
	current.m_clouds_moving_speed = 1.0f;
	current.m_clouds_scale_by_distance = 0.0f;
	current.m_clouds_scale_xy = 5.0f;
	current.m_fxaa_quality_subpix = 0.25f;
	current.m_fxaa_quality_edge_threshold = 0.166f;
	current.m_fxaa_quality_edge_threshold_min = 0.03f;
	current.m_grass_lod1_distance = 40.0f;
	current.m_grass_lod2_distance = 60.0f;
	current.m_ssao_screen_ratio = 0.5f;
	current.m_motion_blur_scale = 1.0f;
	current.m_gamma_correction_factor = 1.0f;
	current.m_ssao_num_samples = 24;
	current.m_clouds_num_evaluate_slices = 128;
	current.m_clouds_grid_width = 128;
	current.m_clouds_grid_height = 8;
	current.m_organic_irradiance_texture_size = 1024;
	current.m_shadow_map_size = 1024;
	current.m_spot_shadow_map_size = 1024;
	current.m_shadow_quality = uro_shadow_quality_ultra;
	current.m_light_propagation_volumes_rsm_size = 128;
	current.m_num_radiance_volume_cells = 32;
	current.m_num_propagate_iterations = 8;
	current.m_lpv_num_cascades = 3;
	current.m_lpv_refresh_once_per_frames = 1;
	current.m_num_test_lights = 1024;
	current.m_hiz_occlusion_culling_width = 512;
	current.m_hiz_occlusion_culling_height = 256;
	current.m_hiz_occlusion_culling_framerate = 8;
	current.m_num_shadow_cascades = 4;
	current.m_cascaded_shadow_map_size = 1024;
	current.m_num_max_light_instances = 128;
	current.m_texture_quality = 2;
	current.m_max_anisotropic = uro_anisotrophic_filter_16x;
	current.m_monitor_index = 0;
	current.m_geometry_quality = uro_geometry_quality_high;
	current.m_lighting_quality = uro_lighting_quality_ultra;
	current.m_post_process_quality = uro_post_process_quality_high;
	current.m_particles_quality = uro_particles_quality_high;
	current.m_motion_blur_quality = uro_motion_blur_quality_high;
	current.m_shading_quality = uro_shading_quality_ultra;
	current.m_ambient_occlusion_quality = uro_ambient_occlusion_quality_ssao;
	current.m_antialiasing_method = uro_antialiasing_method_2xTAA;
	current.m_decorations_quality = uro_decorations_quality_high;
	current.m_graphics_quality = uro_graphics_quality_ultra;
	current.m_resolution_x = 1280;
	current.m_resolution_y = 720;
	current.m_fullscreen = false;
	current.m_vsync = false;
	current.m_enabled_g_stage = true;
	current.m_enabled_g_stage_pre_pass = true;
	current.m_enabled_g_stage_material_pass = true;
	current.m_enabled_decals_accumulate_stage = true;
	current.m_enabled_distortion_stage = true;
	current.m_enabled_sun_shadows_stage = true;
	current.m_enabled_sun_stage = true;
	current.m_enabled_lighting_stage = true;
	current.m_enabled_ambient_occlusion_stage = true;
	current.m_enabled_forward_lighting_stage = true;
	current.m_enabled_deferred_lighting_stage = true;
	current.m_enabled_forward_stage = true;
	current.m_enabled_particles_stage = true;
	current.m_enabled_post_process_stage = true;
	current.m_enabled_sky_box_stage = false;
	current.m_enabled_clouds_stage = true;
	current.m_enabled_light_propagation_volumes_stage = false;
	current.m_enabled_terrain_shadows = true;
	current.m_enabled_draw_terrain = true;
	current.m_enabled_draw_speedtree = true;
	current.m_enabled_draw_speedtree_billboards = true;
	current.m_enabled_draw_speedtree_branches = true;
	current.m_enabled_draw_speedtree_fronds = true;
	current.m_enabled_draw_speedtree_leafcards = true;
	current.m_enabled_draw_speedtree_leafmeshes = true;
	current.m_enabled_local_light_shadows = true;
	current.m_use_parallax = true;
	current.m_enabled_fxaa = true;
	current.m_enabled_mlaa = true;
	current.m_use_cpu_mlaa = false;
	current.m_enabled_sharpen = true;
	current.m_enabled_lpv_occluders = true;
	current.m_lpv_movable = true;
	current.m_lpv_gather_occluders_from_light_view = true;
	current.m_lpv_gather_occluders_from_camera_view = false;
	current.m_lpv_disable_rsm_generating = false;
	current.m_lpv_disable_rsm_downsampling = false;
	current.m_lpv_disable_vpl_injection = false;
	current.m_lpv_disable_gv_injection = false;
	current.m_lpv_disable_propagation = false;
	current.m_lpv_disable_lpv_lookup = false;
	current.m_lpv_use_specular_reflection = false;
	current.m_enabled_draw_models = true;
	current.m_clouds_allow_moving = true;
	current.m_clouds_debug_mode = false;
	current.m_use_god_rays = false;
	current.m_use_hiz_occlusion_culling = true;
	current.m_enabled_sky_sphere_stage = false;
	current.m_enabled_atmosphere_stage = true;
	current.m_ssao_use_filtering = true;
	current.m_ssao_use_temporal_filtering = true;
	current.m_use_temporal_antialiasing = true;
	current.m_use_motion_blur = true;
	current.m_draw_grass = true;
	current.m_enabled_volume_fog_stage = true;
	current.m_use_16bit_rt = false;
	current.m_use_shader_lods = true;
	current.m_update_shadows_every_frame = true;
	current.m_use_screenspace_reflections_mask = false;
	current.m_use_poisson_disc_shadow_filter = false;
	current.m_use_texture_streaming = true;
	current.m_use_motion_vectors_in_taa = true;
	current.m_use_vegetation_trample = false;
	previous = current;
}

fs_new::virtual_path_string options::get_current_configuration( )
{
	console_commands::cc_string* command = static_cast_checked<console_commands::cc_string*>(vostok::console_commands::find("r_current_render_configuration"));
	console_commands::console_command::status_str		status;
	command->status						(status);
	fs_new::virtual_path_string			file_name;
	file_name.append					(status);

	return file_name;
}

static math::uint2 parse_resolution( pcstr in_str )
{
	char xy_str[16];

	if ( in_str && in_str[0] )
	{
		pcstr const height_str = strings::get_token( in_str, xy_str, strings::length( in_str ), 'x' );
		if ( height_str )
		{
			u32 const width = atoi( xy_str );
			u32 const height = atoi( height_str );
			if ( width && height )
				return math::uint2( width, height );
		}
	}

	return math::uint2( 1280, 720 );
}

string256 s_current_render_configuration = "default";

void options::register_console_commands( )
{
	using namespace console_commands;

	static render_cc_bool	enabled_g_stage_cc					("r_enabled_g_stage",					ocr_need_reset_renderer, 0, current.m_enabled_g_stage,					previous.m_enabled_g_stage,					true, command_type_engine_internal);
	static render_cc_bool	enabled_g_stage_pre_pass_cc			("r_enabled_g_stage_pre_pass",			ocr_need_reset_renderer, 0, current.m_enabled_g_stage_pre_pass,		previous.m_enabled_g_stage_pre_pass,		true, command_type_engine_internal);
	static render_cc_bool	enabled_g_stage_material_pass_cc	("r_enabled_g_stage_material_pass",		ocr_need_reset_renderer, 0, current.m_enabled_g_stage_material_pass,	previous.m_enabled_g_stage_material_pass,	true, command_type_engine_internal);
	static render_cc_bool	enabled_decals_accumulate_stage_cc	("r_enabled_decals_accumulate_stage",	ocr_need_reset_renderer, 0, current.m_enabled_decals_accumulate_stage,	previous.m_enabled_decals_accumulate_stage,	true, command_type_engine_internal);
	static render_cc_bool	enabled_distortion_stage_cc			("r_enabled_distortion_stage",			ocr_need_reset_renderer, 0, current.m_enabled_distortion_stage,		previous.m_enabled_distortion_stage,		true, command_type_engine_internal);
	static render_cc_bool	enabled_sun_shadows_stage_cc		("r_enabled_sun_shadows_stage",			ocr_need_reset_renderer, 0, current.m_enabled_sun_shadows_stage,		previous.m_enabled_sun_shadows_stage,		true, command_type_engine_internal);
	static render_cc_bool	enabled_sun_stage_cc				("r_enabled_sun_stage",					ocr_need_reset_renderer, 0, current.m_enabled_sun_stage,				previous.m_enabled_sun_stage,				true, command_type_engine_internal);
	static render_cc_bool	enabled_lighting_stage_cc			("r_enabled_lighting_stage",			ocr_need_reset_renderer, 0, current.m_enabled_lighting_stage,			previous.m_enabled_lighting_stage,			true, command_type_engine_internal);
	static render_cc_bool	enabled_forward_lighting_stage_cc	("r_enabled_forward_lighting_stage",	ocr_need_reset_renderer, 0, current.m_enabled_forward_lighting_stage,	previous.m_enabled_forward_lighting_stage,	true, command_type_engine_internal);
	static render_cc_bool	enabled_deferred_lighting_stage_cc	("r_enabled_deferred_lighting_stage",	ocr_need_reset_renderer, 0, current.m_enabled_deferred_lighting_stage,	previous.m_enabled_deferred_lighting_stage,	true, command_type_engine_internal);
	static render_cc_bool	enabled_forward_stage_cc			("r_enabled_forward_stage",				ocr_need_reset_renderer, 0, current.m_enabled_forward_stage,			previous.m_enabled_forward_stage,			true, command_type_engine_internal);
	static render_cc_bool	enabled_particles_stage_cc			("r_enabled_particles_stage",			ocr_need_reset_renderer, 0, current.m_enabled_particles_stage,			previous.m_enabled_particles_stage,			true, command_type_engine_internal);
	static render_cc_bool	enabled_post_process_stage_cc		("r_enabled_post_process_stage",		ocr_need_reset_renderer, 0, current.m_enabled_post_process_stage,		previous.m_enabled_post_process_stage,		true, command_type_engine_internal);
	static render_cc_bool	enabled_sky_box_stage_cc			("r_enabled_sky_box_stage",				ocr_need_reset_renderer, 0, current.m_enabled_sky_box_stage,			previous.m_enabled_sky_box_stage,			true, command_type_engine_internal);
	static render_cc_bool	enabled_ambient_occlusion_stage_cc	("r_enabled_ambient_occlusion_stage",	ocr_need_reset_renderer, 0, current.m_enabled_ambient_occlusion_stage,	previous.m_enabled_ambient_occlusion_stage,	true, command_type_engine_internal);
	static render_cc_bool	enabled_clouds_stage_cc				("r_enabled_clouds_stage",				ocr_need_reset_renderer, 0, current.m_enabled_clouds_stage,			previous.m_enabled_clouds_stage,			true, command_type_engine_internal);

	// Terrain parameters.
	static render_cc_bool	enabled_draw_terrain_cc				("r_enabled_draw_terrain",				ocr_need_nothing, 0, current.m_enabled_draw_terrain,			previous.m_enabled_draw_terrain,			true, command_type_engine_internal);
	static render_cc_bool	enabled_terrain_shadows_cc			("r_enabled_terrain_shadows",			ocr_need_nothing, 0, current.m_enabled_terrain_shadows,			previous.m_enabled_terrain_shadows,			true, command_type_engine_internal);

	// SpeedTree parameters.
	static render_cc_bool	enabled_draw_speedtree_cc			("r_enabled_draw_speedtree",			ocr_need_nothing, 0, current.m_enabled_draw_speedtree,			previous.m_enabled_draw_speedtree,			true, command_type_engine_internal);
	static render_cc_bool	enabled_draw_speedtree_branches_cc	("r_enabled_draw_speedtree_branches",	ocr_need_nothing, 0, current.m_enabled_draw_speedtree_branches,	previous.m_enabled_draw_speedtree_branches,	true, command_type_engine_internal);
	static render_cc_bool	enabled_draw_speedtree_fronds_cc	("r_enabled_draw_speedtree_fronds",		ocr_need_nothing, 0, current.m_enabled_draw_speedtree_fronds,	previous.m_enabled_draw_speedtree_fronds,	true, command_type_engine_internal);
	static render_cc_bool	enabled_draw_speedtree_leafmeshes_cc("r_enabled_draw_speedtree_leafmeshes",	ocr_need_nothing, 0, current.m_enabled_draw_speedtree_leafmeshes, previous.m_enabled_draw_speedtree_leafmeshes, true, command_type_engine_internal);
	static render_cc_bool	enabled_draw_speedtree_leafcards_cc	("r_enabled_draw_speedtree_leafcards",	ocr_need_nothing, 0, current.m_enabled_draw_speedtree_leafcards, previous.m_enabled_draw_speedtree_leafcards,	true, command_type_engine_internal);
	static render_cc_bool	enabled_draw_speedtree_billboards_cc("r_enabled_draw_speedtree_billboards",	ocr_need_nothing, 0, current.m_enabled_draw_speedtree_billboards, previous.m_enabled_draw_speedtree_billboards, true, command_type_engine_internal);

	// Models parameters.
	static render_cc_bool	enabled_draw_models_cc				("r_enabled_draw_models",				ocr_need_nothing, 0, current.m_enabled_draw_models,				previous.m_enabled_draw_models,				true, command_type_engine_internal);

	static render_cc_bool	enabled_sharpen_cc					("r_enabled_sharpen",					ocr_need_nothing, 0, current.m_enabled_sharpen,					previous.m_enabled_sharpen,					true, command_type_engine_internal);
	static render_cc_bool	enabled_fxaa_cc						("r_enabled_fxaa",						ocr_need_nothing, 0, current.m_enabled_fxaa,					previous.m_enabled_fxaa,					true, command_type_engine_internal);
	static render_cc_bool	enabled_mlaa_cc						("r_enabled_mlaa",						ocr_need_nothing, 0, current.m_enabled_mlaa,					previous.m_enabled_mlaa,					true, command_type_engine_internal);
	static render_cc_bool	use_cpu_mlaa_cc						("r_use_cpu_mlaa",						ocr_need_nothing, 0, current.m_use_cpu_mlaa,					previous.m_use_cpu_mlaa,					false, command_type_engine_internal);

	// Lighting stage parameters.
	static render_cc_bool	enabled_local_light_shadows_cc		("r_enabled_local_light_shadows",		ocr_need_nothing, 0, current.m_enabled_local_light_shadows,		previous.m_enabled_local_light_shadows,		true, command_type_engine_internal);

	static render_cc_bool	use_parallax_cc						("r_use_parallax",						ocr_need_reset_renderer, "GLOBAL_ALLOW_STEEPPARALLAX",				current.m_use_parallax, previous.m_use_parallax, true, command_type_engine_internal);

	static render_cc_u32	organic_irradiance_texture_size_cc	("r_organic_irradiance_texture_size",	ocr_need_nothing, "GLOBAL_ORGANIC_IRRADIANCE_TEXTURE_SIZE",	current.m_organic_irradiance_texture_size, previous.m_organic_irradiance_texture_size, 128, 2048, true, command_type_engine_internal);
	static render_cc_u32	shadow_map_size_cc					("r_shadow_map_size",					ocr_need_reset_lighting, "GLOBAL_SHADOWMAP_SIZE",					current.m_shadow_map_size, previous.m_shadow_map_size, 128, 2048, true, command_type_engine_internal);
	static render_cc_u32	spot_shadow_map_size_cc				("r_spot_shadow_map_size",				ocr_need_reset_lighting, "GLOBAL_SPOT_SHADOWMAP_SIZE",				current.m_spot_shadow_map_size, previous.m_spot_shadow_map_size, 128, 2048, true, command_type_engine_internal);
	static render_cc_u32	shadow_quality_cc					("r_shadow_quality",					ocr_need_reset_lighting, "GLOBAL_SHADOWMAP_QUALITY",				current.m_shadow_quality, previous.m_shadow_quality, 0, 3, true, command_type_engine_internal);

	// Light Propagation Volumes stage parameters.
	static render_cc_bool	enabled_light_propagation_volumes_stage_cc	("r_enabled_light_propagation_volumes_stage",	ocr_need_reset_renderer, 0, current.m_enabled_light_propagation_volumes_stage, previous.m_enabled_light_propagation_volumes_stage, true, command_type_engine_internal);
	static render_cc_u32	light_propagation_volumes_rsm_size_cc		("r_light_propagation_volumes_rsm_size",		ocr_need_reset_renderer, 0, current.m_light_propagation_volumes_rsm_size, previous.m_light_propagation_volumes_rsm_size, 1, 2048, true, command_type_engine_internal);
	static render_cc_u32	num_radiance_volume_cells_cc				("r_num_radiance_volume_cells",					ocr_need_reset_renderer, 0, current.m_num_radiance_volume_cells,	previous.m_num_radiance_volume_cells,	8, 128, true, command_type_engine_internal);
	static render_cc_u32	num_propagate_iterations_cc					("r_num_propagate_iterations",					ocr_need_reset_renderer, 0, current.m_num_propagate_iterations,	previous.m_num_propagate_iterations,	0, 32, true, command_type_engine_internal);
	static render_cc_float	radiance_volume_scale_cc					("r_radiance_volume_scale",						ocr_need_reset_renderer, 0, current.m_radiance_volume_scale,		previous.m_radiance_volume_scale,		1.0f, 10000.0f, true, command_type_engine_internal);

	static render_cc_float	lpv_flux_amplifier_cc						("r_lpv_flux_amplifier",						ocr_need_nothing, 0, current.m_lpv_flux_amplifier,			previous.m_lpv_flux_amplifier,			0.0f, 100.0f, true, command_type_engine_internal);
	static render_cc_float	lpv_interreflection_contribution_cc			("r_lpv_interreflection_contribution",			ocr_need_nothing, 0, current.m_lpv_interreflection_contribution, previous.m_lpv_interreflection_contribution, 0.0f, 100.0f, true, command_type_engine_internal);
	static render_cc_bool	lpv_movable_cc								("r_lpv_movable",								ocr_need_nothing, 0, current.m_lpv_movable,					previous.m_lpv_movable,					true, command_type_engine_internal);

	static render_cc_u32	lpv_num_cascades_cc							("r_lpv_num_cascades",							ocr_need_reset_renderer, 0, current.m_lpv_num_cascades,			previous.m_lpv_num_cascades,			1, 4, true, command_type_engine_internal);

	static render_cc_bool	lpv_gather_occluders_from_light_view_cc		("r_lpv_gather_occluders_from_light_view",		ocr_need_nothing, 0, current.m_lpv_gather_occluders_from_light_view, previous.m_lpv_gather_occluders_from_light_view, true, command_type_engine_internal);
	static render_cc_bool	lpv_gather_occluders_from_camera_view_cc	("r_lpv_gather_occluders_from_camera_view",		ocr_need_nothing, 0, current.m_lpv_gather_occluders_from_camera_view, previous.m_lpv_gather_occluders_from_camera_view, true, command_type_engine_internal);

	static render_cc_bool	lpv_disable_rsm_generating_cc				("r_lpv_disable_rsm_generating",				ocr_need_nothing, 0, current.m_lpv_disable_rsm_generating,	previous.m_lpv_disable_rsm_generating,	false, command_type_engine_internal);
	static render_cc_bool	lpv_disable_rsm_downsampling_cc				("r_lpv_disable_rsm_downsampling",				ocr_need_nothing, 0, current.m_lpv_disable_rsm_downsampling, previous.m_lpv_disable_rsm_downsampling, false, command_type_engine_internal);
	static render_cc_bool	lpv_disable_vpl_injection_cc				("r_lpv_disable_vpl_injection",					ocr_need_nothing, 0, current.m_lpv_disable_vpl_injection,	previous.m_lpv_disable_vpl_injection,	false, command_type_engine_internal);
	static render_cc_bool	lpv_disable_gv_injection_cc					("r_lpv_disable_gv_injection",					ocr_need_nothing, 0, current.m_lpv_disable_gv_injection,	previous.m_lpv_disable_gv_injection,	false, command_type_engine_internal);
	static render_cc_bool	lpv_disable_propagation_cc					("r_lpv_disable_propagation",					ocr_need_nothing, 0, current.m_lpv_disable_propagation,		previous.m_lpv_disable_propagation,		false, command_type_engine_internal);
	static render_cc_bool	lpv_disable_lpv_lookup_cc					("r_lpv_disable_lpv_lookup",					ocr_need_nothing, 0, current.m_lpv_disable_lpv_lookup,		previous.m_lpv_disable_lpv_lookup,		false, command_type_engine_internal);

	static render_cc_u32	lpv_refresh_once_per_frames_cc				("r_lpv_refresh_once_per_frames",				ocr_need_nothing, 0, current.m_lpv_refresh_once_per_frames,	previous.m_lpv_refresh_once_per_frames,	1, 1024, true, command_type_engine_internal);
	static render_cc_float	lpv_occlusion_amplifier_cc					("r_lpv_occlusion_amplifier",					ocr_need_nothing, 0, current.m_lpv_occlusion_amplifier,		previous.m_lpv_occlusion_amplifier,		0.0f, 100.0f, true, command_type_engine_internal);
	static render_cc_bool	lpv_use_specular_reflection_cc				("r_lpv_use_specular_reflection",				ocr_need_nothing, 0, current.m_lpv_use_specular_reflection,	previous.m_lpv_use_specular_reflection,	true, command_type_engine_internal);

	static render_cc_u32	num_test_lights_cc							("r_num_test_lights",							ocr_need_nothing, 0, current.m_num_test_lights,				previous.m_num_test_lights,				16, 1024 * 1024, true, command_type_engine_internal);
	static render_cc_float clouds_noise_octaves_cc("r_clouds_noise_octaves", ocr_need_nothing, 0, current.m_clouds_noise_octaves, previous.m_clouds_noise_octaves, 0.0f, 10000.0f, true, command_type_engine_internal);
	static render_cc_float clouds_noise_frequency_cc("r_clouds_noise_frequency", ocr_need_nothing, 0, current.m_clouds_noise_frequency, previous.m_clouds_noise_frequency, 0.0f, 10000000.0f, true, command_type_engine_internal);
	static render_cc_float clouds_noise_amplitude_cc("r_clouds_noise_amplitude", ocr_need_nothing, 0, current.m_clouds_noise_amplitude, previous.m_clouds_noise_amplitude, -10000.0f, 10000000.0f, true, command_type_engine_internal);
	static render_cc_float clouds_noise_power_cc("r_clouds_noise_power", ocr_need_nothing, 0, current.m_clouds_noise_power, previous.m_clouds_noise_power, -10000.0f, 10000000.0f, true, command_type_engine_internal);
	static render_cc_float clouds_height_cc("r_clouds_height", ocr_need_nothing, 0, current.m_clouds_height, previous.m_clouds_height, -10000.0f, 10000000.0f, true, command_type_engine_internal);
	static render_cc_float clouds_scale_cc("r_clouds_scale", ocr_need_nothing, 0, current.m_clouds_scale, previous.m_clouds_scale, -10000.0f, 10000000.0f, true, command_type_engine_internal);
	static render_cc_u32 clouds_grid_width_cc("r_clouds_grid_width", ocr_need_nothing, 0, current.m_clouds_grid_width, previous.m_clouds_grid_width, 1, 2048, true, command_type_engine_internal);
	static render_cc_u32 clouds_grid_height_cc("r_clouds_grid_height", ocr_need_nothing, 0, current.m_clouds_grid_height, previous.m_clouds_grid_height, 1, 256, true, command_type_engine_internal);
	static render_cc_bool clouds_allow_moving_cc("r_clouds_allow_moving", ocr_need_nothing, 0, current.m_clouds_allow_moving, previous.m_clouds_allow_moving, true, command_type_engine_internal);
	static render_cc_float clouds_moving_speed_cc("r_clouds_moving_speed", ocr_need_nothing, 0, current.m_clouds_moving_speed, previous.m_clouds_moving_speed, 0.0f, 1000.0f, true, command_type_engine_internal);
	static render_cc_u32 clouds_num_evaluate_slices_cc("r_clouds_num_evaluate_slices", ocr_need_nothing, 0, current.m_clouds_num_evaluate_slices, previous.m_clouds_num_evaluate_slices, 1, 1024, true, command_type_engine_internal);
	static render_cc_float clouds_scale_by_distance_cc("r_clouds_scale_by_distance", ocr_need_nothing, 0, current.m_clouds_scale_by_distance, previous.m_clouds_scale_by_distance, 0.0f, 32.0f, true, command_type_engine_internal);
	static render_cc_float clouds_scale_xy_cc("r_clouds_scale_xy", ocr_need_nothing, 0, current.m_clouds_scale_xy, previous.m_clouds_scale_xy, 0.0f, 100000.0f, true, command_type_engine_internal);
	static render_cc_bool clouds_debug_mode_cc("r_clouds_debug_mode", ocr_need_nothing, 0, current.m_clouds_debug_mode, previous.m_clouds_debug_mode, true, command_type_engine_internal);
	static render_cc_bool use_god_rays_cc("r_use_god_rays", ocr_need_reset_postprocess, "GLOBAL_USE_GOD_RAYS", current.m_use_god_rays, previous.m_use_god_rays, true, command_type_engine_internal);
	static render_cc_bool use_hiz_occlusion_culling_cc("r_use_hiz_occlusion_culling", ocr_need_nothing, 0, current.m_use_hiz_occlusion_culling, previous.m_use_hiz_occlusion_culling, true, command_type_engine_internal);
	static render_cc_u32 hiz_occlusion_culling_framerate_cc("r_hiz_occlusion_culling_framerate", ocr_need_nothing, 0, current.m_hiz_occlusion_culling_framerate, previous.m_hiz_occlusion_culling_framerate, 1, 128, true, command_type_engine_internal);
	static render_cc_u32 num_shadow_cascades_cc("r_num_shadow_cascades", ocr_need_nothing, 0, current.m_num_shadow_cascades, previous.m_num_shadow_cascades, 1, 4, true, command_type_engine_internal);
	static render_cc_u32 cascaded_shadow_map_size_cc("r_cascaded_shadow_map_size", ocr_need_reset_lighting, "GLOBAL_CASCADED_SHADOW_MAP_SIZE", current.m_cascaded_shadow_map_size, previous.m_cascaded_shadow_map_size, 128, 2048, true, command_type_engine_internal);
	static render_cc_u32 num_max_light_instances_cc("r_num_max_light_instances", ocr_need_reset_lighting, "GLOBAL_NUM_MAX_LIGHT_INSTANCES", current.m_num_max_light_instances, previous.m_num_max_light_instances, 1, 2048, true, command_type_engine_internal);
	static render_cc_bool enabled_sky_sphere_stage_cc("r_enabled_sky_sphere_stage", ocr_need_nothing, 0, current.m_enabled_sky_sphere_stage, previous.m_enabled_sky_sphere_stage, true, command_type_engine_internal);
	static render_cc_bool enabled_atmosphere_stage_cc("r_enabled_atmosphere_stage", ocr_need_nothing, 0, current.m_enabled_atmosphere_stage, previous.m_enabled_atmosphere_stage, true, command_type_engine_internal);
	static render_cc_float fxaa_quality_subpix_cc("r_fxaa_quality_subpix", ocr_need_nothing, 0, current.m_fxaa_quality_subpix, previous.m_fxaa_quality_subpix, -10000.0f, 100000.0f, true, command_type_engine_internal);
	static render_cc_float fxaa_quality_edge_threshold_cc("r_fxaa_quality_edge_threshold", ocr_need_nothing, 0, current.m_fxaa_quality_edge_threshold, previous.m_fxaa_quality_edge_threshold, -10000.0f, 100000.0f, true, command_type_engine_internal);
	static render_cc_float fxaa_quality_edge_threshold_min_cc("r_fxaa_quality_edge_threshold_min", ocr_need_nothing, 0, current.m_fxaa_quality_edge_threshold_min, previous.m_fxaa_quality_edge_threshold_min, -10000.0f, 100000.0f, true, command_type_engine_internal);
	static render_cc_float ssao_screen_ratio_cc("r_ssao_screen_ratio", ocr_need_resize_window, 0, current.m_ssao_screen_ratio, previous.m_ssao_screen_ratio, 0.01f, 1.0f, true, command_type_engine_internal);
	static render_cc_u32 ssao_num_samples_cc("r_ssao_num_samples", ocr_need_reset_postprocess, "GLOBAL_SSAO_NUM_SAMPLES", current.m_ssao_num_samples, previous.m_ssao_num_samples, 8, 128, true, command_type_engine_internal);
	static render_cc_bool ssao_use_filtering_cc("r_ssao_use_filtering", ocr_need_resize_window, 0, current.m_ssao_use_filtering, previous.m_ssao_use_filtering, true, command_type_engine_internal);
	static render_cc_bool ssao_use_temporal_filtering_cc("r_ssao_use_temporal_filtering", ocr_need_nothing, 0, current.m_ssao_use_temporal_filtering, previous.m_ssao_use_temporal_filtering, true, command_type_engine_internal);
	static render_cc_bool draw_grass_cc("r_draw_grass", ocr_need_nothing, 0, current.m_draw_grass, previous.m_draw_grass, true, command_type_engine_internal);
	static render_cc_bool enabled_volume_fog_stage_cc("r_enabled_volume_fog_stage", ocr_need_reset_renderer, 0, current.m_enabled_volume_fog_stage, previous.m_enabled_volume_fog_stage, true, command_type_engine_internal);
	static render_cc_bool use_16bit_rt_cc("r_use_16bit_rt", ocr_need_reset_renderer, "GLOBAL_USE_16BIT_RT", current.m_use_16bit_rt, previous.m_use_16bit_rt, true, command_type_engine_internal);
	static render_cc_bool use_shader_lods_cc("r_use_shader_lods", ocr_need_nothing, 0, current.m_use_shader_lods, previous.m_use_shader_lods, true, command_type_engine_internal);
	static render_cc_bool update_shadows_every_frame_cc("r_update_shadows_every_frame", ocr_need_reset_lighting, "GLOBAL_UPDATE_SHADOWS_EVERY_FRAME", current.m_update_shadows_every_frame, previous.m_update_shadows_every_frame, true, command_type_engine_internal);
	static render_cc_bool use_screenspace_reflections_mask_cc("r_use_screenspace_reflections_mask", ocr_need_reset_lighting, "GLOBAL_USE_SCREENSPACE_REFLECTIONS_MASK", current.m_use_screenspace_reflections_mask, previous.m_use_screenspace_reflections_mask, true, command_type_engine_internal);
	static render_cc_bool use_poisson_disc_shadow_filter_cc("r_use_poisson_disc_shadow_filter", ocr_need_reset_lighting, "GLOBAL_USE_POISSON_DISC_SHADOW_FILTER", current.m_use_poisson_disc_shadow_filter, previous.m_use_poisson_disc_shadow_filter, true, command_type_engine_internal);
	static render_cc_bool use_temporal_antialiasing_cc("r_use_temporal_antialiasing", ocr_need_nothing, 0, current.m_use_temporal_antialiasing, previous.m_use_temporal_antialiasing, true, command_type_engine_internal);
	static render_cc_bool use_motion_blur_cc("r_use_motion_blur", ocr_need_reset_postprocess, 0, current.m_use_motion_blur, previous.m_use_motion_blur, true, command_type_engine_internal);
	static render_cc_float grass_lod1_distance_cc("r_grass_lod1_distance", ocr_need_nothing, 0, current.m_grass_lod1_distance, previous.m_grass_lod1_distance, 0.01f, 1000.0f, true, command_type_engine_internal);
	static render_cc_float grass_lod2_distance_cc("r_grass_lod2_distance", ocr_need_nothing, 0, current.m_grass_lod2_distance, previous.m_grass_lod2_distance, 0.01f, 1000.0f, true, command_type_engine_internal);
	static render_cc_bool use_texture_streaming_cc("r_use_texture_streaming", ocr_need_reload_textures, 0, current.m_use_texture_streaming, previous.m_use_texture_streaming, true, command_type_engine_internal);
	static render_cc_bool use_motion_vectors_in_taa_cc("r_use_motion_vectors_in_taa", ocr_need_nothing, 0, current.m_use_motion_vectors_in_taa, previous.m_use_motion_vectors_in_taa, true, command_type_engine_internal);
	static render_cc_float motion_blur_scale_cc("r_motion_blur_scale", ocr_need_nothing, 0, current.m_motion_blur_scale, previous.m_motion_blur_scale, 0.0f, 100.0f, true, command_type_engine_internal);
	static render_cc_u32 texture_quality_cc("r_texture_quality", ocr_need_reload_textures, 0, current.m_texture_quality, previous.m_texture_quality, 0, 2, true, command_type_engine_internal);
	static render_cc_bool use_vegetation_trample_cc("r_use_vegetation_trample", ocr_need_update_vegetation, "GLOBAL_USE_VEGETATION_TRAMPLE", current.m_use_vegetation_trample, previous.m_use_vegetation_trample, false, command_type_engine_internal);
	static render_cc_float gamma_correction_factor_cc("r_gamma_correction_factor", ocr_need_nothing, 0, current.m_gamma_correction_factor, previous.m_gamma_correction_factor, 0.5f, 1.5f, true, command_type_engine_internal);
	static render_cc_u32 max_anisotropic_cc("r_max_anisotropic", ocr_need_recreate_samplers, 0, current.m_max_anisotropic, previous.m_max_anisotropic, 0, 16, true, command_type_engine_internal);
	static render_cc_u32 monitor_index_cc("r_monitor_index", ocr_need_resize_window, 0, current.m_monitor_index, previous.m_monitor_index, 0, 5, true, command_type_engine_internal);
	static render_cc_u32 geometry_quality_cc("r_geometry_quality", ocr_need_nothing, 0, current.m_geometry_quality, previous.m_geometry_quality, 0, 5, true, command_type_engine_internal);
	static render_cc_bool vsync_cc("r_vsync", ocr_need_nothing, 0, current.m_vsync, previous.m_vsync, true, command_type_engine_internal);
	static render_cc_bool fullscreen_cc("r_fullscreen", ocr_need_resize_window, 0, current.m_fullscreen, previous.m_fullscreen, true, command_type_engine_internal);
	static render_cc_u32 resolution_x_cc("r_resolution_x", ocr_need_resize_window, 0, current.m_resolution_x, previous.m_resolution_x, 1, 8192, true, command_type_engine_internal);
	static render_cc_u32 resolution_y_cc("r_resolution_y", ocr_need_resize_window, 0, current.m_resolution_y, previous.m_resolution_y, 1, 8192, true, command_type_engine_internal);

	static render_cc_u32 lighting_quality_cc("r_lighting_quality", ocr_need_reset_lighting, "GLOBAL_LIGHTING_QUALITY", current.m_lighting_quality, previous.m_lighting_quality, 0, 100, true, command_type_engine_internal);
	static render_cc_u32 post_process_quality_cc("r_post_process_quality", static_cast<enum_options_changes_result>(ocr_need_reset_lighting | ocr_need_reset_postprocess), "GLOBAL_POST_PROCESS_QUALITY", current.m_post_process_quality, previous.m_post_process_quality, 0, 100, true, command_type_engine_internal);
	static render_cc_u32 particles_quality_cc("r_particles_quality", ocr_need_nothing, 0, current.m_particles_quality, previous.m_particles_quality, 0, 100, true, command_type_engine_internal);
	static render_cc_u32 motion_blur_quality_cc("r_motion_blur_quality", ocr_need_reset_postprocess, 0, current.m_motion_blur_quality, previous.m_motion_blur_quality, 0, 100, true, command_type_engine_internal);
	static render_cc_u32 shading_quality_cc("r_shading_quality", ocr_need_reset_lighting, "GLOBAL_SHADING_QUALITY", current.m_shading_quality, previous.m_shading_quality, 0, 100, true, command_type_engine_internal);
	static render_cc_u32 ambient_occlusion_quality_cc("r_ambient_occlusion_quality", ocr_need_nothing, 0, current.m_ambient_occlusion_quality, previous.m_ambient_occlusion_quality, 0, 100, true, command_type_engine_internal);
	static render_cc_u32 antialiasing_method_cc("r_antialiasing_method", ocr_need_nothing, 0, current.m_antialiasing_method, previous.m_antialiasing_method, 0, 100, true, command_type_engine_internal);
	static render_cc_u32 decorations_quality_cc("r_decorations_quality", ocr_need_nothing, 0, current.m_decorations_quality, previous.m_decorations_quality, 0, 100, true, command_type_engine_internal);
	static render_cc_u32 graphics_quality_cc("r_graphics_quality", ocr_need_nothing, 0, current.m_graphics_quality, previous.m_graphics_quality, 0, 100, true, command_type_engine_internal);

	first_command				= &enabled_g_stage_cc;
	console_command* command	= first_command;

	while (command)
	{
		if (!command->next())
		{
			last_command = command;
			break;
		}
		command = command->next();
	}
}

void options::begin_render_options_changing( long volatile* waiting_for )
{
	threading::interlocked_exchange( *waiting_for, 0 );
}

enum_options_changes_result options::end_render_options_changing(
	vector<fs_new::virtual_path_string>& out_changed_defines
)
{
	math::uint2 res_str = parse_resolution( s_r_resolution_value.c_str( ) );
	current.m_resolution_x = res_str.x;
	current.m_resolution_y = res_str.y;
	current.m_enabled_local_light_shadows = current.m_lighting_quality != uro_lighting_quality_low;

	if ( current.m_shadow_quality > uro_shadow_quality_low )
	{
		switch ( current.m_antialiasing_method )
		{
		case uro_antialiasing_method_off:
			current.m_enabled_fxaa = false;
			current.m_use_temporal_antialiasing = false;
			break;
		case uro_antialiasing_method_FXAA:
			current.m_enabled_fxaa = true;
			current.m_enabled_sharpen = true;
			current.m_use_temporal_antialiasing = false;
			break;
		case uro_antialiasing_method_2xTAA:
			current.m_use_temporal_antialiasing = true;
			break;
		}
	}

	if ( current.m_shadow_quality == uro_shadow_quality_low )
	{
		current.m_use_poisson_disc_shadow_filter = false;
		current.m_cascaded_shadow_map_size = 512;
	}
	else if ( current.m_shadow_quality < uro_shadow_quality_count )
	{
		current.m_use_poisson_disc_shadow_filter = true;
		current.m_cascaded_shadow_map_size = 1024;
	}

	if ( current.m_ambient_occlusion_quality == uro_ambient_occlusion_quality_off )
		current.m_enabled_ambient_occlusion_stage = false;
	else if ( current.m_ambient_occlusion_quality < uro_ambient_occlusion_quality_count )
		current.m_enabled_ambient_occlusion_stage = true;

	if ( current.m_motion_blur_quality == uro_motion_blur_quality_off )
		current.m_use_motion_blur = false;
	else if ( current.m_motion_blur_quality < uro_motion_blur_quality_count )
		current.m_use_motion_blur = true;
	if ( current.m_post_process_quality == uro_post_process_quality_minimum )
		current.m_ssao_use_temporal_filtering = false;

	enum_options_changes_result result = ocr_need_nothing;
	for ( render_cc* command = first_render_command; command; command = command->render_next )
	{
		if ( !command->is_changed( ) )
			continue;

		result = static_cast<enum_options_changes_result>( result | command->get_changes_result( ) );
		out_changed_defines.push_back( fs_new::virtual_path_string( command->define_name( ) ) );
	}

	previous = current;
	console_commands::save( "user.cfg", console_commands::command_type_user_specific, memory::g_mt_allocator );
	return result;
}

void options::save( pcstr file_name )
{
	memory::writer			f(g_allocator);
	console_commands::console_command* current = first_command;
	console_commands::save_storage		s(*g_allocator);

	while(current)
	{
		if(current->is_serializable())
			current->save_to( s, g_allocator );

		if (current == last_command)
			break;

		current				= current->next();
	}

	s.save_to				( f );
	f.save_to				( file_name );
}

void options::on_config_loaded( resources::queries_result& data )
{
	if( !data.is_successful( ) )
		return;

	resources::pinned_ptr_const<u8> pinned_data	(data[ 0 ].get_managed_resource( ));
	memory::reader				F( pinned_data.c_ptr( ), pinned_data.size( ) );
	load_impl					( F );
}

static bool is_line_term( char a )
{
	return (a==13)||(a==10);
}

static u32 advance_term_string( memory::reader& F )
{
	u32 sz		= 0;
	while (!F.eof())
	{
		F.advance(1);				//Pos++;
		sz++;
		if (!F.eof() && is_line_term(*(char*)F.pointer()) )
		{
			while(!F.eof() && is_line_term(*(char*)F.pointer()))
				F.advance(1);		//Pos++;
			break;
		}
	}
	return sz;
}

static void r_string( memory::reader& F, char (&dest)[4096] )
{
	char *src 	= (char *) F.pointer();
	u32 sz 		= advance_term_string(F);
	vostok::strings::copy_n	(dest, sizeof(dest), src, sz);
}

void options::load_impl( memory::reader& F )
{
	string4096				str;
	while (!F.eof())
	{
		r_string			(F, str);
		vostok::console_commands::execute				(str, vostok::console_commands::execution_filter_all);
	}
}

void options::load( pcstr file_name )
{
	resources::query_resource	(
		file_name,
		vostok::resources::raw_data_class,
		boost::bind(&options::on_config_loaded, this, _1),
		g_allocator
	);
}

void options::fill_global_macros( shader_defines_list& out_defines )
{
	render_cc* current = first_render_command;

	while (current)
	{
		shader_macro d;

		if (current->fill_macro(d))
			out_defines.push_back(d);

		current				= current->render_next;
	}
}

void options::load_from_config( configs::binary_config_value const& config )
{
 	vostok::console_commands::console_command* command = first_command;
 	while (command)
 	{
 		if (config.value_exists(command->name()))
 		{
			vostok::configs::binary_config_value const& value_config = config[command->name()];

 			vostok::configs::enum_types type = (vostok::configs::enum_types)value_config.type;
 			switch (type)
 			{
 			case vostok::configs::t_boolean:
 				command->execute(
 					bool(value_config) ?
 					"true" : "false"
 					);
 				break;
 			case vostok::configs::t_float:
 				{
 					fs_new::virtual_path_string value;
 					value.assignf("%f", float(value_config));
 					command->execute(value.c_str());
 					break;
 				}
 			case vostok::configs::t_integer:
 				{
 					fs_new::virtual_path_string value;
 					value.assignf("%d", s32(value_config));
 					command->execute(value.c_str());
 					break;
 				}
 			default: NODEFAULT();
 			};
 		}

		if (command == last_command)
			break;
 		command = command->next();
 	}
}

void options::on_config_loaded2( resources::queries_result& data )
{
	if (data.is_successful())
	{
		load_from_config(
			vostok::static_cast_resource_ptr<vostok::configs::binary_config_ptr>(
				data[0].get_unmanaged_resource()
			)->get_root()["options"]
		);
	}
}

} // namespace render
} // namespace vostok
