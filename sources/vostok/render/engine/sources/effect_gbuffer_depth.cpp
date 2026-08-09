#include "pch.h"
#include "effect_gbuffer_depth.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_gbuffer_depth::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	shader_configuration configuration;




	compile_begin("vertex_base", "geometry_depth_pass", compiler, &configuration, config);
		compiler.set_depth(true, false);
		compiler.set_cull_mode(D3D_CULL_NONE);
		compiler.color_write_enable(D3D11_COLOR_WRITE_ENABLE_RED);
	compile_end(compiler);
}

} // namespace render
} // namespace vostok
