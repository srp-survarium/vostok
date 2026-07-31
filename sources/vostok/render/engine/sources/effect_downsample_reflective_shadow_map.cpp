#include "pch.h"
#include "effect_downsample_reflective_shadow_map.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_downsample_reflective_shadow_map::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	// FUNCTION BODY[0x7b5280]
	VOSTOK_UNREFERENCED_PARAMETER(config);

	compiler.begin_technique();
		compiler.begin_pass("lpv_downsample_reflective_shadow_map", NULL, "lpv_downsample_reflective_shadow_map", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_texture("t_lpv_rsm_albedo_source", r2_rt_lpv_rsm_albedo_source, 0, false, 0);
			compiler.set_texture("t_lpv_rsm_normal_source", r2_rt_lpv_rsm_normal_source, 0, false, 0);
			compiler.set_texture("t_lpv_rsm_position_source", r2_rt_lpv_rsm_position_source, 0, false, 0);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
