#include "pch.h"
#include "effect_distortion_stage_default_materials.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_distortion_stage_default_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	vostok::math::float3 distortion_scale = vostok::math::float3( config["distortion_scale"] );
	shader_configuration shader_config;
	compile_begin( "vertex_base", "distortion_base", compiler, &shader_config, config );
	compiler.set_stencil( false, 0x80, 0xff, 0xff, D3D_COMPARISON_ALWAYS, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_REPLACE, D3D_STENCIL_OP_KEEP );
	compiler.set_texture( "t_base", pcstr( config["texture_distortion"] ), 0, false, u32( -1 ) );

	compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );

	compiler.set_constant( "distortion_scale", distortion_scale );
	compiler.set_depth( true, false );
	compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE );
	compile_end( compiler );
}

} // namespace render
} // namespace vostok
