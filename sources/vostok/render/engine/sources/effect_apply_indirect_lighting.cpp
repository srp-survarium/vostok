// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_apply_indirect_lighting.h"
#include "geometry_type.h"
#include "shared_names.h"
#include <vostok/render/core/dx11/effect_compiler.h>
namespace vostok {
namespace render {

void effect_apply_indirect_lighting::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	VOSTOK_UNREFERENCED_PARAMETER(config);
	compiler.begin_technique();
		compiler.begin_pass("apply_indirect_lighting", NULL, "apply_indirect_lighting_diffuse", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_texture("t_normal", r2_rt_n, 0, false, u32(-1));
			compiler.set_texture("t_position", r2_rt_p, 0, false, u32(-1));
			compiler.set_texture("t_ssao_accumulator", "$user$ssao_accumulator_full_x", 0, false, u32(-1));
			compiler.set_alpha_blend(true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD);
			compiler.set_stencil(true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("apply_indirect_lighting", NULL, "apply_indirect_lighting_specular", shader_configuration(), NULL);
			compiler.set_texture("t_normal", r2_rt_n, 0, false, u32(-1));
			compiler.set_texture("t_position", r2_rt_p, 0, false, u32(-1));
			compiler.set_stencil(true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
