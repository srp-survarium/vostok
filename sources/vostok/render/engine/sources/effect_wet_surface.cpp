#include "pch.h"
#include "effect_wet_surface.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_wet_surface::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	VOSTOK_UNREFERENCED_PARAMETER(config);

	compiler.begin_technique();
		compiler.begin_pass("wet_sufrace", NULL, "wet_sufrace_normal_modify", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));
			compiler.set_texture("t_normal", "$user$normal_copy", 0, false, u32(-1));
			compiler.set_texture("t_diffuse", "$user$albedo", 0, false, u32(-1));
			compiler.set_texture("t_rain_shadow_map", "$user$rain_shadow_map", 0, false, u32(-1));
			compiler.set_texture("t_water_flowing_normals", "engine/water_flowing_nmap", 0, false, u32(-1));
			compiler.set_texture("t_water_flowing_specular", "engine/water_flowing_spec", 0, false, u32(-1));
			compiler.set_texture("t_puddle_rings", "engine/rain_puddle_rings", 0, false, u32(-1));
			compiler.set_texture("t_rain_stream", "engine/rain_stream", 0, false, u32(-1));
			compiler.set_texture("t_rain_puddle", "engine/rain_puddle", 0, false, u32(-1));
			compiler.set_texture("t_rain_puddle_mask", "engine/rain_puddle_mask", 0, false, u32(-1));
			compiler.set_stencil(true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("wet_sufrace", NULL, "wet_sufrace_normal_write", shader_configuration(), NULL);
			compiler.set_depth(true, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_texture("t_unpacked_normal", "$user$generic1", 0, false, u32(-1));
			compiler.set_stencil(true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("wet_sufrace", NULL, "wet_sufrace_color_modify", shader_configuration(), NULL);
			compiler.set_depth(true, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));
			compiler.set_texture("t_normal", "$user$normal", 0, false, u32(-1));
			compiler.set_texture("t_diffuse", "$user$albedo", 0, false, u32(-1));
			compiler.set_texture("t_rain_shadow_map", "$user$rain_shadow_map", 0, false, u32(-1));
			compiler.set_texture("t_accum_diffuse", "$user$accum_diffuse", 0, false, u32(-1));
			compiler.set_texture("t_accum_specular", "$user$accum_specular", 0, false, u32(-1));
			compiler.set_stencil(true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("wet_sufrace", NULL, "wet_sufrace_color_write", shader_configuration(), NULL);
			compiler.set_depth(true, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_texture("t_accum_diffuse", "$user$generic0", 0, false, u32(-1));
			compiler.set_texture("t_accum_specular", "$user$generic1", 0, false, u32(-1));
			compiler.set_stencil(true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("wet_sufrace", NULL, "wet_sufrace_copy", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_stencil(true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
