#include "pch.h"
#include "effect_god_rays.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_god_rays::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

	compiler.begin_technique();
		compiler.begin_pass("god_rays", NULL, "god_rays_mask", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_texture("t_position", r2_rt_p, 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();


	compiler.begin_technique();
		compiler.begin_pass("god_rays", NULL, "god_rays", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_texture("t_light_scattering_mask", "$user$light_scattering_mask", 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
