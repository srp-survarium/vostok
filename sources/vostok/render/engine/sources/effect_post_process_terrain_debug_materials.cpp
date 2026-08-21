#include "pch.h"
#include "effect_post_process_terrain_debug_materials.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_post_process_terrain_debug_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{

	compiler.begin_technique();
		compiler.begin_pass("copy_image", NULL, "terrain_debug", shader_configuration(), NULL);
			compiler.set_depth( false, false, D3D_COMPARISON_LESS_EQUAL);
			compiler.set_stencil( true, 0x80,0x80,0x00, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP,D3D_STENCIL_OP_KEEP,D3D_STENCIL_OP_KEEP);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_fill_mode(D3D_FILL_SOLID);
			compiler.set_texture("t_position", r2_rt_p, 0, false, u32(-1));
			compiler.set_texture("t_normal", r2_rt_n, 0, false, u32(-1));
			compiler.set_texture("t_decals_diffuse", r2_rt_decals_diffuse, 0, false, u32(-1));
			compiler.set_texture("t_decals_normal", r2_rt_decals_normal, 0, false, u32(-1));
			compiler.set_texture("t_gradient", pcstr(custom_config["texture_gradient"]), 0, false, u32(-1));
			compiler.set_constant("deepening_color_and_range", float4(float4(custom_config["constant_deepening_color"]).xyz(), float(custom_config["constant_deepening_range"])));
			compiler.set_constant("deepening_parameters", float4(
				float(custom_config["constant_deepening_clip_dist"]),
				float(custom_config["constant_deepening_scale"]),
				float(custom_config["constant_deepening_power"]),
				0));
			compiler.set_alpha_blend(false);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
