#include "pch.h"
#include "depth_accumulate_material_effect.h"
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/render/core/custom_config_value.h>
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void depth_accumulate_material_effect::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	// FUNCTION BODY[0x7c06a0]
	bool is_static_mesh = false;
	if (config.value_exists("vertex_input_type")) {
		enum_vertex_input_type vertex_input_type = (enum_vertex_input_type)config["vertex_input_type"];
		is_static_mesh = vertex_input_type != grassmesh_vertex_input_type;
	}

	u32 const debug_last_mips = is_static_mesh ? 5 : u32(-1);


	shader_configuration configuration;
	configuration.use_alpha_test = config.value_exists("use_alpha_test") ? bool(config["use_alpha_test"]) : false;

	if (config.value_exists("wind_motion"))
		configuration.wind_motion = s32(config["wind_motion"]);

	compile_begin("vertex_base", "depth_accumulate", compiler, &configuration, config);

		if (configuration.use_alpha_test)
			compiler.set_texture("t_base", pcstr(config["texture_diffuse"]), 0, true, debug_last_mips);

		if (configuration.wind_motion && config.value_exists("wind_scale"))
			compiler.set_constant("wind_scale", float(config["wind_scale"]));
		else
			compiler.set_constant("wind_scale", 1.0f);

		float alpha_ref = 0.25f;

		if (configuration.use_alpha_test && config.value_exists("alpha_ref"))
			alpha_ref = float(config["alpha_ref"]);

		compiler.set_constant("alpha_ref_parameter", alpha_ref);

		compiler.set_depth			(true, true);
		compiler.color_write_enable (D3D_COLOR_WRITE_ENABLE_NONE);
		compiler.set_cull_mode		(D3D_CULL_NONE);

	compile_end(compiler);

	compile_begin("vertex_base_shadow_batched", "depth_accumulate_batched", compiler, &configuration, config);

		if (configuration.use_alpha_test)
			compiler.set_texture("t_base", pcstr(config["texture_diffuse"]), 0, true, debug_last_mips);

		if (configuration.use_alpha_test && config.value_exists("alpha_ref"))
			alpha_ref = float(config["alpha_ref"]);

		compiler.set_constant("alpha_ref_parameter", alpha_ref);

		if (configuration.wind_motion && config.value_exists("wind_scale"))
			compiler.set_constant("wind_scale", float(config["wind_scale"]));
		else
			compiler.set_constant("wind_scale", 1.0f);

		compiler.set_depth			(true, true);
		compiler.color_write_enable (D3D_COLOR_WRITE_ENABLE_NONE);
		compiler.set_cull_mode		(D3D_CULL_NONE);

	compile_end(compiler);
}

} // namespace render
} // namespace vostok
