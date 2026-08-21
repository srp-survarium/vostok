#include "pch.h"
#include "effect_hiz_occlusion.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_hiz_occlusion::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{

	VOSTOK_UNREFERENCED_PARAMETER( config );
	compiler.begin_technique( );
	compiler.begin_pass( "dumb", NULL, "hiz_debug_color", shader_configuration( ), NULL );
	compiler.set_depth( true, true );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "dumb", NULL, "hiz_debug_color", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "hiz_copy_scene_depth", NULL, "hiz_copy_scene_depth", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "hiz_depth", NULL, "hiz_depth", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "hiz_downsample_depth", NULL, "hiz_downsample_depth", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "hiz_merge_mip", NULL, "hiz_merge_mip", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "hiz_fill_culling_results_buffer", NULL, "hiz_fill_culling_results_buffer", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );

	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "hiz_copy_scene_depth", NULL, "hiz_copy_to_lockable_rt", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );

	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
