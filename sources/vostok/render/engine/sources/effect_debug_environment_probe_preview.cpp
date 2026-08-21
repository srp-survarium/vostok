#include "pch.h"
#include "effect_debug_environment_probe_preview.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_debug_environment_probe_preview::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{

	VOSTOK_UNREFERENCED_PARAMETER(config);
	compiler.begin_technique();
		compiler.begin_pass("environment_probe_preview", NULL, "environment_probe_preview", shader_configuration(), NULL);
			compiler.set_depth(true, true);
			compiler.set_cull_mode(D3D_CULL_FRONT);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
