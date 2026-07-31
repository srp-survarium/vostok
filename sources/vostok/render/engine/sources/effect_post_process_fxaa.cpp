#include "pch.h"
#include "effect_post_process_fxaa.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_post_process_fxaa::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	// FUNCTION BODY[0x7bc080]
	VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

	compiler.begin_technique();
		compiler.begin_pass("post_process_fxaa", NULL, "post_process_fxaa", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_texture("t_frame_color", r2_rt_generic1, 0, false, 0);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
