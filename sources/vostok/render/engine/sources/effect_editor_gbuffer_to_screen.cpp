#include "pch.h"
#include "effect_editor_gbuffer_to_screen.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_editor_gbuffer_to_screen::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	// FUNCTION BODY[0x7c01e0]
	VOSTOK_UNREFERENCED_PARAMETER(config);

		compiler.begin_technique();
		compiler.begin_pass("post_process0", NULL, "gbuffer_to_screen", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_fill_mode(D3D_FILL_SOLID);
			compiler.set_alpha_blend(false);
			compiler.set_texture("t_position", r2_rt_p, 0, false, 0);
			compiler.set_texture("t_normal", r2_rt_n, 0, false, 0);
			compiler.set_texture("t_diffuse", r2_rt_albedo, 0, false, 0);
			compiler.set_texture("t_emissive",	r2_rt_emissive, 0, false, 0);
			compiler.set_texture("t_ssao_accumulator",	r2_rt_ssao_accumulator, 0, false, 0);
			compiler.set_texture("t_accumulator_dif", r2_rt_accum_diffuse, 0, false, 0);
			compiler.set_texture("t_accumulator_spec", r2_rt_accum_specular, 0, false, 0);
			compiler.set_texture("t_frame_color0", r2_rt_generic0, 0, false, 0);
			compiler.set_texture("t_frame_color1", r2_rt_generic1, 0, false, 0);
			compiler.set_texture("t_distortion", r2_rt_distortion, 0, false, 0);
			compiler.set_texture("t_frame_luminance", r2_rt_frame_luminance, 0, false, 0);
			compiler.set_texture("t_frame_luminance_histogram", r2_rt_frame_luminance_histogram, 0, false, 0);
			compiler.set_texture("t_decals_diffuse", r2_rt_decals_diffuse, 0, false, 0);
			compiler.set_texture("t_decals_normal", r2_rt_decals_normal, 0, false, 0);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
