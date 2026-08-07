#include "pch.h"
#include "effect_apply_decal_normals.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_apply_decal::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	compiler.begin_technique( );
	compiler.begin_pass( "apply_decal", NULL, "apply_decal_normals_blend", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_texture( "t_decals_smoothness", "$user$decals_smoothness", 0, false, 0 );
	compiler.set_texture( "t_decals_normal", "$user$decals_normal", 0, false, 0 );
	compiler.set_texture( "t_normal", "$user$normal", 0, false, 0 );
	compiler.set_texture( "t_diffuse", "$user$albedo", 0, false, 0 );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );


	compiler.begin_technique( );
	compiler.begin_pass( "apply_decal", NULL, "apply_decal_normals_write", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_texture( "t_decals_blend_result", "$user$decals_blend_result", 0, false, 0 );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );


	compiler.begin_technique( );
	compiler.begin_pass( "apply_decal", NULL, "apply_decal_diffuse_write", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_texture( "t_decals_diffuse", "$user$decals_diffuse", 0, false, 0 );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL );
	compiler.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA );
	compiler.color_write_enable( static_cast<D3D11_COLOR_WRITE_ENABLE>( D3D_COLOR_WRITE_ENABLE_RED | D3D_COLOR_WRITE_ENABLE_GREEN | D3D_COLOR_WRITE_ENABLE_BLUE ) );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
