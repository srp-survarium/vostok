#include "pch.h"
#include "effect_reflection_mask.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_reflection_mask::compile(
	effect_compiler& compiler,
	custom_config_value const&
)
{
	compiler.begin_technique( );
	compiler.begin_pass( "reflection_mask", NULL, "reflection_mask", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_texture( "t_position", "$user$gbuffer_position_normal_downsampled_x2", 0, false, u32(-1) );
	compiler.set_texture( "t_normal", "$user$normal", 0, false, u32(-1) );
	compiler.set_texture( "t_diffuse", "$user$albedo", 0, false, u32(-1) );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
