#include "pch.h"
// claude@NOTE: legacy-harvest disposition: the remaining stubs have no legacy ancestor - legacy render_cc_bool/float/u32 override only fill_macro (no execute/is_changed/m_prev_value machinery), and parse_resolution/begin-end_render_options_changing are new-in-target - matcher-phase work.
#include <vostok/math_int2.h>
#include <vostok/render/core/options.h>
#include <vostok/console_command_processor.h>

namespace vostok {
namespace render {

render_cc::render_cc(
	pcstr define_name,
	enum_options_changes_result changed_result
)
	: render_next( 0 ),
	  m_define_name( define_name ),
	  m_changes_result( changed_result )
{
	// FUNCTION BODY[0x55cdc0]
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
	console_commands::command_type command_type
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
	// FUNCTION BODY[0x12b550]
}

void render_cc_bool::execute( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b5b0]
}

bool render_cc_bool::is_changed( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b5c0]
	return false;
}

bool render_cc_bool::fill_macro( shader_macro& out_macro ) const
{
	// FUNCTION BODY[0x12b5d0]
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
	float min,
	float max,
	bool serializable,
	console_commands::command_type command_type
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
	// FUNCTION BODY[0x12b630]
}

void render_cc_float::execute( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b690]
}

bool render_cc_float::is_changed( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b6a0]
	return false;
}

bool render_cc_float::fill_macro( shader_macro& out_macro ) const
{
	// FUNCTION BODY[0x12b6c0]
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
	u32 min,
	u32 max,
	bool serializable,
	console_commands::command_type command_type
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
	// FUNCTION BODY[0x12b720]
}

void render_cc_u32::execute( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b780]
}

bool render_cc_u32::is_changed( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x12b790]
	return false;
}

