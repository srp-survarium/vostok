#include "pch.h"
#include "effect_forward_system.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_forward_system::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	compiler.begin_technique();
		compiler.begin_pass("test", NULL, "test", shader_configuration(), NULL);
	 		compiler.set_depth(true, true);
	 		compiler.set_alpha_blend( false);
		compiler.set_texture("t_base", pcstr(config["texture0"]), 0, false, u32(-1));
		compiler.end_pass();
	 compiler.end_technique();
}

} // namespace render
} // namespace vostok
