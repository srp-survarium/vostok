#include "pch.h"
#include "effect_post_process_distortion_materials.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_post_process_distortion_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	// 3 target lines are likely retail-compiled-out source.
	float distortion_scale = float( custom_config["constant_distortion_scale"] );

	shader_configuration shader_config;

	compile_begin( "post_process", "post_process_distortion", compiler, &shader_config, custom_config );
	compiler.set_texture( "t_base", pcstr( custom_config["texture_base"] ), 0, false, u32( -1 ) );
	compiler.set_constant( "distortion_scale", distortion_scale );
	compiler.set_depth( true, false );

	compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE );
	compile_end( compiler );
}

} // namespace render
} // namespace vostok
