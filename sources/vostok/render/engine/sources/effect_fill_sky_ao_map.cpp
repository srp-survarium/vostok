#include "pch.h"
#include "effect_fill_sky_ao_map.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_fill_sky_ao_map::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	VOSTOK_UNREFERENCED_PARAMETER(config);
	compiler.begin_technique();
		compiler.begin_pass("fill_sky_ao_map", NULL, "fill_sky_ao_map", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_fill_mode(D3D_FILL_SOLID);
			compiler.set_alpha_blend(false);
			compiler.set_texture("t_sao_map_texture", "$user$ssao_accumulator", 0, false, u32(-1));
			compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
