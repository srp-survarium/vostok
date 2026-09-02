// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_fill_environment_probe_face.h"
#include <vostok/render/core/dx11/effect_compiler.h>
namespace vostok {
namespace render {

void effect_fill_environment_probe_face::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{

	VOSTOK_UNREFERENCED_PARAMETER(config);
	compiler.begin_technique();
		compiler.begin_pass("fill_environment_probe_face", NULL, "fill_environment_probe_face", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_fill_mode(D3D_FILL_SOLID);
			compiler.set_alpha_blend(false);
			compiler.set_texture("t_probe_face_texture", "$user$present", 0, false, u32(-1));
			compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));
			compiler.set_texture("t_normal", "$user$normal", 0, false, u32(-1));
			compiler.set_texture("t_diffuse", "$user$albedo", 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
