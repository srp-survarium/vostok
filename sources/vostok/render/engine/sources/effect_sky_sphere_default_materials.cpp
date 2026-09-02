// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_sky_sphere_default_materials.h"
#include <vostok/render/core/custom_config_value.h>
#include <vostok/render/core/dx11/effect_compiler.h>
namespace vostok {
namespace render {

void effect_sky_sphere_default_materials::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	compiler.begin_technique();
		compiler.begin_pass("vertex_base", NULL, "sky_sphere_preview", shader_configuration(), NULL);
			compiler.set_texture("t_sky_sphere", pcstr(config["sky_texture"]), 0, false, u32(-1));
			compiler.set_cull_mode(D3D_CULL_FRONT);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("sky_sphere", NULL, "sky_sphere", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_stencil(true, 0, 0xff, 0, D3D_COMPARISON_EQUAL);
			compiler.set_alpha_blend(false);
			compiler.set_cull_mode(D3D_CULL_NONE);

			compiler.set_texture("t_sky_sphere", pcstr(config["sky_texture"]), 0, false, u32(-1));

			if (config.value_exists("sky_rotation"))
			{
				float const angle_in_rad = math::deg2rad(float(config["sky_rotation"]));
				compiler.set_constant("sky_cos_sin", float4(math::cos(angle_in_rad), math::sin(angle_in_rad), 0.f, 0.f));
			}
			else
				compiler.set_constant("sky_cos_sin", float4(1.f, 0.f, 0.f, 0.f));

			if (config.value_exists("sky_fog_power") && config.value_exists("sky_fog_up_limit"))
			{
				float const fog_power = float(config["sky_fog_power"]);
				compiler.set_constant(
					"fog_power_and_range",
					float4(fog_power, float(config["sky_fog_up_limit"]), 0.f, 0.f)
				);
			} else
				compiler.set_constant("fog_power_and_range", float4(1.f, 0.25f, 0.f, 0.f));

		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
