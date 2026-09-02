// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_rain.h"
#include <vostok/render/core/dx11/effect_compiler.h>
namespace vostok {
namespace render {

void effect_rain::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	VOSTOK_UNREFERENCED_PARAMETER( config );
	compiler.begin_technique( );
	compiler.begin_pass( "rain", NULL, "rain", shader_configuration( ), NULL );

	compiler.set_depth( true, false );
	compiler.set_cull_mode( D3D_CULL_NONE );

	compiler.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA );

	compiler.set_texture( "t_scroll_rain", "engine/scroll_rain_0", 0, false, u32( -1 ) );
	compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
	compiler.set_texture( "t_rain_shadow_map", "$user$rain_shadow_map", 0, false, u32( -1 ) );
	compiler.set_texture( "t_frame_color", "$user$generic1", 0, false, u32( -1 ) );
	compiler.set_texture( "t_blurred_frame_bloom_only_color", "$user$blur3", 0, false, u32( -1 ) );
	compiler.set_texture( "t_diffuse_lighting", "$user$accum_diffuse", 0, false, u32( -1 ) );

	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "rain_resolve", NULL, "rain_resolve", shader_configuration( ), NULL );
	compiler.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA );
	compiler.set_texture( "t_rain", "$user$rain_result", 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
