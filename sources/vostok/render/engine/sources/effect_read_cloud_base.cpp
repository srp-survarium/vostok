#include "pch.h"
#include "effect_read_cloud_base.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_read_cloud_base::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{

	VOSTOK_UNREFERENCED_PARAMETER(config);




	compiler.begin_technique();
		compiler.begin_pass("cloud_read_base", NULL, "cloud_read_base", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_texture("t_cloud_base", "engine/cloud_base", 0, false, u32(-1));
			compiler.set_texture("t_base_noise", "engine/base_noise", 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
