#include "pch.h"
#include "effect_clouds_god_rays.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_clouds_god_rays::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{

	VOSTOK_UNREFERENCED_PARAMETER(config);
	// 4 target lines are likely retail-compiled-out source.
	compiler.begin_technique();
		compiler.begin_pass("god_rays_accumulate3", NULL, "god_rays_accumulate3", shader_configuration(), NULL);
			compiler.set_depth(true, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_alpha_blend(true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA);

			compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));
			compiler.set_texture("t_cloud_lighting", "$user$generic1", 0, false, u32(-1));


		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
