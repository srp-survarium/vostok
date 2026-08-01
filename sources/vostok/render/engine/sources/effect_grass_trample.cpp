#include "pch.h"
#include "effect_grass_trample.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_grass_trample::compile(
	effect_compiler& compiler,
	custom_config_value const&
)
{
	compiler.begin_technique();
	compiler.begin_pass("grass_trample", NULL, "grass_trample", shader_configuration(), NULL);
	compiler.set_depth(false, false);
	compiler.set_cull_mode(D3D11_CULL_NONE);
	compiler.set_texture("t_trample_template", "engine/grass_trample_template", 0, false, 0);
	compiler.set_alpha_blend(true, D3D_BLEND_ONE, D3D_BLEND_ONE);
	compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
