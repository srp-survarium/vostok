#include "pch.h"
#include "effect_editor_vertex_alpha.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_editor_vertex_alpha::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	shader_configuration configuration;

	compile_begin("vertex_base", "editor_vertex_alpha", compiler, &configuration, config);
		compiler.set_depth(true, true);
		compiler.set_cull_mode(D3D_CULL_NONE);
	compile_end(compiler);
}

} // namespace render
} // namespace vostok
