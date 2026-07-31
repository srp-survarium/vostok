#include "pch.h"
#include "effect_ssao_accumulation.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_ssao_accumulation::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	// FUNCTION BODY[0x7be840]
	VOSTOK_UNREFERENCED_PARAMETER(custom_config);

 	compiler.begin_technique();
 		compiler.begin_pass("post_process0", NULL, "ssao_accumulation", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_alpha_blend(false);
 			compiler.set_texture("t_random_rotates", "engine/ssao_rotate", 0, true, 0);
			compiler.set_texture("t_position", r2_rt_p, 0, false, 0);
			compiler.set_texture("t_normal", r2_rt_n, 0, false, 0);
			compiler.set_texture("t_decals_normal", r2_rt_decals_normal, 0, false, 0);
 		compiler.end_pass();
 	compiler.end_technique();
}

} // namespace render
} // namespace vostok
