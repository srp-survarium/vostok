#include "pch.h"
#include "effect_copy_image.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_copy_image::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{

	VOSTOK_UNREFERENCED_PARAMETER( custom_config );

	compiler.begin_technique( );
	compiler.begin_pass( "copy_image", NULL, "copy_image", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_fill_mode( D3D_FILL_SOLID );
	compiler.set_alpha_blend( false );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "copy_image", NULL, "copy_image_simple", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_fill_mode( D3D_FILL_SOLID );
	compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
