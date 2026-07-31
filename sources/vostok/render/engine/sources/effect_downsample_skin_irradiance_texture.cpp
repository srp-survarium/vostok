#include "pch.h"
#include "effect_downsample_skin_irradiance_texture.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_downsample_skin_irradiance_texture::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	// FUNCTION BODY[0x7b5320]
	VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

	compiler.begin_technique();
		compiler.begin_pass("blur_irradiance_texture", NULL, "downsample_irradiance_texture", shader_configuration(), NULL);
			compiler.set_depth( false, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_fill_mode(D3D_FILL_SOLID);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
