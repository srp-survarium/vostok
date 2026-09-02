// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_fstage_sky_materials.h"
#include <vostok/render/core/custom_config_value.h>
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/facade/vertex_input_type.h>
namespace vostok {
namespace render {

void effect_fstage_sky_materials::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	shader_configuration configuration;
	configuration.vertex_input_type = static_mesh_vertex_input_type;
	compile_begin("forward_sky", "forward_sky", compiler, &configuration, config);
		float4 sky_color = float4(config["sky_color"]);
		sky_color.xyz() *= float(config["sky_color_multiplier"]);
		compiler.set_constant("sky_color", sky_color);
		compiler.set_texture("t_base", pcstr(config["texture_emissive"]), 0, false, u32(-1));
		compiler.set_depth(false, false);
		compiler.set_alpha_blend(true, D3D_BLEND_ONE, D3D_BLEND_ONE);
		compiler.set_stencil(true, 0, 0xff, 0, D3D_COMPARISON_EQUAL);
		compiler.set_cull_mode(D3D_CULL_NONE);

		if (config.value_exists("sky_fog_power") && config.value_exists("sky_fog_up_limit"))
		{
			float const fog_power = float(config["sky_fog_power"]);
			compiler.set_constant(
				"fog_power_and_range",
				float4(fog_power, float(config["sky_fog_up_limit"]), 0.f, 0.f)
			);
		} else
			compiler.set_constant("fog_power_and_range", float4(1.f, 0.25f, 0.f, 0.f));

	compile_end(compiler);
}

} // namespace render
} // namespace vostok
