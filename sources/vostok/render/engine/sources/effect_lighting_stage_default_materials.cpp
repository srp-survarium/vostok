#include "pch.h"
#include "effect_lighting_stage_default_materials.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/facade/light_props.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

bool effect_lighting_stage_default_materials::should_recompile_when_global_changes(
	vector< fs_new::virtual_path_string > const& global_names
) const
{
	return std::find(global_names.begin(), global_names.end(), "GLOBAL_SHADING_QUALITY") != global_names.end() ||
		std::find(global_names.begin(), global_names.end(), "GLOBAL_USE_POISSON_DISC_SHADOW_FILTER") != global_names.end();
}

void effect_lighting_stage_default_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	shader_configuration configuration;
	// 2 target lines are likely retail-compiled-out source.
	configuration.use_diffuse_texture = bool(custom_config["use_tdiffuse"]);
	configuration.use_normal_texture = bool(custom_config["use_nmap"]);
	configuration.use_alpha_test = bool(custom_config["use_alpha_test"]);
	configuration.use_transparency_texture = bool(custom_config["use_ttransparency"]);
	// 2 target lines are likely retail-compiled-out source.
	configuration.use_specular_intensity_texture = custom_config.value_exists("use_tspecular_inensity") ? bool(custom_config["use_tspecular_inensity"]) : false;
	configuration.use_fresnel_texture = custom_config.value_exists("use_tfresnel") ? bool(custom_config["use_tfresnel"]) : false;
	configuration.use_roughness_texture = custom_config.value_exists("use_troughness") ? bool(custom_config["use_troughness"]) : false;
	// 2 target lines are likely retail-compiled-out source.
	configuration.is_anisotropic_material = custom_config.value_exists("is_anisotropic_material") ? bool(custom_config["is_anisotropic_material"]) : false;

	configuration.use_soft_edges = custom_config.value_exists("use_soft_edges") ? bool(custom_config["use_soft_edges"]) : false;
	// 2 target lines are likely retail-compiled-out source.
	{
		configuration.light_type = 0;

		compile_begin("vertex_base", "forward_lighting", compiler, &configuration, custom_config);
		// 6 target lines are likely retail-compiled-out source.
		if (configuration.use_diffuse_texture)
			compiler.set_texture("t_base", pcstr(custom_config["texture_diffuse"]), 0, false, u32(-1));
		// 2 target lines are likely retail-compiled-out source.
		float4 solid_color_specular = float4(custom_config["constant_diffuse"]);
		solid_color_specular.w = 0.0f;
		// 5 target lines are likely retail-compiled-out source.
		if (configuration.vertex_input_type == particle_vertex_input_type || configuration.vertex_input_type == particle_subuv_vertex_input_type || configuration.vertex_input_type == particle_beamtrail_vertex_input_type)
			compiler.set_depth(false, false);

		compiler.set_texture("t_jitter_lookup", "$user$jitter_lookup", 0, false, u32(-1));

		compiler.set_alpha_blend(true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_ONE);

		if (configuration.use_normal_texture)
			compiler.set_texture("t_normal", pcstr(custom_config["texture_normal"]), 0, false, u32(-1));

		float4 specular_intensity_ranges(0.0f, 1.0f, 0.0f, 0.0f);

		if (configuration.use_specular_intensity_texture)
		{
			compiler.set_texture("t_specular_intensity", pcstr(custom_config["texture_specular_intensity"]), 0, false, u32(-1));

			if (custom_config.value_exists("constant_specular_intensity_min"))
			{
				specular_intensity_ranges.x = float(custom_config["constant_specular_intensity_min"]);
				specular_intensity_ranges.y = float(custom_config["constant_specular_intensity_max"]) - specular_intensity_ranges.x;
			}
		}

		if (configuration.use_soft_edges)
			compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));

		compiler.set_texture("t_jitter_lookup", "$user$jitter_lookup", 0, false, u32(-1));

		compiler.set_constant("specular_intensity_ranges", specular_intensity_ranges);

		if (custom_config.value_exists("constant_specular_color") && custom_config.value_exists("constant_specular_color_multiplier"))
			compiler.set_constant("specular_color_parameter", float4(custom_config["constant_specular_color"]).xyz() * float3(custom_config["constant_specular_color_multiplier"]));
		// 2 target lines are likely retail-compiled-out source.
		compiler.set_constant("solid_color_specular", solid_color_specular);

		float4 solid_material_params(0.0f, 0.0f, 0.0f, 0.0f);
		// 4 target lines are likely retail-compiled-out source.
		float4 specular_fresnel_roughness_parameters(0.0f, 0.0f, 0.0f, 0.0f);

		if (configuration.use_roughness_texture)
		{
			compiler.set_texture("t_roughness", pcstr(custom_config["texture_roughness"]), 0, false, u32(-1));
			if (custom_config.value_exists("constant_roughness_min"))
			{
				specular_fresnel_roughness_parameters.z = float(custom_config["constant_roughness_min"]);
				specular_fresnel_roughness_parameters.w = float(custom_config["constant_roughness_max"]) - specular_fresnel_roughness_parameters.z;
			}

		}

		else if (custom_config.value_exists("constant_roughness"))
			specular_fresnel_roughness_parameters.z = float(custom_config["constant_roughness"]);

		if (configuration.use_fresnel_texture)
		{
			compiler.set_texture("t_fresnel", pcstr(custom_config["texture_fresnel"]), 0, false, u32(-1));
			if (custom_config.value_exists("constant_fresnel_min"))
			{
				specular_fresnel_roughness_parameters.x = float(custom_config["constant_fresnel_min"]);
				specular_fresnel_roughness_parameters.y = float(custom_config["constant_fresnel_max"]) - specular_fresnel_roughness_parameters.x;
			}

		}

		else if (custom_config.value_exists("constant_fresnel"))
			specular_fresnel_roughness_parameters.x = float(custom_config["constant_fresnel"]);

		compiler.set_constant("specular_fresnel_roughness_parameters", specular_fresnel_roughness_parameters);

		float solid_transparency = 1.0f;
		// 5 target lines are likely retail-compiled-out source.
		if (configuration.use_transparency_texture)
			compiler.set_texture("t_transparency", pcstr(custom_config["texture_transparency"]), 0, false, u32(-1));

		solid_transparency = custom_config.value_exists("constant_transparency") ? float(custom_config["constant_transparency"]) : 1.0f;

		compiler.set_constant("solid_transparency", solid_transparency);
		// 2 target lines are likely retail-compiled-out source.
		compiler.set_constant("solid_material_params", solid_material_params);

		compiler.set_texture("t_cascaded_shadow_map", "$user$cascaded_shadow_map", 0, false, u32(-1));
		// 2 target lines are likely retail-compiled-out source.
		compile_end(compiler);
	}
	// 3 target lines are likely retail-compiled-out source.
	{
		shader_configuration probe_configuration;
		probe_configuration.use_transparency_texture = bool(custom_config["use_ttransparency"]);
		probe_configuration.use_diffuse_texture = bool(custom_config["use_tdiffuse"]);
		probe_configuration.use_normal_texture = bool(custom_config["use_nmap"]);
		probe_configuration.use_alpha_test = bool(custom_config["use_alpha_test"]);
		probe_configuration.use_fresnel_texture = custom_config.value_exists("use_tfresnel") ? bool(custom_config["use_tfresnel"]) : false;
		probe_configuration.use_roughness_texture = custom_config.value_exists("use_troughness") ? bool(custom_config["use_troughness"]) : false;
		probe_configuration.use_soft_edges = custom_config.value_exists("use_soft_edges") ? bool(custom_config["use_soft_edges"]) : false;

		compile_begin("vertex_base", "forward_probe_lighting", compiler, &probe_configuration, custom_config);
		// 5 target lines are likely retail-compiled-out source.
		compiler.set_alpha_blend(true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_ONE);
		// 3 target lines are likely retail-compiled-out source.
		if (configuration.vertex_input_type == particle_vertex_input_type || configuration.vertex_input_type == particle_subuv_vertex_input_type || configuration.vertex_input_type == particle_beamtrail_vertex_input_type)
			compiler.set_depth(false, false);
		// 4 target lines are likely retail-compiled-out source.
		if (probe_configuration.use_diffuse_texture)
			compiler.set_texture("t_base", pcstr(custom_config["texture_diffuse"]), 0, false, u32(-1));
		// 2 target lines are likely retail-compiled-out source.
		float4 solid_color_specular = float4(custom_config["constant_diffuse"]);
		solid_color_specular.w = 0.0f;
		// 4 target lines are likely retail-compiled-out source.
		compiler.set_constant("solid_color_specular", solid_color_specular);

		if (probe_configuration.use_normal_texture)
			compiler.set_texture("t_normal", pcstr(custom_config["texture_normal"]), 0, false, u32(-1));
		// 3 target lines are likely retail-compiled-out source.
		if (probe_configuration.use_transparency_texture)
			compiler.set_texture("t_transparency", pcstr(custom_config["texture_transparency"]), 0, false, u32(-1));

		float solid_transparency = custom_config.value_exists("constant_transparency") ? float(custom_config["constant_transparency"]) : 1.0f;

		compiler.set_constant("solid_transparency", solid_transparency);

		if (probe_configuration.use_soft_edges)
			compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));

		float4 specular_fresnel_roughness_parameters(0.0f, 0.0f, 0.0f, 0.0f);

		if (probe_configuration.use_roughness_texture)
		{
			compiler.set_texture("t_roughness", pcstr(custom_config["texture_roughness"]), 0, false, u32(-1));
			if (custom_config.value_exists("constant_roughness_min"))
			{
				specular_fresnel_roughness_parameters.z = float(custom_config["constant_roughness_min"]);
				specular_fresnel_roughness_parameters.w = float(custom_config["constant_roughness_max"]) - specular_fresnel_roughness_parameters.z;
			}

		}

		else if (custom_config.value_exists("constant_roughness"))
			specular_fresnel_roughness_parameters.z = float(custom_config["constant_roughness"]);

		if (probe_configuration.use_fresnel_texture)
		{
			compiler.set_texture("t_fresnel", pcstr(custom_config["texture_fresnel"]), 0, false, u32(-1));
			if (custom_config.value_exists("constant_fresnel_min"))
			{
				specular_fresnel_roughness_parameters.x = float(custom_config["constant_fresnel_min"]);
				specular_fresnel_roughness_parameters.y = float(custom_config["constant_fresnel_max"]) - specular_fresnel_roughness_parameters.x;
			}

		}

		else if (custom_config.value_exists("constant_fresnel"))
			specular_fresnel_roughness_parameters.x = float(custom_config["constant_fresnel"]);

		compiler.set_constant("specular_fresnel_roughness_parameters", specular_fresnel_roughness_parameters);

		float4 specular_intensity_ranges(0.0f, 1.0f, 0.0f, 0.0f);

		if (configuration.use_specular_intensity_texture)
		{
			compiler.set_texture("t_specular_intensity", pcstr(custom_config["texture_specular_intensity"]), 0, false, u32(-1));

			if (custom_config.value_exists("constant_specular_intensity_min"))
			{
				specular_intensity_ranges.x = float(custom_config["constant_specular_intensity_min"]);
				specular_intensity_ranges.y = float(custom_config["constant_specular_intensity_max"]) - specular_intensity_ranges.x;
			}
		}

		compiler.set_constant("specular_intensity_ranges", specular_intensity_ranges);

		if (custom_config.value_exists("constant_specular_color") && custom_config.value_exists("constant_specular_color_multiplier"))
			compiler.set_constant("specular_color_parameter", float4(custom_config["constant_specular_color"]).xyz() * float3(custom_config["constant_specular_color_multiplier"]));
		// 2 target lines are likely retail-compiled-out source.
		compiler.set_constant("solid_color_specular", solid_color_specular);

		compile_end(compiler);
	}
}

} // namespace render
} // namespace vostok
