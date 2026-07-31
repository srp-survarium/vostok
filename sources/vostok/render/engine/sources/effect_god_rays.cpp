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
	// FUNCTION BODY[0x7bc500]
	// claude@NOTE: ported from legacy effect_gather_sun_light_scattering_zone
	// (rename proven in temp/render_legacy/triage_log.md)
	VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

	compiler.begin_technique();
		compiler.begin_pass("gather_bloom", NULL, "gather_sun_light_scattering_zone", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			//compiler.set_stencil(true, 0x80, 0x80, 0x00, D3D_COMPARISON_LESS_EQUAL);
			compiler.set_texture("t_position", r2_rt_p, 0, false, 0);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
