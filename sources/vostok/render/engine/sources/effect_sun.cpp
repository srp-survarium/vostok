#include "pch.h"
#include "effect_sun.h"

#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_sun::compile(
	effect_compiler& compiler,
	custom_config_value const&
)
{
	compiler.begin_technique( );
	compiler.begin_pass( "sun", NULL, "sun", shader_configuration( ), NULL );
	compiler.set_depth( false, false, D3D_COMPARISON_GREATER_EQUAL );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL );
	compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
	compiler.set_texture( "t_normal", "$user$normal", 0, false, u32( -1 ) );
	compiler.set_texture( "t_diffuse", "$user$albedo", 0, false, u32( -1 ) );
	compiler.set_texture( "t_material", "$user$material", 0, false, u32( -1 ) );
	compiler.set_texture( "t_emissive", "$user$emmisive", 0, false, u32( -1 ) );
	compiler.set_texture( "t_cascaded_shadow_map", "$user$cascaded_shadow_map", 0, false, u32( -1 ) );
	compiler.set_texture( "t_jitter_lookup", "$user$jitter_lookup", 0, false, u32( -1 ) );
	compiler.set_texture( "t_pcf_random_rotates", "engine/ssao_rotate", 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
