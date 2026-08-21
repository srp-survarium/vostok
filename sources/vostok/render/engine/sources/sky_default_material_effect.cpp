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
	compiler.begin_technique();
		compiler.begin_pass("vertex_base", NULL, "skybox_preview", shader_configuration(), NULL);
			compiler.set_texture("t_skybox", pcstr(config["sky_texture"]), 0, true, u32(-1));
			compiler.set_cull_mode(D3D_CULL_FRONT);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("sky2", NULL, "sky2", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_stencil(true, 0x00, 0xff, 0x00, D3D_COMPARISON_EQUAL);
			compiler.set_alpha_blend(true);
			compiler.set_texture("t_skybox", pcstr(config["sky_texture"]), 0, true, u32(-1));
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
