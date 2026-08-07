#include "pch.h"
#include "effect_motion_vectors_accumulation.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_motion_vectors_accumulation::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	shader_configuration configuration;

	configuration.use_alpha_test = config.value_exists("use_alpha_test") ? bool(config["use_alpha_test"]) : false;
	configuration.use_diffuse_texture = config.value_exists("use_tdiffuse") ? bool(config["use_tdiffuse"]) : false;

	compile_begin("vertex_base", "motion_vectors_accumulation", compiler, &configuration, config);
		compiler.set_depth(true, false);

		float alpha_ref = 0.25f;

		if (configuration.use_diffuse_texture)
			compiler.set_texture("t_base", pcstr(config["texture_diffuse"]), 0, false, u32(-1));

		if (configuration.use_alpha_test && config.value_exists("alpha_ref"))
			alpha_ref = float(config["alpha_ref"]);

		compiler.set_constant("alpha_ref_parameter", alpha_ref);

		compiler.set_cull_mode(D3D_CULL_NONE);
	compile_end(compiler);
}

} // namespace render
} // namespace vostok
