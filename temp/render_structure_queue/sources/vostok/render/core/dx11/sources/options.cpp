////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\options.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 's_r_resolution_value''( )
{
	// FUNCTION BODY[0x7df410]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_r_resolution_cc''( )
{
	// FUNCTION BODY[0x7df450]
	// ******
}

// STATE[STUB]
void `vostok::render::options::register_console_commands'::`2'::`dynamic atexit destructor for 'graphics_quality_cc''( )
{
	// FUNCTION BODY[0x7f1370]
	// ******
}

// STATE[STUB]
 render_cc::render_cc( pcstr define_name, enum_options_changes_result changed_result )
{
	// FUNCTION BODY[0x55cdc0]: 2
	// <0x55cdc7>|0x007|+0x014:'33'
	// <0x55cddb>|0x01b|+0x002:'34'
	// ******
}

// STATE[STUB]
 render_cc_bool::render_cc_bool(
	pcstr							name,
	enum_options_changes_result		changed_result,
	pcstr							define_name,
	bool&							value,
	bool&							prev_value,
	bool							serializable,
	const console_commands::command_type	arg_6 /* console_commands::command_type command_type */
)
{
	// FUNCTION BODY[0x12b550]: 0
	// ******
}

// STATE[STUB]
void render_cc_bool::execute( pcstr args )
{
	// FUNCTION BODY[0x12b5b0]: 1
	// <0x12b5b0>|0x000|+0x00a:'49'
	// ******
}

// STATE[STUB]
bool render_cc_bool::is_changed( ) const
{
	return false;

	// FUNCTION BODY[0x12b5c0]: 1
	// <0x12b5c0>|0x000|+0x00f:'54'
	// ******
}

// STATE[STUB]
bool render_cc_bool::fill_macro( shader_macro& out_macro ) const
{
	return false;

	// FUNCTION BODY[0x12b5d0]: 8
	// <0x12b5d3>|0x003|+0x006:'58'
	// <0>
	// <0x12b5d9>|0x009|+0x033:'60'
	// <0x12b60c>|0x03c|+0x018:'61'
	// <0x12b624>|0x054|+0x006:'62'
	// <0>
	// <1>
	// <0x12b62a>|0x05a|-0x003:'65'
	// <0x12b627>|0x057|+0x006:'66'
	// ******
}

// STATE[STUB]
 render_cc_float::render_cc_float(
	pcstr							name,
	enum_options_changes_result		changed_result,
	pcstr							define_name,
	float&							value,
	float&							prev_value,
	const float						min,
	const float						max,
	bool							serializable,
	const console_commands::command_type	arg_8 /* console_commands::command_type command_type */
)
{
	// FUNCTION BODY[0x12b630]: 0
	// ******
}

// STATE[STUB]
void render_cc_float::execute( pcstr args )
{
	// FUNCTION BODY[0x12b690]: 1
	// <0x12b690>|0x000|+0x00a:'83'
	// ******
}

// STATE[STUB]
bool render_cc_float::is_changed( ) const
{
	return false;

	// FUNCTION BODY[0x12b6a0]: 1
	// <0x12b6a0>|0x000|+0x019:'88'
	// <0x12b6b9>|0x019|-0x001:'88'
	// <0x12b6b8>|0x018|+0x003:'89'
	// ******
}

// STATE[STUB]
bool render_cc_float::fill_macro( shader_macro& out_macro ) const
{
	return false;

	// FUNCTION BODY[0x12b6c0]: 8
	// <0x12b6c3>|0x003|+0x006:'92'
	// <0>
	// <0x12b6c9>|0x009|+0x028:'94'
	// <0x12b6f1>|0x031|+0x01b:'95'
	// <0x12b70c>|0x04c|+0x006:'96'
	// <0>
	// <1>
	// <0x12b712>|0x052|-0x003:'99'
	// <0x12b70f>|0x04f|+0x006:'100'
	// ******
}

// STATE[STUB]
 render_cc_u32::render_cc_u32(
	pcstr							name,
	enum_options_changes_result		changed_result,
	pcstr							define_name,
	u32&							value,
	u32&							prev_value,
	const u32						min,
	const u32						max,
	bool							serializable,
	const console_commands::command_type	arg_8 /* console_commands::command_type command_type */
)
{
	// FUNCTION BODY[0x12b720]: 0
	// ******
}

// STATE[STUB]
void render_cc_u32::execute( pcstr args )
{
	// FUNCTION BODY[0x12b780]: 1
	// <0x12b780>|0x000|+0x00a:'117'
	// ******
}

// STATE[STUB]
bool render_cc_u32::is_changed( ) const
{
	return false;

	// FUNCTION BODY[0x12b790]: 1
	// <0x12b790>|0x000|+0x00f:'122'
	// ******
}

// STATE[STUB]
bool render_cc_u32::fill_macro( shader_macro& out_macro ) const
{
	return false;

	// FUNCTION BODY[0x12b7a0]: 18
	// <0x12b7a4>|0x004|+0x00b:'126'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x12b7af>|0x00f|+0x108:'131'
	// <0>
	// <0x12b8b7>|0x117|+0x002:'133'
	// <0>
	// <0x12b8b9>|0x119|+0x002:'135'
	// <0>
	// <0x12b8bb>|0x11b|+0x01b:'137'
	// <0>
	// <0x12b8d6>|0x136|+0x021:'139'
	// <0x12b8f7>|0x157|-0x006:'139'
	// <0x12b8f1>|0x151|+0x007:'140'
	// <0>
	// <1>
	// <0x12b8f8>|0x158|-0x004:'143'
	// <0x12b8f4>|0x154|+0x007:'144'
	// ******
}

// STATE[STUB]
 options::options( )
{
	// FUNCTION BODY[0x55f0a0]: 2
	// <0x55f0b3>|0x013|+0x005:'154'
	// <0x55f0b8>|0x018|+0x008:'155'
	// ******
}

// STATE[STUB]
void options::set_default_values( )
{
	// FUNCTION BODY[0x55c9d0]: 124
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <36>
	// <0x55c9d0>|0x000|+0x015:'200'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <0x55c9e5>|0x015|+0x1ac:'224'
	// <0x55cb91>|0x1c1|+0x005:'225'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55cb96>|0x1c6|+0x005:'231'
	// <0x55cb9b>|0x1cb|+0x00d:'232'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x55cba8>|0x1d8|+0x00d:'245'
	// <0x55cbb5>|0x1e5|+0x00d:'246'
	// <0x55cbc2>|0x1f2|+0x020:'247'
	// <0x55cbe2>|0x212|+0x005:'248'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x55cbe7>|0x217|+0x0b3:'261'
	// <0x55cc9a>|0x2ca|+0x00d:'262'
	// <0x55cca7>|0x2d7|+0x006:'263'
	// <0x55ccad>|0x2dd|+0x006:'264'
	// <0x55ccb3>|0x2e3|+0x00b:'265'
	// <0x55ccbe>|0x2ee|+0x005:'266'
	// <0x55ccc3>|0x2f3|+0x006:'267'
	// <0x55ccc9>|0x2f9|+0x005:'268'
	// <0x55ccce>|0x2fe|+0x00b:'269'
	// <0x55ccd9>|0x309|+0x006:'270'
	// <0x55ccdf>|0x30f|+0x006:'271'
	// <0x55cce5>|0x315|+0x006:'272'
	// <0x55cceb>|0x31b|+0x006:'273'
	// <0x55ccf1>|0x321|+0x006:'274'
	// <0x55ccf7>|0x327|+0x006:'275'
	// <0x55ccfd>|0x32d|+0x006:'276'
	// <0x55cd03>|0x333|+0x006:'277'
	// <0x55cd09>|0x339|+0x006:'278'
	// <0x55cd0f>|0x33f|+0x00a:'279'
	// <0x55cd19>|0x349|+0x00a:'280'
	// <0x55cd23>|0x353|+0x006:'281'
	// <0x55cd29>|0x359|+0x006:'282'
	// <0x55cd2f>|0x35f|+0x006:'283'
	// <0x55cd35>|0x365|+0x006:'284'
	// <0>
	// <0x55cd3b>|0x36b|+0x011:'286'
	// ******
}

// STATE[STUB]
math::uint2 parse_resolution( pcstr in_str )
{
	// LOCALS
	// char[16] 						xy_str
	// ******

	return vostok::math::uint2(1, 1);

	// FUNCTION BODY[0x55ce00]: 22
	// <0>
	// <1>
	// <0x55ce0a>|0x00a|+0x009:'304'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55ce13>|0x013|+0x021:'310'
	// <0>
	// <0x55ce34>|0x034|+0x004:'312'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55ce38>|0x038|+0x00a:'317'
	// <0x55ce42>|0x042|+0x00b:'318'
	// <0>
	// <0x55ce4d>|0x04d|+0x008:'320'
	// <0>
	// <1>
	// <0x55ce55>|0x055|+0x007:'323'
	// ******
}

// STATE[STUB]
void options::register_console_commands( )
{
	// STATICS
	// static render_cc_bool 			enabled_g_stage_cc = <0x4c2abf8>;
	// static render_cc_bool 			enabled_draw_speedtree_billboards_cc = <0x4c2a358>;
	// static render_cc_bool 			enabled_fxaa_cc = <0x4c2a238>;
	// static render_cc_float 			clouds_noise_frequency_cc = <0x4c29610>;
	// static render_cc_u32 			clouds_grid_width_cc = <0x4c29408>;
	// static render_cc_bool 			use_screenspace_reflections_mask_cc = <0x4c28910>;
	// static render_cc_float 			gamma_correction_factor_cc = <0x4c284c0>;
	// static render_cc_u32 			num_test_lights_cc = <0x4c296e0>;
	// static render_cc_float 			clouds_height_cc = <0x4c294d8>;
	// static render_cc_bool 			enabled_sky_sphere_stage_cc = <0x4c28e78>;
	// static render_cc_u32 			ssao_num_samples_cc = <0x4c28c10>;
	// static render_cc_bool 			draw_grass_cc = <0x4c28af0>;
	// static render_cc_bool 			use_temporal_antialiasing_cc = <0x4c28850>;
	// static render_cc_bool 			enabled_draw_speedtree_leafcards_cc = <0x4c2a3b8>;
	// static render_cc_float 			motion_blur_scale_cc = <0x4c285f0>;
	// static render_cc_u32 			lighting_quality_cc = <0x4c28190>;
	// static render_cc_u32 			ambient_occlusion_quality_cc = <0x4c27f88>;
	// static render_cc_bool 			enabled_forward_stage_cc = <0x4c2a838>;
	// static render_cc_bool 			use_cpu_mlaa_cc = <0x4c2a178>;
	// static render_cc_u32 			spot_shadow_map_size_cc = <0x4c29f78>;
	// static render_cc_u32 			hiz_occlusion_culling_framerate_cc = <0x4c29010>;
	// static render_cc_bool 			use_16bit_rt_cc = <0x4c28a30>;
	// static render_cc_float 			grass_lod1_distance_cc = <0x4c28788>;
	// static render_cc_u32 			motion_blur_quality_cc = <0x4c28058>;
	// static render_cc_u32 			antialiasing_method_cc = <0x4c27f20>;
	// static render_cc_bool 			enabled_draw_speedtree_fronds_cc = <0x4c2a478>;
	// static render_cc_u32 			shadow_map_size_cc = <0x4c29fe0>;
	// static render_cc_u32 			num_propagate_iterations_cc = <0x4c29d78>;
	// static render_cc_bool 			clouds_allow_moving_cc = <0x4c29338>;
	// static render_cc_u32 			num_shadow_cascades_cc = <0x4c28fa8>;
	// static render_cc_bool 			ssao_use_temporal_filtering_cc = <0x4c28b50>;
	// static render_cc_bool 			use_motion_vectors_in_taa_cc = <0x4c28658>;
	// static render_cc_bool 			use_vegetation_trample_cc = <0x4c28528>;
	// static render_cc_u32 			resolution_y_cc = <0x4c281f8>;
	// static render_cc_bool 			enabled_sun_shadows_stage_cc = <0x4c2aa18>;
	// static render_cc_bool 			enabled_forward_lighting_stage_cc = <0x4c2a8f8>;
	// static render_cc_bool 			enabled_terrain_shadows_cc = <0x4c2a598>;
	// static render_cc_bool 			enabled_local_light_shadows_cc = <0x4c2a118>;
	// static render_cc_bool 			use_parallax_cc = <0x4c2a0b8>;
	// static render_cc_u32 			organic_irradiance_texture_size_cc = <0x4c2a050>;
	// static render_cc_float 			lpv_flux_amplifier_cc = <0x4c29ca8>;
	// static render_cc_bool 			use_shader_lods_cc = <0x4c289d0>;
	// static render_cc_bool 			use_texture_streaming_cc = <0x4c286b8>;
	// static render_cc_u32 			max_anisotropic_cc = <0x4c28458>;
	// static render_cc_bool 			enabled_deferred_lighting_stage_cc = <0x4c2a898>;
	// static render_cc_bool 			enabled_sky_box_stage_cc = <0x4c2a718>;
	// static render_cc_bool 			enabled_clouds_stage_cc = <0x4c2a658>;
	// static render_cc_bool 			lpv_disable_rsm_generating_cc = <0x4c29a58>;
	// static render_cc_bool 			use_hiz_occlusion_culling_cc = <0x4c29078>;
	// static render_cc_u32 			monitor_index_cc = <0x4c283f0>;
	// static render_cc_bool 			vsync_cc = <0x4c28328>;
	// static render_cc_bool 			enabled_distortion_stage_cc = <0x4c2aa78>;
	// static render_cc_bool 			enabled_lighting_stage_cc = <0x4c2a958>;
	// static render_cc_bool 			enabled_mlaa_cc = <0x4c2a1d8>;
	// static render_cc_bool 			enabled_light_propagation_volumes_stage_cc = <0x4c29eb0>;
	// static render_cc_bool 			lpv_disable_vpl_injection_cc = <0x4c29998>;
	// static render_cc_float 			clouds_moving_speed_cc = <0x4c292d0>;
	// static render_cc_bool 			enabled_atmosphere_stage_cc = <0x4c28e18>;
	// static render_cc_bool 			use_poisson_disc_shadow_filter_cc = <0x4c288b0>;
	// static render_cc_bool 			enabled_draw_speedtree_cc = <0x4c2a538>;
	// static render_cc_u32 			light_propagation_volumes_rsm_size_cc = <0x4c29e48>;
	// static render_cc_float 			lpv_occlusion_amplifier_cc = <0x4c297a8>;
	// static render_cc_u32 			clouds_num_evaluate_slices_cc = <0x4c29268>;
	// static render_cc_bool 			use_motion_blur_cc = <0x4c287f0>;
	// static render_cc_u32 			shading_quality_cc = <0x4c27ff0>;
	// static render_cc_bool 			enabled_post_process_stage_cc = <0x4c2a778>;
	// static render_cc_bool 			enabled_draw_terrain_cc = <0x4c2a5f8>;
	// static render_cc_bool 			enabled_draw_speedtree_leafmeshes_cc = <0x4c2a418>;
	// static render_cc_u32 			shadow_quality_cc = <0x4c29f10>;
	// static render_cc_float 			radiance_volume_scale_cc = <0x4c29d10>;
	// static render_cc_bool 			lpv_disable_lpv_lookup_cc = <0x4c29878>;
	// static render_cc_float 			fxaa_quality_edge_threshold_cc = <0x4c28d48>;
	// static render_cc_float 			fxaa_quality_edge_threshold_min_cc = <0x4c28ce0>;
	// static render_cc_bool 			update_shadows_every_frame_cc = <0x4c28970>;
	// static render_cc_u32 			geometry_quality_cc = <0x4c28388>;
	// static render_cc_bool 			fullscreen_cc = <0x4c282c8>;
	// static render_cc_bool 			enabled_decals_accumulate_stage_cc = <0x4c2aad8>;
	// static render_cc_bool 			enabled_draw_speedtree_branches_cc = <0x4c2a4d8>;
	// static render_cc_bool 			enabled_draw_models_cc = <0x4c2a2f8>;
	// static render_cc_u32 			num_radiance_volume_cells_cc = <0x4c29de0>;
	// static render_cc_bool 			lpv_movable_cc = <0x4c29be0>;
	// static render_cc_bool 			lpv_disable_rsm_downsampling_cc = <0x4c299f8>;
	// static render_cc_bool 			lpv_disable_propagation_cc = <0x4c298d8>;
	// static render_cc_float 			ssao_screen_ratio_cc = <0x4c28c78>;
	// static render_cc_bool 			ssao_use_filtering_cc = <0x4c28bb0>;
	// static render_cc_float 			grass_lod2_distance_cc = <0x4c28720>;
	// static render_cc_u32 			decorations_quality_cc = <0x4c27eb8>;
	// static render_cc_u32 			graphics_quality_cc = <0x4c27e50>;
	// static render_cc_bool 			enabled_particles_stage_cc = <0x4c2a7d8>;
	// static render_cc_u32 			lpv_num_cascades_cc = <0x4c29b78>;
	// static render_cc_float 			clouds_scale_cc = <0x4c29470>;
	// static render_cc_bool 			enabled_volume_fog_stage_cc = <0x4c28a90>;
	// static render_cc_bool 			enabled_g_stage_material_pass_cc = <0x4c2ab38>;
	// static render_cc_bool 			lpv_gather_occluders_from_camera_view_cc = <0x4c29ab8>;
	// static render_cc_bool 			lpv_use_specular_reflection_cc = <0x4c29748>;
	// static render_cc_float 			clouds_noise_power_cc = <0x4c29540>;
	// static render_cc_u32 			clouds_grid_height_cc = <0x4c293a0>;
	// static render_cc_float 			clouds_scale_by_distance_cc = <0x4c29200>;
	// static render_cc_u32 			cascaded_shadow_map_size_cc = <0x4c28f40>;
	// static render_cc_u32 			num_max_light_instances_cc = <0x4c28ed8>;
	// static render_cc_float 			fxaa_quality_subpix_cc = <0x4c28db0>;
	// static render_cc_u32 			resolution_x_cc = <0x4c28260>;
	// static render_cc_bool 			enabled_sun_stage_cc = <0x4c2a9b8>;
	// static render_cc_bool 			enabled_ambient_occlusion_stage_cc = <0x4c2a6b8>;
	// static render_cc_u32 			lpv_refresh_once_per_frames_cc = <0x4c29810>;
	// static render_cc_float 			clouds_noise_amplitude_cc = <0x4c295a8>;
	// static render_cc_bool 			use_god_rays_cc = <0x4c290d8>;
	// static render_cc_bool 			enabled_g_stage_pre_pass_cc = <0x4c2ab98>;
	// static render_cc_bool 			enabled_sharpen_cc = <0x4c2a298>;
	// static render_cc_bool 			lpv_gather_occluders_from_light_view_cc = <0x4c29b18>;
	// static render_cc_bool 			lpv_disable_gv_injection_cc = <0x4c29938>;
	// static render_cc_float 			clouds_noise_octaves_cc = <0x4c29678>;
	// static render_cc_u32 			texture_quality_cc = <0x4c28588>;
	// static render_cc_float 			lpv_interreflection_contribution_cc = <0x4c29c40>;
	// static render_cc_float 			clouds_scale_xy_cc = <0x4c29198>;
	// static render_cc_bool 			clouds_debug_mode_cc = <0x4c29138>;
	// static render_cc_u32 			post_process_quality_cc = <0x4c28128>;
	// static render_cc_u32 			particles_quality_cc = <0x4c280c0>;
	// ******

	// FUNCTION BODY[0x55cfa0]: 149
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x55cfa1>|0x001|+0x046:'343'
	// <0x55cfe7>|0x047|+0x042:'344'
	// <0x55d029>|0x089|+0x042:'345'
	// <0x55d06b>|0x0cb|+0x042:'346'
	// <0x55d0ad>|0x10d|+0x042:'347'
	// <0x55d0ef>|0x14f|+0x042:'348'
	// <0x55d131>|0x191|+0x042:'349'
	// <0x55d173>|0x1d3|+0x047:'350'
	// <0x55d1ba>|0x21a|+0x048:'351'
	// <0x55d202>|0x262|+0x044:'352'
	// <0x55d246>|0x2a6|+0x048:'353'
	// <0x55d28e>|0x2ee|+0x048:'354'
	// <0x55d2d6>|0x336|+0x048:'355'
	// <0x55d31e>|0x37e|+0x048:'356'
	// <0x55d366>|0x3c6|+0x048:'357'
	// <0x55d3ae>|0x40e|+0x048:'358'
	// <0x55d3f6>|0x456|+0x045:'359'
	// <0x55d43b>|0x49b|+0x045:'360'
	// <0x55d480>|0x4e0|+0x045:'361'
	// <0x55d4c5>|0x525|+0x045:'362'
	// <0x55d50a>|0x56a|+0x045:'363'
	// <0x55d54f>|0x5af|+0x045:'364'
	// <0x55d594>|0x5f4|+0x045:'365'
	// <0x55d5d9>|0x639|+0x045:'366'
	// <0x55d61e>|0x67e|+0x045:'367'
	// <0x55d663>|0x6c3|+0x045:'368'
	// <0x55d6a8>|0x708|+0x045:'369'
	// <0x55d6ed>|0x74d|+0x045:'370'
	// <0x55d732>|0x792|+0x046:'371'
	// <0x55d778>|0x7d8|+0x045:'372'
	// <0x55d7bd>|0x81d|+0x04b:'373'
	// <0x55d808>|0x868|+0x04a:'374'
	// <0x55d852>|0x8b2|+0x045:'375'
	// <0x55d897>|0x8f7|+0x047:'376'
	// <0x55d8de>|0x93e|+0x048:'377'
	// <0x55d926>|0x986|+0x042:'378'
	// <0x55d968>|0x9c8|+0x047:'379'
	// <0x55d9af>|0xa0f|+0x044:'380'
	// <0x55d9f3>|0xa53|+0x045:'381'
	// <0x55da38>|0xa98|+0x04c:'382'
	// <0x55da84>|0xae4|+0x04b:'383'
	// <0x55dacf>|0xb2f|+0x04b:'384'
	// <0x55db1a>|0xb7a|+0x045:'385'
	// <0x55db5f>|0xbbf|+0x04a:'386'
	// <0x55dba9>|0xc09|+0x045:'387'
	// <0x55dbee>|0xc4e|+0x045:'388'
	// <0x55dc33>|0xc93|+0x046:'389'
	// <0x55dc79>|0xcd9|+0x046:'390'
	// <0x55dcbf>|0xd1f|+0x046:'391'
	// <0x55dd05>|0xd65|+0x046:'392'
	// <0x55dd4b>|0xdab|+0x046:'393'
	// <0x55dd91>|0xdf1|+0x046:'394'
	// <0x55ddd7>|0xe37|+0x04a:'395'
	// <0x55de21>|0xe81|+0x04b:'396'
	// <0x55de6c>|0xecc|+0x045:'397'
	// <0x55deb1>|0xf11|+0x04b:'398'
	// <0x55defc>|0xf5c|+0x04b:'399'
	// <0x55df47>|0xfa7|+0x04b:'400'
	// <0x55df92>|0xff2|+0x050:'401'
	// <0x55dfe2>|0x1042|+0x050:'402'
	// <0x55e032>|0x1092|+0x050:'403'
	// <0x55e082>|0x10e2|+0x050:'404'
	// <0x55e0d2>|0x1132|+0x047:'405'
	// <0x55e119>|0x1179|+0x047:'406'
	// <0x55e160>|0x11c0|+0x03d:'407'
	// <0x55e19d>|0x11fd|+0x045:'408'
	// <0x55e1e2>|0x1242|+0x041:'409'
	// <0x55e223>|0x1283|+0x045:'410'
	// <0x55e268>|0x12c8|+0x045:'411'
	// <0x55e2ad>|0x130d|+0x03f:'412'
	// <0x55e2ec>|0x134c|+0x045:'413'
	// <0x55e331>|0x1391|+0x03e:'414'
	// <0x55e36f>|0x13cf|+0x046:'415'
	// <0x55e3b5>|0x1415|+0x047:'416'
	// <0x55e3fc>|0x145c|+0x050:'417'
	// <0x55e44c>|0x14ac|+0x048:'418'
	// <0x55e494>|0x14f4|+0x045:'419'
	// <0x55e4d9>|0x1539|+0x045:'420'
	// <0x55e51e>|0x157e|+0x050:'421'
	// <0x55e56e>|0x15ce|+0x050:'422'
	// <0x55e5be>|0x161e|+0x050:'423'
	// <0x55e60e>|0x166e|+0x04c:'424'
	// <0x55e65a>|0x16ba|+0x04a:'425'
	// <0x55e6a4>|0x1704|+0x045:'426'
	// <0x55e6e9>|0x1749|+0x045:'427'
	// <0x55e72e>|0x178e|+0x045:'428'
	// <0x55e773>|0x17d3|+0x048:'429'
	// <0x55e7bb>|0x181b|+0x04b:'430'
	// <0x55e806>|0x1866|+0x045:'431'
	// <0x55e84b>|0x18ab|+0x04b:'432'
	// <0x55e896>|0x18f6|+0x04b:'433'
	// <0x55e8e1>|0x1941|+0x04b:'434'
	// <0x55e92c>|0x198c|+0x045:'435'
	// <0x55e971>|0x19d1|+0x048:'436'
	// <0x55e9b9>|0x1a19|+0x050:'437'
	// <0x55ea09>|0x1a69|+0x050:'438'
	// <0x55ea59>|0x1ab9|+0x03e:'439'
	// <0x55ea97>|0x1af7|+0x03f:'440'
	// <0x55ead6>|0x1b36|+0x045:'441'
	// <0x55eb1b>|0x1b7b|+0x043:'442'
	// <0x55eb5e>|0x1bbe|+0x046:'443'
	// <0x55eba4>|0x1c04|+0x04a:'444'
	// <0x55ebee>|0x1c4e|+0x043:'445'
	// <0x55ec31>|0x1c91|+0x041:'446'
	// <0x55ec72>|0x1cd2|+0x048:'447'
	// <0x55ecba>|0x1d1a|+0x045:'448'
	// <0x55ecff>|0x1d5f|+0x045:'449'
	// <0x55ed44>|0x1da4|+0x04a:'450'
	// <0x55ed8e>|0x1dee|+0x04a:'451'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55edd8>|0x1e38|+0x04f:'457'
	// <0x55ee27>|0x1e87|+0x04e:'458'
	// <0x55ee75>|0x1ed5|+0x048:'459'
	// <0x55eebd>|0x1f1d|+0x04b:'460'
	// <0x55ef08>|0x1f68|+0x04e:'461'
	// <0x55ef56>|0x1fb6|+0x048:'462'
	// <0x55ef9e>|0x1ffe|+0x048:'463'
	// <0x55efe6>|0x2046|+0x048:'464'
	// <0x55f02e>|0x208e|+0x048:'465'
	// <0>
	// <0x55f076>|0x20d6|+0x00a:'467'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55f080>|0x20e0|+0x00e:'472'
	// <0>
	// <0x55f08e>|0x20ee|-0x007:'474'
	// <0>
	// <1>
	// <0x55f087>|0x20e7|+0x006:'477'
	// <0>
	// <0x55f08d>|0x20ed|+0x004:'479'
	// ******
}

// STATE[STUB]
void options::begin_render_options_changing( long volatile* waiting_for )
{
	// FUNCTION BODY[0x55cd50]: 6
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55cd50>|0x000|+0x004:'487'
	// <0>
	// ******
}

// STATE[STUB]
enum_options_changes_result options::end_render_options_changing( vector< fs_new::virtual_path_string >& out_changed_defines )
{
	// LOCALS
	// math::uint2 						res_str
	// ******

	// CALL SITE INFO
	// <0x55f28e> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x55f180]: 135
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <0x55f180>|0x000|+0x01c:'515'
	// <0>
	// <0x55f19c>|0x01c|+0x004:'517'
	// <0x55f1a0>|0x020|+0x004:'518'
	// <0x55f1a4>|0x024|+0x011:'519'
	// <0>
	// <0x55f1b5>|0x035|+0x026:'521'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x55f1db>|0x05b|+0x022:'530'
	// <0>
	// <1>
	// <0x55f1fd>|0x07d|+0x006:'533'
	// <0x55f203>|0x083|-0x014:'534'
	// <0>
	// <1>
	// <0x55f1ef>|0x06f|+0x006:'537'
	// <0x55f1f5>|0x075|+0x006:'538'
	// <0>
	// <0x55f1fb>|0x07b|-0x014:'540'
	// <0>
	// <0x55f1e7>|0x067|+0x006:'542'
	// <0x55f1ed>|0x06d|+0x01c:'543'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55f209>|0x089|+0x01c:'548'
	// <0>
	// <1>
	// <0x55f225>|0x0a5|+0x006:'551'
	// <0x55f22b>|0x0ab|-0x018:'552'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x55f213>|0x093|+0x006:'561'
	// <0x55f219>|0x099|+0x00a:'562'
	// <0x55f223>|0x0a3|+0x012:'563'
	// <0>
	// <1>
	// <2>
	// <0x55f235>|0x0b5|+0x018:'567'
	// <0>
	// <1>
	// <0x55f24d>|0x0cd|-0x008:'570'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55f245>|0x0c5|+0x006:'575'
	// <0x55f24b>|0x0cb|+0x008:'576'
	// <0>
	// <1>
	// <2>
	// <0x55f253>|0x0d3|+0x018:'580'
	// <0>
	// <1>
	// <0x55f26b>|0x0eb|-0x008:'583'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55f263>|0x0e3|+0x006:'588'
	// <0x55f269>|0x0e9|+0x008:'589'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x55f271>|0x0f1|+0x008:'602'
	// <0>
	// <0x55f279>|0x0f9|+0x006:'604'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55f27f>|0x0ff|+0x004:'609'
	// <0x55f283>|0x103|+0x004:'610'
	// <0>
	// <0x55f287>|0x107|+0x00d:'612'
	// <0>
	// <1>
	// <0x55f294>|0x114|+0x029:'615'
	// <0>
	// <1>
	// <0x55f2bd>|0x13d|+0x007:'618'
	// <0>
	// <1>
	// <0x55f2c4>|0x144|+0x010:'621'
	// <0>
	// <0x55f2d4>|0x154|+0x019:'623'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void options::on_config_loaded( resources::queries_result& data )
{
	// LOCALS
	// resources::pinned_ptr_const< u8 > pinned_data
	// memory::reader 					F
	// ******

	// FUNCTION BODY[0x55f350]: 6
	// <0x55f350>|0x000|+0x010:'653'
	// <0>
	// <1>
	// <0x55f360>|0x010|+0x03c:'656'
	// <0x55f39c>|0x04c|+0x010:'657'
	// <0x55f3ac>|0x05c|+0x00d:'658'
	// ******
}

// STATE[STUB]
bool is_line_term( char a )
{
	return false;

	// FUNCTION BODY[0x55c9b0]
	// ******
}

// STATE[STUB]
u32 advance_term_string( memory::reader& F )
{
	return 0;

	// FUNCTION BODY[0x55cd60]: 13
	// <0x55cd68>|0x008|+0x008:'665'
	// <0x55cd70>|0x010|+0x00b:'666'
	// <0>
	// <0x55cd7b>|0x01b|+0x001:'668'
	// <0>
	// <0x55cd7c>|0x01c|+0x018:'670'
	// <0>
	// <0x55cd94>|0x034|+0x014:'672'
	// <0x55cda8>|0x048|-0x016:'673'
	// <0>
	// <1>
	// <0x55cd92>|0x032|+0x01f:'676'
	// <0>
	// ******
}

// STATE[STUB]
void r_string( memory::reader& F, char[4096]& dest )
{
	// FUNCTION BODY[0x55cde0]: 3
	// <0x55cde1>|0x001|+0x003:'682'
	// <0x55cde4>|0x004|+0x005:'683'
	// <0x55cde9>|0x009|+0x015:'684'
	// ******
}

// STATE[STUB]
void options::load_impl( memory::reader& F )
{
	// LOCALS
	// char[4096] 						str
	// ******

	// FUNCTION BODY[0x55f300]: 6
	// <0>
	// <0x55f311>|0x011|+0x00c:'690'
	// <0>
	// <0x55f31d>|0x01d|+0x018:'692'
	// <0x55f335>|0x035|+0x00e:'693'
	// <0x55f343>|0x043|+0x002:'694'
	// ******
}

// STATE[STUB]
void options::fill_global_macros( fixed_vector< shader_macro, 128 >& out_defines )
{
	// LOCALS
	// shader_macro 					d
	// ******

	// CALL SITE INFO
	// <0x55f14c> -> bool < unknown >( shader_macro& ) const
	// ******

	// FUNCTION BODY[0x55f0d0]: 11
	// <0x55f0e1>|0x011|+0x002:'709'
	// <0>
	// <0x55f0e3>|0x013|+0x00d:'711'
	// <0>
	// <0x55f0f0>|0x020|+0x051:'713'
	// <0>
	// <0x55f141>|0x071|+0x011:'715'
	// <0x55f152>|0x082|+0x017:'716'
	// <0>
	// <0x55f169>|0x099|+0x007:'718'
	// <0>
	// ******
}

// STATE[STUB]
void options::load_from_config( configs::binary_config_value const& config )
{
	// LOCALS
	// fs_new::virtual_path_string 		value
	// fs_new::virtual_path_string 		value
	// ******

	// CALL SITE INFO
	// <0x55cf2f> -> void < unknown >( pcstr )
	// <0x55cf67> -> void < unknown >( pcstr )
	// <0x55cf81> -> void < unknown >( pcstr )
	// ******

	// FUNCTION BODY[0x55ce80]: 38
	// <0x55ce86>|0x006|+0x00e:'724'
	// <0x55ce94>|0x014|+0x00c:'725'
	// <0>
	// <0x55cea0>|0x020|+0x013:'727'
	// <0>
	// <0x55ceb3>|0x033|+0x00b:'729'
	// <0>
	// <0x55cebe>|0x03e|+0x004:'731'
	// <0x55cec2>|0x042|+0x0a9:'732'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55cf6b>|0x0eb|-0x09b:'738'
	// <0>
	// <1>
	// <2>
	// <0x55ced0>|0x050|+0x025:'742'
	// <0x55cef5>|0x075|+0x02b:'743'
	// <0x55cf20>|0x0a0|+0x011:'744'
	// <0x55cf31>|0x0b1|+0x002:'745'
	// <0>
	// <1>
	// <2>
	// <0x55cf33>|0x0b3|+0x008:'749'
	// <0x55cf3b>|0x0bb|+0x01d:'750'
	// <0x55cf58>|0x0d8|+0x011:'751'
	// <0x55cf69>|0x0e9|+0x01a:'752'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55cf83>|0x103|+0x008:'758'
	// <0>
	// <0x55cf8b>|0x10b|+0x00b:'760'
	// <0>
	// ******
}

	// TYPEDEFS
	// typedef
	// 	vostok::fs_new::virtual_path_string*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
