#include "pch.h"
#include "sky_default_material_effect.h"

#include <vostok/render/core/custom_config_value.h>
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void sky_default_material_effect::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	// FUNCTION BODY[0x7b8dd0]
	shader_configuration configuration;
	compile_begin("vertex_base", "forward_empty", compiler, &configuration, config);
	compile_end(compiler);

	compiler.begin_technique();
		compiler.begin_pass("sky2", NULL, "sky2", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_stencil(true, 0x00, 0xff, 0x00, D3D_COMPARISON_EQUAL);
			compiler.set_alpha_blend(true);
			compiler.set_texture("t_skybox", pcstr(config["sky_texture"]), 0, true, 0);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
