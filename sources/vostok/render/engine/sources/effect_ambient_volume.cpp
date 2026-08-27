#include "pch.h"
#include "effect_ambient_volume.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_ambient_volume::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	shader_configuration configuration;

	compiler.begin_technique( );
	compiler.begin_pass( "ambient_volume", NULL, "ambient_volume", configuration, NULL );
	compiler.set_depth( true, false );
	compiler.set_alpha_blend( true, D3D_BLEND_ZERO, D3D_BLEND_SRC_COLOR );
	compiler.set_stencil( true, 0xff, 0x40, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT, D3D_STENCIL_OP_INVERT );
	compiler.set_cull_mode( D3D_CULL_BACK );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "ambient_volume", NULL, "ambient_volume", configuration, NULL );
	compiler.set_depth( false, false );
	compiler.set_stencil( true, 0xff, 0x40, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT, D3D_STENCIL_OP_KEEP );
	compiler.set_alpha_blend( true, D3D_BLEND_ZERO, D3D_BLEND_SRC_COLOR );
	compiler.set_cull_mode( D3D_CULL_FRONT );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
