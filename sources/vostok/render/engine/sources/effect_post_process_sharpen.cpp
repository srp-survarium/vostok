#include "pch.h"
#include "effect_post_process_sharpen.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_post_process_sharpen::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	// FUNCTION BODY[0x7bbc70]
	VOSTOK_UNREFERENCED_PARAMETER(custom_config);

	compiler.begin_technique();
		compiler.begin_pass("post_process_sharpen", NULL, "post_process_pre_sharpen", shader_configuration(), NULL);
			compiler.set_depth(false, false);

			compiler.set_texture("t_frame_color", r2_rt_generic0, 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();


	compiler.begin_technique();
		compiler.begin_pass("post_process_sharpen", NULL, "post_process_post_sharpen", shader_configuration(), NULL);
			compiler.set_depth(false, false);

			compiler.set_texture("t_frame_color", r2_rt_generic1, 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
