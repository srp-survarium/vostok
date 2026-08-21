#include "pch.h"
#include "effect_wireframe_colored.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_wireframe_colored::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	VOSTOK_UNREFERENCED_PARAMETER	( config );

 	compiler.begin_technique(/*SE_R2_SHADOW*/)
 		.begin_pass	("test", NULL, "color_fixed", shader_configuration(), NULL)
			.set_fill_mode	( D3D_FILL_WIREFRAME)
 		.end_pass	()
 	.end_technique();
}

} // namespace render
} // namespace vostok
