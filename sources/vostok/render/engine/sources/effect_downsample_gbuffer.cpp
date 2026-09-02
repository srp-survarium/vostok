// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_downsample_gbuffer.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
namespace vostok {
namespace render {

void effect_downsample_gbuffer::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{

	VOSTOK_UNREFERENCED_PARAMETER(config);
	compiler.begin_technique();
		compiler.begin_pass("lpv_downsample_reflective_shadow_map", NULL, "lpv_downsample_gbuffer", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_texture("t_position", r2_rt_p, 0, false, u32(-1));
			compiler.set_texture("t_normal", r2_rt_n, 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
