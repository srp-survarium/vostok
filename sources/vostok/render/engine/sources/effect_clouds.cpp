#include "pch.h"
#include "effect_clouds.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_clouds::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{

	VOSTOK_UNREFERENCED_PARAMETER(config);
	compiler.begin_technique();
		compiler.begin_pass("clouds", NULL, "clouds", shader_configuration(), NULL);
			compiler.set_depth(true, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_stencil(true, 0, 0xff, 0, D3D_COMPARISON_EQUAL);
			compiler.set_alpha_blend(true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA);

			compiler.set_texture("t_noise", "engine/cloud_noise", 0, false, u32(-1));
			compiler.set_texture("t_3d_noise", "engine/noise_32x32x32", 0, false, u32(-1));
			compiler.set_texture("t_mie_scattering", "$user$mie_scattering", 0, false, u32(-1));
			compiler.set_texture("t_rayleigh_scattering", "$user$rayleigh_scattering", 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
