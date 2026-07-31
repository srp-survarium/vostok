#include "pch.h"
#include "effect_shadow_map.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_shadow_map::compile(
	effect_compiler&			compiler,
	custom_config_value const&
)
{
	// FUNCTION BODY[0x7b5560]
	compiler.begin_technique( )
			.begin_pass	( "dumb", NULL, "dumb", shader_configuration(), NULL)
				.set_depth		( true, true)
				.color_write_enable( D3D_COLOR_WRITE_ENABLE_NONE)
			.end_pass	( )
		.end_technique();
}

} // namespace render
} // namespace vostok
