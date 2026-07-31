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
	// FUNCTION BODY[0x7bd8b0]
	VOSTOK_UNREFERENCED_PARAMETER(config);

	compiler.begin_technique();
		compiler.begin_pass("apply_indirect_lighting", NULL, "apply_indirect_lighting", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_texture("t_normal", r2_rt_n, 0, false, 0);
			compiler.set_texture("t_position", r2_rt_p, 0, false, 0);
			compiler.set_texture("t_ssao_accumulator",	r2_rt_ssao_accumulator, 0, false, 0);
			compiler.set_alpha_blend(true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("apply_indirect_lighting", NULL, "apply_indirect_lighting_add", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_alpha_blend(true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD);
			compiler.set_texture("t_indirect",	r2_rt_indirect_lighting, 0, false, 0);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
