#include "pch.h"
#include "effect_ssao_downsample_position_and_normal.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_ssao_downsample_position_and_normal::compile(
	effect_compiler& compiler,
	custom_config_value const& custom_config
)
{
	compiler.begin_technique( );
	compiler.begin_pass( "ssao_downsample_depth_and_normal", NULL, "ssao_downsample_depth_and_normal", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_alpha_blend( false );
	compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
	compiler.set_texture( "t_normal", "$user$normal", 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "ssao_downsample_depth_and_normal", NULL, "ssao_downsample_depth", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_alpha_blend( false );
	compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
