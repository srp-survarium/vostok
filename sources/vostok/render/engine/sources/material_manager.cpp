#include "pch.h"
#include "effect_material_base.h"
#include "material_manager.h"
#include "material.h"
#include "post_process_parameters.h"
#include <vostok/render/core/custom_config.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/res_texture.h>

namespace vostok {
namespace render {

class material_shader_base : public effect_material_base {
	material_shader_base(
		enum_vertex_input_type vertex_type,
		material_ptr material
	) :
		m_vertex_type( vertex_type ),
		m_mtl( material )
	{
	}

public:
	virtual ~material_shader_base( ) { }

private:
	u32 m_vertex_type;
	material_ptr m_mtl;
	res_effect_ptr m_shader;
};

material_manager::material_manager( ) :
	m_watcher_materials_subscribe_id			( 0 ),
	m_watcher_material_instanced_subscribe_id	( 0 )
{
	// FUNCTION BODY[0x784bc0]

#ifndef MASTER_GOLD
	m_watcher_materials_subscribe_id	=	vostok::resources::subscribe_watcher(get_materials_path(), 
											boost::bind(&material_manager::on_material_source_changed, this, _1));

	m_watcher_material_instanced_subscribe_id	=	
											vostok::resources::subscribe_watcher(get_material_instances_path(), 
											boost::bind(&material_manager::on_material_source_changed, this, _1));
#endif // #ifndef MASTER_GOLD
}

material_manager::~material_manager( )
{
	// FUNCTION BODY[0x784b40]

#ifndef MASTER_GOLD
	vostok::resources::unsubscribe_watcher	(m_watcher_materials_subscribe_id);
	vostok::resources::unsubscribe_watcher	(m_watcher_material_instanced_subscribe_id);
#endif // #ifndef MASTER_GOLD

	{
		material_effects_entries_type	temp_material_effects;
		m_material_effects.swap			(temp_material_effects);
		m_material_effects.clear		();
	}
//	resources::unregister_cook(resources::material_effects_instance_class);
}

static pcstr resources_materials_string						=	"resources/materials";
static pcstr resources_material_instances_string			=	"resources/material_instances";
static pcstr resources_materials_string_with_slash			=	"resources/materials/";
void material_manager::initialize_post_process_parameters(
	post_process_parameters* out_post_process_parameters_ptr,
	material_ptr mtl,
	bool force_recompilation
)
{
	// FUNCTION BODY[0x784c00]

	VOSTOK_UNREFERENCED_PARAMETER									(force_recompilation);
	
	//remove_post_process_parameters								(out_post_process_parameters_ptr);
	
	u32 crc														= 0;
	custom_config_ptr material_config_ptr						= create_custom_config(mtl->m_config->get_root()["material"], crc, false);
	custom_config_value const& material_config					= material_config_ptr->root();
	
	if (!material_config.value_exists("post_process"))
		return;
	
	fs_new::virtual_path_string mtl_name						= mtl->get_material_name();
	
	m_post_process_parameters[out_post_process_parameters_ptr]	= mtl_name;
	
	custom_config_value const& config							= material_config["post_process"];
	
	post_process_parameters& out_post_process_parameters		= *out_post_process_parameters_ptr;
	out_post_process_parameters									=  post_process_parameters();
	
	if (config.value_exists("dof_height_lights"))
		out_post_process_parameters.dof_height_lights			= float3(config["dof_height_lights"]["value"]);
	
	if (config.value_exists("dof_focus_power"))
		out_post_process_parameters.dof_focus_power				= float(config["dof_focus_power"]["value"]);
	
	if (config.value_exists("dof_focus_region"))
		out_post_process_parameters.dof_focus_region			= float(config["dof_focus_region"]["value"]);

	if (config.value_exists("dof_focus_distance"))
		out_post_process_parameters.dof_focus_distance			= float(config["dof_focus_distance"]["value"]);

	if (config.value_exists("dof_near_blurriness_amount"))
		out_post_process_parameters.dof_near_blur_amount		= float(config["dof_near_blurriness_amount"]["value"]);

	if (config.value_exists("dof_near_blurriness_far"))
		out_post_process_parameters.dof_far_blur_amount			= float(config["dof_near_blurriness_far"]["value"]);
	
	if (config.value_exists("use_bokeh_dof"))
		out_post_process_parameters.use_bokeh_dof				= bool(config["use_bokeh_dof"]["value"]);
	
	if (config.value_exists("bokeh_dof_radius"))
		out_post_process_parameters.bokeh_dof_radius			= float(config["bokeh_dof_radius"]["value"]);

	if (config.value_exists("bokeh_dof_density"))
		out_post_process_parameters.bokeh_dof_density			= float(config["bokeh_dof_density"]["value"]);

	if (config.value_exists("use_bokeh_image"))
		out_post_process_parameters.use_bokeh_image				= bool(config["use_bokeh_image"]["value"]);
	
	if (config.value_exists("enable_ssao"))
		out_post_process_parameters.enable_ssao					= bool(config["enable_ssao"]["value"]);
	
	if (config.value_exists("ssao_saturation"))
		out_post_process_parameters.ssao_saturation				= float(config["ssao_saturation"]["value"]);
	
	if (config.value_exists("ssao_radius_scale"))
		out_post_process_parameters.ssao_radius_scale			= float(config["ssao_radius_scale"]["value"]);
	
	if (config.value_exists("bloom_scale"))
		out_post_process_parameters.bloom_scale					= float(config["bloom_scale"]["value"]);

	if (config.value_exists("bloom_max_color"))
		out_post_process_parameters.bloom_max_color				= float(config["bloom_max_color"]["value"]);

	if (config.value_exists("frame_desaturation"))
		out_post_process_parameters.frame_desaturation			= float(config["frame_desaturation"]["value"]);

	if (config.value_exists("frame_height_lights"))
		out_post_process_parameters.frame_height_lights			= float3(config["frame_height_lights"]["value"]);
	
	if (config.value_exists("frame_mid_tones"))
		out_post_process_parameters.frame_mid_tones				= float3(config["frame_mid_tones"]["value"]);

	if (config.value_exists("frame_shadows"))
		out_post_process_parameters.frame_shadows				= float3(config["frame_shadows"]["value"]);

	if (config.value_exists("environment_far_fog_color") &&
		config.value_exists("environment_far_fog_distance") &&
		config.value_exists("environment_near_fog_distance"))
	{
		out_post_process_parameters.environment_far_fog_color		= float4(config["environment_far_fog_color"]["value"]).xyz();
		out_post_process_parameters.environment_far_fog_distance	= float(config["environment_far_fog_distance"]["value"]);
		out_post_process_parameters.environment_near_fog_distance	= float(config["environment_near_fog_distance"]["value"]);
	}
	
	if (config.value_exists("environment_ambient_color"))
		out_post_process_parameters.environment_ambient_color		= float4(config["environment_ambient_color"]["value"]).xyz();
	
	if (config.value_exists("environment_sun_color"))
		out_post_process_parameters.environment_sun_color			= float4(config["environment_sun_color"]["value"]).xyz();
	
	if (config.value_exists("environment_shadow_transparency"))
		out_post_process_parameters.environment_shadow_transparency	= float(config["environment_shadow_transparency"]["value"]);
	
	if (config.value_exists("environment_skylight_upper_color"))
		out_post_process_parameters.environment_skylight_upper_color	= float4(config["environment_skylight_upper_color"]["value"]);	
	
	if (config.value_exists("environment_skylight_lower_color"))
		out_post_process_parameters.environment_skylight_lower_color	= float4(config["environment_skylight_lower_color"]["value"]);	

	if (
		config.value_exists("environment_skylight_upper_power")	&&
		config.value_exists("environment_skylight_lower_power")
		)
		out_post_process_parameters.environment_skylight_parameters		= float4(
			float(config["environment_skylight_upper_power"]["value"]),
			float(config["environment_skylight_lower_power"]["value"]),
			0.0f,
			0.0f
		);	
	
	if (config.value_exists("adaptation_speed"))
		out_post_process_parameters.adaptation_speed			= math::max(float(config["adaptation_speed"]["value"]), 0.25f);
	
	if (config.value_exists("tonemap_bright_threshold"))
		out_post_process_parameters.tonemap_bright_threshold	= float(config["tonemap_bright_threshold"]["value"]);
	if (config.value_exists("tonemap_median"))
		out_post_process_parameters.tonemap_median				= float(config["tonemap_median"]["value"]);
	if (config.value_exists("tonemap_darkness_threshold"))
		out_post_process_parameters.tonemap_darkness_threshold	= float(config["tonemap_darkness_threshold"]["value"]);
	if (config.value_exists("tonemap_middle_gray"))
		out_post_process_parameters.tonemap_middle_gray			= float(config["tonemap_middle_gray"]["value"]);
	
	
	
	if (config.value_exists("wind_direction")) 
		out_post_process_parameters.wind_direction				= float3(config["wind_direction"]["value"]);
	
	if (config.value_exists("wind_strength")) 
		out_post_process_parameters.wind_strength				= float(config["wind_strength"]["value"]);

	if (config.value_exists("enable_bloom"))
		out_post_process_parameters.enable_bloom				= bool(config["enable_bloom"]["value"]);

	if (config.value_exists("environment_skycolor_up"))
		out_post_process_parameters.environment_skycolor[3]		= float4(config["environment_skycolor_up"]["value"]);
	
	if (config.value_exists("environment_skycolor_x"))
		out_post_process_parameters.environment_skycolor[1]		= float4(config["environment_skycolor_x"]["value"]);
	
	if (config.value_exists("environment_skycolor_minus_x"))
		out_post_process_parameters.environment_skycolor[0]		= float4(config["environment_skycolor_minus_x"]["value"]);
	
	if (config.value_exists("environment_skycolor_z"))
		out_post_process_parameters.environment_skycolor[5]		= float4(config["environment_skycolor_z"]["value"]);
	
	if (config.value_exists("environment_skycolor_minus_z"))
		out_post_process_parameters.environment_skycolor[4]		= float4(config["environment_skycolor_minus_z"]["value"]);
	
	if (config.value_exists("environment_use_skycolor"))
		out_post_process_parameters.use_environment_skycolor	= bool(config["environment_use_skycolor"]["value"]);
	
#pragma message( VOSTOK_TODO("Add color_grading_texture to post_process_parameters in other platforms") )
	
	if (config.value_exists("use_color_grading_lut"))
		out_post_process_parameters.use_color_grading_lut		= bool(config["use_color_grading_lut"]["value"]);
	
#if VOSTOK_PLATFORM_WINDOWS
	if (config.value_exists("color_grading_texture"))
		out_post_process_parameters.color_grading_texture		= resource_manager::ref().create_texture(pcstr(config["color_grading_texture"]["value"]), 0, 0, false, true, true, 0); // buildability: legacy 1-arg call, canonical grew to 7 params
#endif // #if VOSTOK_PLATFORM_WINDOWS
}

void material_manager::remove_material_effects(
	material_effects_instance_ptr const& in_material_effects_instance
)
{
	// FUNCTION BODY[0x784990]

	material_effects_entries_type::iterator it_begin	= m_material_effects.begin();
	material_effects_entries_type::iterator it_end		= m_material_effects.end();
	material_effects_entries_type::iterator it			= it_begin;
	
	for ( ; it != it_end; ++it)
	{
		if (*it->m_material_effects_instance_ptr == in_material_effects_instance)
			m_material_effects.erase(it);
	}
}

void material_manager::add_material_effects(
	material_effects_instance_ptr& in_material_effects_instance,
	fs_new::virtual_path_string const& in_material_name
)
{
	// FUNCTION BODY[0x7849f0]

	material_effects_entries_type::iterator it_begin	= m_material_effects.begin();
	material_effects_entries_type::iterator it_end		= m_material_effects.end();
	material_effects_entries_type::iterator it			= it_begin;
	
	for ( ; it != it_end; ++it)
	{
		if (*it->m_material_effects_instance_ptr == in_material_effects_instance)
			return;
	}
	
	m_material_effects.push_back(
		material_effects_entry(
			&in_material_effects_instance, 
			in_material_name
		)
	);
}

} // namespace render
} // namespace vostok
