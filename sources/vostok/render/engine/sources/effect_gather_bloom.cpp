#include "pch.h"
#include "effect_gather_bloom.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_gather_bloom::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	// FUNCTION BODY[0x7bc890]
	VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

	compiler.begin_technique();
		compiler.begin_pass("gather_bloom", NULL, "gather_bloom", shader_configuration(), NULL);
			//compiler.set_texture("t_frame_color", r2_rt_generic0);
			compiler.set_texture("t_position", r2_rt_p, 0, false, 0);
			compiler.set_texture("t_frame_luminance", r2_rt_frame_luminance, 0, false, 0);
			compiler.set_depth( true, false);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
