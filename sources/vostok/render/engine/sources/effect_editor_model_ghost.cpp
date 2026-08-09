#include "pch.h"
#include "effect_editor_model_ghost.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_editor_model_ghost::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	shader_configuration configuration;
	compile_begin("vertex_base", "ghost", compiler, &configuration, config);
		compiler.set_depth(true, false);
		compiler.set_alpha_blend(true,D3D_BLEND_ONE,D3D_BLEND_ONE);
		compiler.set_cull_mode(D3D_CULL_NONE);
	compile_end(compiler);
}

} // namespace render
} // namespace vostok
