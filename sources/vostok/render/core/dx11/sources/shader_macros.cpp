#include "pch.h"
#include <vostok/render/core/shader_macros.h>
#include <vostok/render/core/options.h>

#include <algorithm>

namespace vostok {
namespace render {

pcstr const str_global_use_branching				=	"GLOBAL_USE_BRANCHING";
pcstr const str_global_allow_steepparallax			=	"GLOBAL_ALLOW_STEEPPARALLAX";
pcstr const str_global_master_gold					=	"GLOBAL_MASTER_GOLD";
pcstr const str_global_num_max_light_instances		=	"GLOBAL_NUM_MAX_LIGHT_INSTANCES";
pcstr const str_global_cascaded_shadow_map_size		=	"GLOBAL_CASCADED_SHADOW_MAP_SIZE";
pcstr const str_global_use_god_rays					=	"GLOBAL_USE_GOD_RAYS";
pcstr const str_global_shadowmap_size				=	"GLOBAL_SHADOWMAP_SIZE";
pcstr const str_global_spot_shadowmap_size			=	"GLOBAL_SPOT_SHADOWMAP_SIZE";
pcstr const str_post_process_quality				=	"GLOBAL_POST_PROCESS_QUALITY";
pcstr const str_lighting_quality					=	"GLOBAL_LIGHTING_QUALITY";
pcstr const str_shading_quality						=	"GLOBAL_SHADING_QUALITY";

pcstr const str_cfg_use_diffuse						=	"CONFIG_TDIFFUSE";
pcstr const str_cfg_use_anisotropic_direction		=	"CONFIG_TANISOTROPIC_DIRECTION";
pcstr const str_cfg_use_normal						=	"CONFIG_TNORMAL";
pcstr const str_cfg_use_detail_normal				=	"CONFIG_TDETAIL_NORMAL";
pcstr const str_cfg_use_alpha_test					=	"CONFIG_ALPHA_TEST";
pcstr const str_cfg_use_parallax					=	"CONFIG_PARALLAX";
pcstr const str_cfg_use_trancluency_texture			=	"CONFIG_TRANSLUCENCY";
pcstr const str_cfg_use_specular_color_texture		=	"CONFIG_TSPECULAR_COLOR";
pcstr const str_cfg_use_fresnel_texture				=	"CONFIG_TFRESNEL";
pcstr const str_cfg_use_roughness_texture			=	"CONFIG_TROUGHNESS";
pcstr const str_cfg_is_anisotropic_material			=	"CONFIG_ANISOTROPIC_MATERIAL";
pcstr const str_cfg_use_emissive					=	"CONFIG_EMISSIVE";
pcstr const str_cfg_use_diffuse_power_texture		=	"CONFIG_TDIFFUSE_POWER";
pcstr const str_cfg_use_detail						=	"CONFIG_TDETAIL";
pcstr const str_cfg_use_detail_bump					=	"CONFIG_TDETAIL_BUMP";
pcstr const str_cfg_use_alphablended_diffuse		=	"CONFIG_TALPHABLENDED_DIFFUSE";
pcstr const str_cfg_use_alphablended_normal			=	"CONFIG_TALPHABLENDED_NORMAL";
pcstr const str_cfg_use_vertex_blended_diffuse		=	"CONFIG_VERTEX_BLENDED_DIFFUSE";
pcstr const str_cfg_use_vertex_blended_mask			=	"CONFIG_VERTEX_BLENDED_MASK";
pcstr const str_cfg_use_vertex_blended_normal		=	"CONFIG_VERTEX_BLENDED_NORMAL";
pcstr const str_cfg_use_vertex_blended_specular		=	"CONFIG_VERTEX_BLENDED_SPECULAR";
pcstr const str_cfg_use_subuv						=	"CONFIG_USE_SUBUV";
pcstr const str_cfg_use_reflection					=	"CONFIG_REFLECTION";
pcstr const str_cfg_use_ttransparency				=	"CONFIG_TTRANSPARENCY";
pcstr const str_cfg_use_grass_fresnel_effect		=	"CONFIG_USE_GRASS_FRESNEL_EFFECT";
pcstr const str_cfg_light_type						=	"CONFIG_LIGHT_TYPE";
pcstr const str_cfg_vertex_input_type				=	"CONFIG_VERTEX_INPUT_TYPE";
pcstr const str_cfg_post_process_blur_kernel		=	"CONFIG_POST_PROCESS_BLUR_KERNEL";
pcstr const str_cfg_lod_index						=	"CONFIG_LOD_INDEX";
pcstr const str_cfg_use_specular_lighting			=	"CONFIG_USE_SPECULAR_LIGHTING";
pcstr const str_cfg_use_variation_mask				=	"CONFIG_VARIATION_MASK";
pcstr const str_cfg_use_shadowed_light				=	"CONFIG_SHADOWED_LIGHT";
pcstr const str_cfg_use_bokeh_dof					=	"CONFIG_USE_BOKEH_DOF";
pcstr const str_cfg_use_bokeh_image					=	"CONFIG_USE_BOKEH_IMAGE";
pcstr const str_cfg_wind_motion						=	"CONFIG_WIND_MOTION";
pcstr const str_cfg_use_bound_normals				=	"CONFIG_USE_BOUND_NORMALS";
pcstr const str_cfg_use_up_directional_normals		=	"CONFIG_USE_UP_DIRECTIONAL_NORMALS";
pcstr const str_cfg_use_diffuse_as_specular			=	"CONFIG_USE_DIFFUSE_AS_SPECULAR";
pcstr const str_cfg_num_used_terrain_layers			=	"CONFIG_NUM_USED_TERRAIN_LAYERS";
pcstr const str_cfg_used_terrain_height_mask		=	"CONFIG_USED_TERRAIN_HEIGHT_MASK";
pcstr const str_cfg_lighting_model					=	"CONFIG_LIGHTING_MODEL";
pcstr const str_cfg_decal_material					=	"CONFIG_DECAL_MATERIAL";
pcstr const str_cfg_decal_type						=	"CONFIG_DECAL_TYPE";
pcstr const str_cfg_reflection_mask					=	"CONFIG_REFLECTION_MASK";
pcstr const str_cfg_use_ao_texture					=	"CONFIG_USE_AO_TEXTURE";
pcstr const str_cfg_local_reflections				=	"CONFIG_LOCAL_REFLECTIONS";
pcstr const str_cfg_use_indirect_specular			=	"CONFIG_USE_INDIRECT_SPECULAR";
pcstr const str_cfg_use_image_grain					=	"CONFIG_USE_IMAGE_GRAIN";
pcstr const str_cfg_env_probe_clip_by_normal		=	"CONFIG_ENV_PROBE_CLIP_BY_NORMAL";
pcstr const str_cfg_env_probe_with_shadows			=	"CONFIG_ENV_PROBE_WITH_SHADOWS";
pcstr const str_cfg_env_probe_geometry_type			=	"CONFIG_ENV_PROBE_GEOMETRY_TYPE";
pcstr const str_cfg_use_diffuse_masked_color		=	"CONFIG_USE_DIFFUSE_MASKED_COLOR";
pcstr const str_cfg_use_diffuse_masked_color_by_hue	=	"CONFIG_USE_DIFFUSE_MASKED_COLOR_BY_HUE";
pcstr const str_cfg_use_uv_scrolling				=	"CONFIG_USE_UV_SCROLLING";
pcstr const str_cfg_use_normal_waves				=	"CONFIG_USE_NORMAL_WAVES";
pcstr const str_cfg_use_fuzziness					=	"CONFIG_USE_FUZZINESS";
pcstr const str_cfg_terrain_blend_mode				=	"CONFIG_TERRAIN_BLEND_MODE";
pcstr const str_cfg_use_soft_edges					=	"CONFIG_USE_SOFT_EDGES";
pcstr const str_cfg_use_thickness_map				=	"CONFIG_USE_THICKNESS_MAP";
pcstr const str_cfg_use_subsurface_scattering_mask_map = "CONFIG_USE_SUBSURFACE_SCATTERING_MASK_MAP";
pcstr const str_cfg_use_olta						=	"CONFIG_USE_OLTA";
pcstr const str_cfg_use_sequence					=	"CONFIG_SEQUENCE";

// for organic materials (skin, body, meat, etc)
pcstr const str_cfg_use_organic_scattering_amount_mask		= "CONFIG_USE_ORGANIC_SCATTERING_AMOUNT_MASK";
pcstr const str_cfg_use_organic_scattering_depth_texture	= "CONFIG_USE_ORGANIC_SCATTERING_DEPTH_TEXTURE";
pcstr const str_cfg_use_organic_back_illumination_texture	= "CONFIG_USE_ORGANIC_BACK_ILLUMINATION_TEXTURE";
pcstr const str_cfg_use_organic_subdermal_texture			= "CONFIG_USE_ORGANIC_SUBDERMAL_TEXTURE";

namespace {

void add_bool_macro(
	shader_defines_list& macros,
	pcstr name,
	bool flag
)
{
	macros.push_back( shader_macro() );
	macros.back().name       = name;
	macros.back().definition = flag ? "1" : "0";
}

void add_u8_macro(
	shader_defines_list& macros,
	pcstr name,
	u8 value
)
{
	shader_macro macro;
	macro.name       = name;
	macro.definition.assignf( "%d", value );
	macros.push_back( macro);
}

} // namespace

shader_macros::shader_macros( )
{
	register_available_macros();
}

void shader_macros::register_available_macros( )
{
	// global flags
	m_available_macros.push_back(str_global_use_branching);
	m_available_macros.push_back(str_global_allow_steepparallax);
	m_available_macros.push_back(str_global_master_gold);
	m_available_macros.push_back(str_global_num_max_light_instances);
	m_available_macros.push_back(str_global_cascaded_shadow_map_size);
	m_available_macros.push_back(str_global_use_god_rays);
	m_available_macros.push_back(str_global_shadowmap_size);
	m_available_macros.push_back(str_global_spot_shadowmap_size);
	m_available_macros.push_back(str_post_process_quality);
	m_available_macros.push_back(str_lighting_quality);
	m_available_macros.push_back(str_shading_quality);

	// configuration flags
	m_available_macros.push_back(str_cfg_use_diffuse);
	m_available_macros.push_back(str_cfg_use_anisotropic_direction);
	m_available_macros.push_back(str_cfg_use_normal);
	m_available_macros.push_back(str_cfg_use_detail_normal);
	m_available_macros.push_back(str_cfg_use_alpha_test);
	m_available_macros.push_back(str_cfg_use_parallax);
	m_available_macros.push_back(str_cfg_use_trancluency_texture);
	m_available_macros.push_back(str_cfg_use_specular_color_texture);
	m_available_macros.push_back(str_cfg_use_fresnel_texture);
	m_available_macros.push_back(str_cfg_use_roughness_texture);
	m_available_macros.push_back(str_cfg_use_emissive);
	m_available_macros.push_back(str_cfg_use_diffuse_power_texture);
	m_available_macros.push_back(str_cfg_use_detail);
	m_available_macros.push_back(str_cfg_use_alphablended_diffuse);
	m_available_macros.push_back(str_cfg_use_alphablended_normal);
	m_available_macros.push_back(str_cfg_use_vertex_blended_diffuse);
	m_available_macros.push_back(str_cfg_use_vertex_blended_mask);
	m_available_macros.push_back(str_cfg_use_vertex_blended_normal);
	m_available_macros.push_back(str_cfg_use_vertex_blended_specular);
	m_available_macros.push_back(str_cfg_use_detail_bump);
	m_available_macros.push_back(str_cfg_use_subuv);
	m_available_macros.push_back(str_cfg_use_reflection);
	m_available_macros.push_back(str_cfg_use_ttransparency);
	m_available_macros.push_back(str_cfg_use_grass_fresnel_effect);
	m_available_macros.push_back(str_cfg_light_type);
	m_available_macros.push_back(str_cfg_vertex_input_type);
	m_available_macros.push_back(str_cfg_post_process_blur_kernel);
	m_available_macros.push_back(str_cfg_lod_index);
	m_available_macros.push_back(str_cfg_use_specular_lighting);
	m_available_macros.push_back(str_cfg_wind_motion);
	m_available_macros.push_back(str_cfg_use_bound_normals);
	m_available_macros.push_back(str_cfg_use_up_directional_normals);
	m_available_macros.push_back(str_cfg_use_diffuse_as_specular);
	m_available_macros.push_back(str_cfg_num_used_terrain_layers);
	m_available_macros.push_back(str_cfg_used_terrain_height_mask);
	m_available_macros.push_back(str_cfg_use_variation_mask);
	m_available_macros.push_back(str_cfg_use_shadowed_light);
	m_available_macros.push_back(str_cfg_use_bokeh_dof);
	m_available_macros.push_back(str_cfg_use_bokeh_image);
	m_available_macros.push_back(str_cfg_lighting_model);
	m_available_macros.push_back(str_cfg_decal_material);
	m_available_macros.push_back(str_cfg_decal_type);
	m_available_macros.push_back(str_cfg_is_anisotropic_material);
	m_available_macros.push_back(str_cfg_use_ao_texture);
	m_available_macros.push_back(str_cfg_local_reflections);
	m_available_macros.push_back(str_cfg_use_indirect_specular);
	m_available_macros.push_back(str_cfg_use_image_grain);
	m_available_macros.push_back(str_cfg_env_probe_clip_by_normal);
	m_available_macros.push_back(str_cfg_env_probe_with_shadows);
	m_available_macros.push_back(str_cfg_env_probe_geometry_type);
	m_available_macros.push_back(str_cfg_use_diffuse_masked_color);
	m_available_macros.push_back(str_cfg_use_diffuse_masked_color_by_hue);
	m_available_macros.push_back(str_cfg_use_uv_scrolling);
	m_available_macros.push_back(str_cfg_use_normal_waves);
	m_available_macros.push_back(str_cfg_use_fuzziness);
	m_available_macros.push_back(str_cfg_terrain_blend_mode);
	m_available_macros.push_back(str_cfg_use_soft_edges);
	m_available_macros.push_back(str_cfg_use_thickness_map);
	m_available_macros.push_back(str_cfg_use_subsurface_scattering_mask_map);
	m_available_macros.push_back(str_cfg_use_olta);
	m_available_macros.push_back(str_cfg_use_organic_scattering_amount_mask);
	m_available_macros.push_back(str_cfg_use_organic_scattering_depth_texture);
	m_available_macros.push_back(str_cfg_use_organic_back_illumination_texture);
	m_available_macros.push_back(str_cfg_use_organic_subdermal_texture);

	m_available_macros.push_back(str_cfg_use_sequence);

	m_available_macros.push_back(str_cfg_reflection_mask);

	std::sort( m_available_macros.begin(), m_available_macros.end(), shader_macros_dort_predicate());
}

void shader_macros::fill_shader_macro_list(
	shader_defines_list& macros,
	shader_configuration shader_config
)
{
	macros.clear();

	fill_global_macros( macros);
	fill_shader_configuration_macros ( macros, shader_config);
}

void shader_macros::fill_shader_configuration_macros(
	shader_defines_list& macros,
	shader_configuration shader_config
)
{
	add_bool_macro  ( macros, str_global_master_gold,					true);

	add_bool_macro 	(macros, str_cfg_use_diffuse,						shader_config.use_diffuse_texture);
	add_bool_macro 	(macros, str_cfg_use_normal,						shader_config.use_normal_texture);
	add_bool_macro 	(macros, str_cfg_use_detail_normal,					shader_config.use_detail_normal_texture);
	add_bool_macro 	(macros, str_cfg_use_alpha_test,					shader_config.use_alpha_test);
	add_bool_macro 	(macros, str_cfg_use_parallax, 						shader_config.use_parallax);
	add_bool_macro 	(macros, str_cfg_use_trancluency_texture,			shader_config.use_translucency_texture);
	add_bool_macro 	(macros, str_cfg_use_specular_color_texture,		shader_config.use_specular_intensity_texture);
	add_bool_macro 	(macros, str_cfg_use_fresnel_texture,				shader_config.use_fresnel_texture);
	add_bool_macro 	(macros, str_cfg_use_roughness_texture,		shader_config.use_roughness_texture);
	add_u8_macro		(macros, str_cfg_use_emissive,						shader_config.use_emissive);
	add_bool_macro 	(macros, str_cfg_use_diffuse_power_texture,			shader_config.use_diffuse_power_texture);
	add_bool_macro 	(macros, str_cfg_use_detail,						shader_config.use_detail_texture);
	add_bool_macro 	(macros, str_cfg_use_subuv,							shader_config.use_subuv);
	add_bool_macro 	(macros, str_cfg_use_ttransparency,					shader_config.use_transparency_texture);
	add_u8_macro 	(macros, str_cfg_use_reflection,					shader_config.use_reflection);
	add_bool_macro 	(macros, str_cfg_use_grass_fresnel_effect,			shader_config.use_grass_fresnel_effect);
	add_u8_macro 	(macros, str_cfg_light_type,						shader_config.light_type);
	add_u8_macro 	(macros, str_cfg_vertex_input_type,					shader_config.vertex_input_type);
	add_bool_macro 	(macros, str_cfg_use_bokeh_dof,						shader_config.use_bokeh_dof);
	add_bool_macro 	(macros, str_cfg_use_bokeh_image,					shader_config.use_bokeh_image);
	add_bool_macro 	(macros, str_cfg_use_variation_mask,				shader_config.use_variation_mask);
	add_bool_macro 	(macros, str_cfg_use_shadowed_light,				shader_config.shadowed_light);
	add_bool_macro 	(macros, str_cfg_use_alphablended_diffuse,			shader_config.alphablended_diffuse);
	add_bool_macro 	(macros, str_cfg_use_alphablended_normal,			shader_config.alphablended_normal);
	add_bool_macro 	(macros, str_cfg_use_bound_normals,					shader_config.use_bound_normals);
	add_bool_macro 	(macros, str_cfg_use_up_directional_normals,		shader_config.use_up_directional_normals);
	add_bool_macro 	(macros, str_cfg_use_diffuse_as_specular,			shader_config.use_diffuse_as_specular);
	add_u8_macro 	(macros, str_cfg_num_used_terrain_layers,			shader_config.num_used_terrain_layers);
	add_u8_macro 	(macros, str_cfg_used_terrain_height_mask,			shader_config.used_terrain_height_mask);
	add_u8_macro 	(macros, str_cfg_lod_index,							shader_config.lod_index);
	add_bool_macro 	(macros, str_cfg_use_specular_lighting,				shader_config.use_specular_lighting);
	add_bool_macro 	(macros, str_cfg_use_vertex_blended_diffuse,		shader_config.use_vertex_blended_diffuse);
	add_bool_macro 	(macros, str_cfg_use_vertex_blended_mask,			shader_config.use_vertex_blended_mask);
	add_bool_macro 	(macros, str_cfg_use_vertex_blended_normal,			shader_config.use_vertex_blended_normal);
	add_bool_macro 	(macros, str_cfg_use_vertex_blended_specular,		shader_config.use_vertex_blended_specular);
	add_bool_macro 	(macros, str_cfg_use_indirect_specular,				shader_config.use_indirect_specular);
	add_bool_macro 	(macros, str_cfg_use_image_grain,					shader_config.use_image_grain);
	add_bool_macro 	(macros, str_cfg_env_probe_clip_by_normal,			shader_config.env_probe_clip_by_normal);
	add_bool_macro 	(macros, str_cfg_env_probe_with_shadows,			shader_config.env_probe_with_shadows);
	add_bool_macro 	(macros, str_cfg_env_probe_geometry_type,			shader_config.env_probe_geometry_type);
	add_bool_macro 	(macros, str_cfg_use_diffuse_masked_color,			shader_config.use_diffuse_masked_color);
	add_bool_macro 	(macros, str_cfg_use_diffuse_masked_color_by_hue,	shader_config.use_diffuse_masked_color_by_hue);
	add_bool_macro 	(macros, str_cfg_use_uv_scrolling,					shader_config.use_uv_scrolling);
	add_bool_macro 	(macros, str_cfg_use_normal_waves,					shader_config.use_normal_waves);
	add_bool_macro 	(macros, str_cfg_use_fuzziness,						shader_config.use_fuzziness);
	add_bool_macro 	(macros, str_cfg_terrain_blend_mode,				shader_config.terrain_blend_mode != 0);
	add_bool_macro 	(macros, str_cfg_use_soft_edges,					shader_config.use_soft_edges);
	add_bool_macro 	(macros, str_cfg_use_thickness_map,					shader_config.use_thickness_map);
	add_bool_macro 	(macros, str_cfg_use_subsurface_scattering_mask_map, shader_config.use_subsurface_scattering_mask_map);
	add_bool_macro 	(macros, str_cfg_use_olta,							shader_config.use_olta);
	add_u8_macro 	(macros, str_cfg_wind_motion,						shader_config.wind_motion);
	add_bool_macro 	(macros, str_cfg_decal_material,					shader_config.decal_material);
	add_u8_macro 	(macros, str_cfg_decal_type,						shader_config.decal_type);
	add_u8_macro 	(macros, str_cfg_post_process_blur_kernel,			shader_config.post_process_blur_kernel);
	add_bool_macro 	(macros, str_cfg_use_anisotropic_direction,			shader_config.use_anisotropic_direction_texture);
	add_bool_macro 	(macros, str_cfg_is_anisotropic_material,			shader_config.is_anisotropic_material);
	add_bool_macro 	(macros, str_cfg_use_ao_texture,					shader_config.use_ao_texture);
	add_bool_macro 	(macros, str_cfg_use_organic_scattering_amount_mask,	shader_config.use_organic_scattering_amount_mask);
	add_bool_macro 	(macros, str_cfg_use_organic_scattering_depth_texture,	shader_config.use_organic_scattering_depth_texture);
	add_bool_macro 	(macros, str_cfg_use_organic_back_illumination_texture,	shader_config.use_organic_back_illumination_texture);
	add_bool_macro 	(macros, str_cfg_use_organic_subdermal_texture,			shader_config.use_organic_subdermal_texture);

	add_bool_macro 	(macros, str_cfg_use_sequence,						shader_config.use_sequence);

	add_bool_macro 	(macros, str_cfg_reflection_mask,					shader_config.use_reflection_mask);
	add_bool_macro 	(macros, str_cfg_local_reflections,					shader_config.use_local_reflections);
}

void shader_macros::fill_global_macros( shader_defines_list& defines )
{
	vostok::render::options::ref().fill_global_macros(defines);
}

void shader_macros::merge_with_declared_macroses(
	shader_declarated_macroses_list const& declared_macroses,
	shader_defines_list& macros
)
{
	for (shader_defines_list::iterator define_it = macros.begin(); define_it!=macros.end(); ++define_it)
	{
		if (!define_it->name.length())
			continue;

		bool found = false;
		for (shader_declarated_macroses_list::const_iterator decl_it = declared_macroses.begin(); decl_it!=declared_macroses.end(); ++decl_it)
		{
			if (strings::compare(define_it->name.c_str(), (*decl_it).c_str())==0)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			//LOG_ERROR("definition '%s' doesn't exist in declared defines, but used in shader.", define_it->Name);
			//macros.erase(define_it);
		}
	}
}

} // namespace render
} // namespace vostok
