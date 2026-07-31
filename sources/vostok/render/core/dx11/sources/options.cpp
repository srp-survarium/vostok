#include "pch.h"
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

math::uint2 parse_resolution( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55ce00]
	return math::uint2( 1, 1 );
}

void options::register_console_commands( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55cfa0]
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

} // namespace render
} // namespace vostok
