#include "pch.h"
#include "effect_post_process_blend_texture_materials.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_post_process_blend_texture_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	float4 modulate_color = float4( custom_config["constant_modulate_color"] );

	compiler.begin_technique( );
	compiler.begin_pass( "copy_image", NULL, "blend_texture", shader_configuration( ), NULL );
	compiler.set_texture( "t_base", pcstr( custom_config["texture_base"] ), 0, false, u32( -1 ) );
	compiler.set_constant( "modulate_color", modulate_color );
	compiler.set_depth( true, false );
	compiler.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