bool render_cc_u32::fill_macro( shader_macro& out_macro ) const
{
	// FUNCTION BODY[0x12b7a0]
	if (define_name())
	{
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
	// FUNCTION BODY[0x55f0a0]
	register_console_commands();
	set_default_values();
}

// claude@NOTE: legacy defaults only; target-added table fields (clouds/
// fxaa-quality/grass/ssao/quality tiers/resolution) get their defaults from
// the immediates at 0x55c9d0 - matcher work.
void options::set_default_values( )
{
	// FUNCTION BODY[0x55c9d0]
	current.m_enabled_g_stage					= true;
	current.m_enabled_g_stage_pre_pass			= true;
	current.m_enabled_g_stage_material_pass		= true;
	current.m_enabled_decals_accumulate_stage	= true;
	current.m_enabled_distortion_stage			= true;
	current.m_enabled_sun_shadows_stage			= true;
	current.m_enabled_sun_stage					= true;
	current.m_enabled_lighting_stage			= true;
	current.m_enabled_forward_lighting_stage	= true;
	current.m_enabled_deferred_lighting_stage	= true;
	current.m_enabled_forward_stage				= true;
	current.m_enabled_particles_stage			= true;
	current.m_enabled_post_process_stage		= true;
	current.m_enabled_sky_box_stage				= true;
	current.m_test_float_option					= 1.0f;
	current.m_use_parallax						= true;
	current.m_enabled_ambient_occlusion_stage	= true;
	current.m_enabled_light_propagation_volumes_stage = true;
	current.m_enabled_mlaa						= true;
	current.m_use_cpu_mlaa						= false;
	current.m_shadow_map_size					= 1024;
	current.m_spot_shadow_map_size				= 1024;
	current.m_organic_irradiance_texture_size	= 1024;
	current.m_shadow_quality					= 1;
	current.m_enabled_local_light_shadows		= true;
	current.m_enabled_terrain_shadows			= true;
	current.m_enabled_draw_terrain				= true;
	current.m_enabled_draw_speedtree			= true;
	current.m_enabled_draw_speedtree_billboards	= true;
	current.m_enabled_draw_speedtree_branches	= true;
	current.m_enabled_draw_speedtree_fronds		= true;
	current.m_enabled_draw_speedtree_leafcards	= true;
	current.m_enabled_draw_speedtree_leafmeshes	= true;
	current.m_enabled_fxaa						= false;

	current.m_light_propagation_volumes_rsm_size= 128;
	current.m_num_radiance_volume_cells			= 32;
	current.m_radiance_volume_scale				= 10.0f;
	current.m_num_propagate_iterations			= 2;
	current.m_enabled_lpv_occluders				= true;

	current.m_lpv_flux_amplifier				= 3.5f;
	current.m_lpv_interreflection_contribution	= 1.5f;

	current.m_lpv_movable						= true;

	current.m_lpv_num_cascades					= 3;

	current.m_lpv_gather_occluders_from_light_view = true;
	current.m_lpv_gather_occluders_from_camera_view = true;

	current.m_lpv_disable_rsm_generating		= false;
	current.m_lpv_disable_rsm_downsampling		= false;
	current.m_lpv_disable_vpl_injection			= false;
	current.m_lpv_disable_gv_injection			= false;
	current.m_lpv_disable_propagation			= false;
	current.m_lpv_disable_lpv_lookup			= false;

	current.m_lpv_refresh_once_per_frames		= 5;
	current.m_lpv_occlusion_amplifier			= 1.0f;

	current.m_enabled_draw_models				= true;
	current.m_enabled_clouds_stage				= true;

	current.m_num_test_lights					= 1024;
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

math::uint2 parse_resolution( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55ce00]
	return math::uint2( 1, 1 );
}

string256 s_current_render_configuration = "default";

// claude@NOTE: legacy registrations adapted to the canonical two-table
// (current/previous) signature; every changed_result is the ocr_need_nothing
// placeholder - the real per-cc values live in the immediates at 0x55cfa0
// (matcher work), as do the target-added ccs (clouds/fxaa/grass/ssao/quality
// tiers/resolution/...). Legacy ccs on fields dropped from optinos_table
// (use_branching, use_loop_unrolling, gbuffer_pos_packing,
// gbuffer_normal_packing) are dropped with their fields.
void options::register_console_commands( )
{
	// FUNCTION BODY[0x55cfa0]
	using namespace console_commands;

	static cc_string current_render_configuration_cc("r_current_render_configuration", s_current_render_configuration, 256, true, command_type_engine_internal );

	static render_cc_bool	enabled_g_stage_cc					("r_enabled_g_stage",					ocr_need_nothing, 0, current.m_enabled_g_stage,					previous.m_enabled_g_stage,					true, command_type_engine_internal);
	static render_cc_bool	enabled_g_stage_pre_pass_cc			("r_enabled_g_stage_pre_pass",			ocr_need_nothing, 0, current.m_enabled_g_stage_pre_pass,		previous.m_enabled_g_stage_pre_pass,		true, command_type_engine_internal);
	static render_cc_bool	enabled_g_stage_material_pass_cc	("r_enabled_g_stage_material_pass",		ocr_need_nothing, 0, current.m_enabled_g_stage_material_pass,	previous.m_enabled_g_stage_material_pass,	true, command_type_engine_internal);
	static render_cc_bool	enabled_decals_accumulate_stage_cc	("r_enabled_decals_accumulate_stage",	ocr_need_nothing, 0, current.m_enabled_decals_accumulate_stage,	previous.m_enabled_decals_accumulate_stage,	true, command_type_engine_internal);
	static render_cc_bool	enabled_distortion_stage_cc			("r_enabled_distortion_stage",			ocr_need_nothing, 0, current.m_enabled_distortion_stage,		previous.m_enabled_distortion_stage,		true, command_type_engine_internal);
	static render_cc_bool	enabled_sun_shadows_stage_cc		("r_enabled_sun_shadows_stage",			ocr_need_nothing, 0, current.m_enabled_sun_shadows_stage,		previous.m_enabled_sun_shadows_stage,		true, command_type_engine_internal);
	static render_cc_bool	enabled_sun_stage_cc				("r_enabled_sun_stage",					ocr_need_nothing, 0, current.m_enabled_sun_stage,				previous.m_enabled_sun_stage,				true, command_type_engine_internal);
	static render_cc_bool	enabled_lighting_stage_cc			("r_enabled_lighting_stage",			ocr_need_nothing, 0, current.m_enabled_lighting_stage,			previous.m_enabled_lighting_stage,			true, command_type_engine_internal);
	static render_cc_bool	enabled_forward_lighting_stage_cc	("r_enabled_forward_lighting_stage",	ocr_need_nothing, 0, current.m_enabled_forward_lighting_stage,	previous.m_enabled_forward_lighting_stage,	true, command_type_engine_internal);
	static render_cc_bool	enabled_deferred_lighting_stage_cc	("r_enabled_deferred_lighting_stage",	ocr_need_nothing, 0, current.m_enabled_deferred_lighting_stage,	previous.m_enabled_deferred_lighting_stage,	true, command_type_engine_internal);
	static render_cc_bool	enabled_forward_stage_cc			("r_enabled_forward_stage",				ocr_need_nothing, 0, current.m_enabled_forward_stage,			previous.m_enabled_forward_stage,			true, command_type_engine_internal);
	static render_cc_bool	enabled_particles_stage_cc			("r_enabled_particles_stage",			ocr_need_nothing, 0, current.m_enabled_particles_stage,			previous.m_enabled_particles_stage,			true, command_type_engine_internal);
	static render_cc_bool	enabled_post_process_stage_cc		("r_enabled_post_process_stage",		ocr_need_nothing, 0, current.m_enabled_post_process_stage,		previous.m_enabled_post_process_stage,		true, command_type_engine_internal);
	static render_cc_bool	enabled_sky_box_stage_cc			("r_enabled_sky_box_stage",				ocr_need_nothing, 0, current.m_enabled_sky_box_stage,			previous.m_enabled_sky_box_stage,			true, command_type_engine_internal);
	static render_cc_bool	enabled_ambient_occlusion_stage_cc	("r_enabled_ambient_occlusion_stage",	ocr_need_nothing, 0, current.m_enabled_ambient_occlusion_stage,	previous.m_enabled_ambient_occlusion_stage,	true, command_type_engine_internal);
	static render_cc_bool	enabled_clouds_stage_cc				("r_enabled_clouds_stage",				ocr_need_nothing, 0, current.m_enabled_clouds_stage,			previous.m_enabled_clouds_stage,			true, command_type_engine_internal);

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

	static render_cc_bool	enabled_fxaa_cc						("r_enabled_fxaa",						ocr_need_nothing, 0, current.m_enabled_fxaa,					previous.m_enabled_fxaa,					true, command_type_engine_internal);
	static render_cc_bool	enabled_mlaa_cc						("r_enabled_mlaa",						ocr_need_nothing, 0, current.m_enabled_mlaa,					previous.m_enabled_mlaa,					true, command_type_engine_internal);
	static render_cc_bool	use_cpu_mlaa_cc						("r_use_cpu_mlaa",						ocr_need_nothing, 0, current.m_use_cpu_mlaa,					previous.m_use_cpu_mlaa,					false, command_type_engine_internal);

	// Lighting stage parameters.
	static render_cc_bool	enabled_local_light_shadows_cc		("r_enabled_local_light_shadows",		ocr_need_nothing, 0, current.m_enabled_local_light_shadows,		previous.m_enabled_local_light_shadows,		true, command_type_engine_internal);

	static render_cc_bool	use_parallax_cc						("r_use_parallax",						ocr_need_nothing, "GLOBAL_ALLOW_STEEPPARALLAX",				current.m_use_parallax, previous.m_use_parallax, true, command_type_engine_internal);

	static render_cc_u32	organic_irradiance_texture_size_cc	("r_organic_irradiance_texture_size",	ocr_need_nothing, "GLOBAL_ORGANIC_IRRADIANCE_TEXTURE_SIZE",	current.m_organic_irradiance_texture_size, previous.m_organic_irradiance_texture_size, 128, 2048, true, command_type_engine_internal);
	static render_cc_u32	shadow_map_size_cc					("r_shadow_map_size",					ocr_need_nothing, "GLOBAL_SHADOWMAP_SIZE",					current.m_shadow_map_size, previous.m_shadow_map_size, 128, 2048, true, command_type_engine_internal);
	static render_cc_u32	spot_shadow_map_size_cc				("r_spot_shadow_map_size",				ocr_need_nothing, "GLOBAL_SPOT_SHADOWMAP_SIZE",				current.m_spot_shadow_map_size, previous.m_spot_shadow_map_size, 128, 2048, true, command_type_engine_internal);
	static render_cc_u32	shadow_quality_cc					("r_shadow_quality",					ocr_need_nothing, "GLOBAL_SHADOWMAP_QUALITY",				current.m_shadow_quality, previous.m_shadow_quality, 0, 2, true, command_type_engine_internal);
	static render_cc_float  test_float_option_cc				("r_test_float_option",					ocr_need_nothing, "GLOBAL_TEST_FLOAT",						current.m_test_float_option, previous.m_test_float_option, 0.0f, 100.0f, true, command_type_engine_internal);

	// Light Propagation Volumes stage parameters.
	static render_cc_bool	enabled_light_propagation_volumes_stage_cc	("r_enabled_light_propagation_volumes_stage",	ocr_need_nothing, 0, current.m_enabled_light_propagation_volumes_stage, previous.m_enabled_light_propagation_volumes_stage, true, command_type_engine_internal);
	static render_cc_u32	light_propagation_volumes_rsm_size_cc		("r_light_propagation_volumes_rsm_size",		ocr_need_nothing, 0, current.m_light_propagation_volumes_rsm_size, previous.m_light_propagation_volumes_rsm_size, 1, 2048, true, command_type_engine_internal);
	static render_cc_u32	num_radiance_volume_cells_cc				("r_num_radiance_volume_cells",					ocr_need_nothing, 0, current.m_num_radiance_volume_cells,	previous.m_num_radiance_volume_cells,	8, 128, true, command_type_engine_internal);
	static render_cc_u32	num_propagate_iterations_cc					("r_num_propagate_iterations",					ocr_need_nothing, 0, current.m_num_propagate_iterations,	previous.m_num_propagate_iterations,	0, 32, true, command_type_engine_internal);
	static render_cc_float	radiance_volume_scale_cc					("r_radiance_volume_scale",						ocr_need_nothing, 0, current.m_radiance_volume_scale,		previous.m_radiance_volume_scale,		1.0f, 100.0f, true, command_type_engine_internal);

	static render_cc_float	lpv_flux_amplifier_cc						("r_lpv_flux_amplifier",						ocr_need_nothing, 0, current.m_lpv_flux_amplifier,			previous.m_lpv_flux_amplifier,			0.0f, 100.0f, true, command_type_engine_internal);
	static render_cc_float	lpv_interreflection_contribution_cc			("r_lpv_interreflection_contribution",			ocr_need_nothing, 0, current.m_lpv_interreflection_contribution, previous.m_lpv_interreflection_contribution, 0.0f, 100.0f, true, command_type_engine_internal);
	static render_cc_bool	lpv_movable_cc								("r_lpv_movable",								ocr_need_nothing, 0, current.m_lpv_movable,					previous.m_lpv_movable,					true, command_type_engine_internal);

	static render_cc_u32	lpv_num_cascades_cc							("r_lpv_num_cascades",							ocr_need_nothing, 0, current.m_lpv_num_cascades,			previous.m_lpv_num_cascades,			1, 4, true, command_type_engine_internal);

	static render_cc_bool	lpv_gather_occluders_from_light_view_cc		("r_lpv_gather_occluders_from_light_view",		ocr_need_nothing, 0, current.m_lpv_gather_occluders_from_light_view, previous.m_lpv_gather_occluders_from_light_view, true, command_type_engine_internal);
	static render_cc_bool	lpv_gather_occluders_from_camera_view_cc	("r_lpv_gather_occluders_from_camera_view",		ocr_need_nothing, 0, current.m_lpv_gather_occluders_from_camera_view, previous.m_lpv_gather_occluders_from_camera_view, true, command_type_engine_internal);

	static render_cc_bool	lpv_disable_rsm_generating_cc				("r_lpv_disable_rsm_generating",				ocr_need_nothing, 0, current.m_lpv_disable_rsm_generating,	previous.m_lpv_disable_rsm_generating,	false, command_type_engine_internal);
	static render_cc_bool	lpv_disable_rsm_downsampling_cc				("r_lpv_disable_rsm_downsampling",				ocr_need_nothing, 0, current.m_lpv_disable_rsm_downsampling, previous.m_lpv_disable_rsm_downsampling, false, command_type_engine_internal);
	static render_cc_bool	lpv_disable_vpl_injection_cc				("r_lpv_disable_vpl_injection",					ocr_need_nothing, 0, current.m_lpv_disable_vpl_injection,	previous.m_lpv_disable_vpl_injection,	false, command_type_engine_internal);
	static render_cc_bool	lpv_disable_gv_injection_cc					("r_lpv_disable_gv_injection",					ocr_need_nothing, 0, current.m_lpv_disable_gv_injection,	previous.m_lpv_disable_gv_injection,	false, command_type_engine_internal);
	static render_cc_bool	lpv_disable_propagation_cc					("r_lpv_disable_propagation",					ocr_need_nothing, 0, current.m_lpv_disable_propagation,		previous.m_lpv_disable_propagation,		false, command_type_engine_internal);
	static render_cc_bool	lpv_disable_lpv_lookup_cc					("r_lpv_disable_lpv_lookup",					ocr_need_nothing, 0, current.m_lpv_disable_lpv_lookup,		previous.m_lpv_disable_lpv_lookup,		false, command_type_engine_internal);

	static render_cc_u32	lpv_refresh_once_per_frames_cc				("r_lpv_refresh_once_per_frames",				ocr_need_nothing, 0, current.m_lpv_refresh_once_per_frames,	previous.m_lpv_refresh_once_per_frames,	1, 20, true, command_type_engine_internal);
	static render_cc_float	lpv_occlusion_amplifier_cc					("r_lpv_occlusion_amplifier",					ocr_need_nothing, 0, current.m_lpv_occlusion_amplifier,		previous.m_lpv_occlusion_amplifier,		0.0f, 100.0f, true, command_type_engine_internal);

	static render_cc_u32	num_test_lights_cc							("r_num_test_lights",							ocr_need_nothing, 0, current.m_num_test_lights,				previous.m_num_test_lights,				16, 1024 * 1024, true, command_type_engine_internal);

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

void options::begin_render_options_changing( long volatile* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55cd50]
}

enum_options_changes_result options::end_render_options_changing(
	vector<fs_new::virtual_path_string>&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f180]
	return ocr_need_nothing;
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
	// FUNCTION BODY[0x55f350]
	if( !data.is_successful( ) )
		return;

	resources::pinned_ptr_const<u8> pinned_data	(data[ 0 ].get_managed_resource( ));
	memory::reader				F( pinned_data.c_ptr( ), pinned_data.size( ) );
	load_impl					( F );
}

bool is_line_term( char a )
{
	// FUNCTION BODY[0x55c9b0]
	return (a==13)||(a==10);
}

u32 advance_term_string( memory::reader& F )
{
	// FUNCTION BODY[0x55cd60]
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

void r_string( memory::reader& F, char (&dest)[4096] )
{
	// FUNCTION BODY[0x55cde0]
	char *src 	= (char *) F.pointer();
	u32 sz 		= advance_term_string(F);
	vostok::strings::copy_n	(dest, sizeof(dest), src, sz);
}

void options::load_impl( memory::reader& F )
{
	// FUNCTION BODY[0x55f300]
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
	// FUNCTION BODY[0x55f0d0]
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
	// FUNCTION BODY[0x55ce80]
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

// claude@NOTE: legacy body was gated #ifndef MASTER_GOLD; the canonical class
// declares it unconditionally and load_from_config ships (0x55ce80), so the
// gate is dropped.
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
