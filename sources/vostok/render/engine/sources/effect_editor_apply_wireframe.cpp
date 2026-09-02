// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_editor_apply_wireframe.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
namespace vostok {
namespace render {

void effect_editor_apply_wireframe::compile(
	effect_compiler&			compiler,
	custom_config_value const&
)
{
	compiler.begin_technique();
		compiler.begin_pass("eye_adaptation", NULL, "editor_apply_wireframe", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_alpha_blend(false);
			compiler.set_texture("t_wireframe_buffer", r2_rt_generic0, 0, false, u32(-1));
			compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
