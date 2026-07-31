#include "pch.h"
#include "depth_accumulate_material_effect.h"

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
	shader_configuration				configuration;
	configuration.use_alpha_test		= config.value_exists("use_alpha_test") ? bool(config["use_alpha_test"]) : false;

	if (config.value_exists("wind_motion"))
		configuration.wind_motion = s32(config["wind_motion"]);

	compile_begin("vertex_base", "depth_accumulate", compiler, &configuration, config);

		if (configuration.use_alpha_test)
			compiler.set_texture("t_base", pcstr(config["texture_diffuse"]), 0, true, 0);

		compiler.set_depth			(true, true);
		compiler.color_write_enable (D3D_COLOR_WRITE_ENABLE_NONE);
		compiler.set_cull_mode		(D3D_CULL_NONE);

	compile_end(compiler);
}

} // namespace render
} // namespace vostok
