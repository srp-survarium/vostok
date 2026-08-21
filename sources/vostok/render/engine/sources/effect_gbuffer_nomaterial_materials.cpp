#include "pch.h"
#include "effect_gbuffer_nomaterial_materials.h"
#include "geometry_type.h"
#include "shared_names.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_gbuffer_nomaterial_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	for (u32 lod_index = 0; lod_index < 2; lod_index++)
	{
		shader_configuration configuration;
		configuration.lod_index = lod_index;

		compile_begin("vertex_base", "gbuffer_nomaterial_pass", compiler, &configuration, config);

		compiler.set_stencil(
			true,
			all_geometry_type + static_geometry_type,
			0xff, 0xff,
			D3D_COMPARISON_ALWAYS,
			D3D_STENCIL_OP_KEEP,
			D3D_STENCIL_OP_REPLACE,
			D3D_STENCIL_OP_KEEP
		);

		compiler.set_depth(true, true);

		compiler.set_texture("t_default_texture", "no_texture", 0, true, u32(-1));
		compile_end(compiler);
	}

	// fill rsm backend
	{
		shader_configuration local_configuration;
		compile_begin("vertex_base", "fill_reflective_shadow_map_backend", compiler, &local_configuration, config);
		compile_end(compiler);
	}

	// fill vertex color
	{
		shader_configuration local_configuration;
		compile_begin("fill_vertex_color", "fill_vertex_color", compiler, &local_configuration, config);
			compiler.set_constant("diffuse_color_parameter", float3(1.0f, 1.0f, 1.0f));
			compiler.set_texture("t_base", "no_texture", 0, true, u32(-1));
		compile_end(compiler);
	}

	// fill rsm
	{
		shader_configuration local_configuration;
		compile_begin("vertex_base", "fill_reflective_shadow_map", compiler, &local_configuration, config);
			compiler.set_constant("diffuse_color_parameter", float3(1.0f, 1.0f, 1.0f));
			compiler.set_texture("t_base", "no_texture", 0, true, u32(-1));
		compile_end(compiler);
	}

	// emissive
	{
		shader_configuration local_configuration;
		compile_begin("vertex_base", "gbuffer_emissive_pass", compiler, &local_configuration, config);
		compiler.set_depth(true, false);
		compiler.set_stencil(false);

		float4 solid_emission_color;

		if (local_configuration.use_emissive != 0)
		{
			float multiplier = float(config["constant_emissive_multiplier"]);
			solid_emission_color = float4(config["constant_emissive"]);
			solid_emission_color.xyz() *= multiplier;
			compiler.set_constant("solid_emission_color", solid_emission_color);
		}

		compiler.set_alpha_blend(true, D3D_BLEND_ONE, D3D_BLEND_ONE);

		if (local_configuration.use_emissive == 2)
		{
			compiler.set_texture("t_emission", pcstr(config["texture_emissive"]), 0, true, u32(-1));
		}

		compile_end(compiler);
	}

	// motion vectors
	{

		shader_configuration local_configuration;

		local_configuration.use_alpha_test = config.value_exists("use_alpha_test") ? bool(config["use_alpha_test"]) : false;
		local_configuration.use_diffuse_texture = config.value_exists("use_tdiffuse") ? bool(config["use_tdiffuse"]) : false;

		compile_begin("vertex_base", "motion_vectors_accumulation", compiler, &local_configuration, config);
		compiler.set_depth(true, false);
		compiler.set_cull_mode(D3D_CULL_NONE);
		compile_end(compiler);
	}

	// subsurface scattering
	{
		shader_configuration local_configuration;

		local_configuration.use_alpha_test = config.value_exists("use_alpha_test") ? bool(config["use_alpha_test"]) : false;
		local_configuration.use_diffuse_texture = config.value_exists("use_tdiffuse") ? bool(config["use_tdiffuse"]) : false;

		compile_begin("vertex_base", "subsurface_scattering", compiler, &local_configuration, config);
		compiler.set_depth(true, false);

		float alpha_ref = 0.25f;

		if (local_configuration.use_diffuse_texture)
			compiler.set_texture("t_base", pcstr(config["texture_diffuse"]), 0, false, u32(-1));

		if (local_configuration.use_alpha_test && config.value_exists("alpha_ref"))
			alpha_ref = float(config["alpha_ref"]);

		compiler.set_constant("alpha_ref_parameter", alpha_ref);

		compiler.set_texture("t_diffuse_lighting", "$user$accum_diffuse", 0, false, u32(-1));
		compiler.set_texture("t_specular_lighting", "$user$accum_specular", 0, false, u32(-1));

		compiler.set_cull_mode(D3D_CULL_NONE);
		compile_end(compiler);
	}
	// z only
	{
		shader_configuration local_configuration;

		compile_begin("vertex_base", "z_only", compiler, &local_configuration, config);
		compiler.set_depth(true, true);
		compiler.color_write_enable(D3D_COLOR_WRITE_ENABLE_NONE);
		compile_end(compiler);
	}
}

} // namespace render
} // namespace vostok
