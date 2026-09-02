// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_fstage_simpe_water_materials.h"
#include <vostok/render/core/custom_config_value.h>
#include <vostok/render/core/dx11/effect_compiler.h>
namespace vostok {
namespace render {

void effect_fstage_simpe_water_materials::compile(
	effect_compiler& compiler,
	custom_config_value const& custom_config
)
{
	shader_configuration configuration;
	configuration.use_normal_texture = true;

	for (u32 pass_index = 0; pass_index < 2; ++pass_index)
	{
		compile_begin(
			"vertex_base",
			pass_index == 0 ? "forward_simple_water" : "forward_simple_water_local_reflections",
			compiler,
			&configuration,
			custom_config
		);
			compiler.set_texture("t_reflection", pcstr(custom_config["texture_reflection"]), 0, false, u32(-1));
			compiler.set_texture("t_normal_map", "engine/water_waves2", 0, false, u32(-1));
			compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));
			compiler.set_texture("t_normal", "$user$normal", 0, false, u32(-1));
			compiler.set_texture("t_diffuse", "$user$albedo", 0, false, u32(-1));
			compiler.set_texture("t_frame_color", "$user$generic1", 0, false, u32(-1));

			float4 packed_water_parameters_0(0.5f, 1.f, 0.5f, 1.f);
			if (custom_config.value_exists("water_fog_color"))
			{
				packed_water_parameters_0 = float4(
					math::pow(float4(custom_config["water_fog_color"]).xyz(), 2.2f),
					float(custom_config["wave_fog_density"])
				);
			}
			compiler.set_constant("packed_water_parameters_0", packed_water_parameters_0);
			compiler.set_texture("t_rain_shadow_map", "$user$rain_shadow_map", 0, false, u32(-1));
			compiler.set_texture("t_puddle_rings", "engine/rain_puddle_rings", 0, false, u32(-1));

			if (pass_index == 0)
			{
				compiler.set_texture("t_local_reflections_result", "$user$local_reflection_result", 0, false, u32(-1));
				compiler.set_texture("t_local_reflections_result_params", "$user$local_reflection_result_params", 0, false, u32(-1));
				compiler.set_texture("t_water_waves", "engine/water_waves", 0, false, u32(-1));
				compiler.set_depth(true, false);
				compiler.set_alpha_blend(false, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA);
			}
			else
			{
				compiler.set_depth(false, false);
				compiler.set_alpha_blend(false);
			}
			compiler.set_cull_mode(D3D_CULL_BACK);
		compile_end(compiler);
	}
}

} // namespace render
} // namespace vostok
