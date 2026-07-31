#include "pch.h"
#include "effect_fill_reflective_shadow_map.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_fill_reflective_shadow_map::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	// FUNCTION BODY[0x7b58a0]
	shader_configuration configuration;
	compile_begin("vertex_base", "fill_reflective_shadow_map", compiler, &configuration, config);
	compile_end(compiler);
}

} // namespace render
} // namespace vostok
