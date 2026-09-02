// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_fstage_default_view_angle_dependent_materials.h"
#include <vostok/render/core/custom_config_value.h>
#include <vostok/render/core/dx11/effect_compiler.h>
namespace vostok {
namespace render {

void effect_fstage_default_view_angle_dependent_materials::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	shader_configuration configuration;
	configuration.use_emissive = config.value_exists("use_temissive") ? bool(config["use_temissive"]) + 1 : 0;
	configuration.use_transparency_texture = config.value_exists("use_ttransparency") ? bool(config["use_ttransparency"]) : false;
	configuration.use_soft_edges = config.value_exists("use_soft_edges") ? bool(config["use_soft_edges"]) : false;

	compile_begin("vertex_base", "forward_view_angle_dependent", compiler, &configuration, config);
		float4 solid_color_specular(0.f, 0.f, 0.f, 0.f);
		float solid_transparency = 1.f;

		if (configuration.use_emissive == 2)
			compiler.set_texture("t_base", pcstr(config["texture_emissive"]), 0, false, u32(-1));

		if (config.value_exists("constant_emissive") && config.value_exists("constant_emissive_multiplier"))
		{
			solid_color_specular = float4(
				float(config["constant_emissive_multiplier"]) * float4(config["constant_emissive"]).xyz(),
				1.f
			);
		}

		if (config.value_exists("attenuation_power"))
			compiler.set_constant("attenuation_power", float(config["attenuation_power"]));

		if (configuration.use_transparency_texture)
			compiler.set_texture("t_transparency", pcstr(config["texture_transparency"]), 0, false, u32(-1));

		if (config.value_exists("constant_transparency"))
			solid_transparency = config.value_exists("constant_transparency") ? float(config["constant_transparency"]) : 1.f;

		if (config.value_exists("solid_transparency"))
			compiler.set_constant("solid_transparency", solid_transparency);

		if (config.value_exists("solid_color_specular"))
			compiler.set_constant("solid_color_specular", solid_color_specular);

		if (config.value_exists("constant_tile_u") && config.value_exists("constant_tile_v"))
			compiler.set_constant("constant_tile_uv", float2(float(config["constant_tile_u"]), float(config["constant_tile_v"])));
		else
			compiler.set_constant("constant_tile_uv", float2(1.f, 1.f));

		if (configuration.use_soft_edges)
			compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));

		compiler.set_cull_mode(D3D_CULL_BACK);
	compile_end(compiler);
}

} // namespace render
} // namespace vostok
