// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_lens_flares.h"
#include <vostok/render/core/dx11/effect_compiler.h>
namespace vostok {
namespace render {

void effect_lens_flares::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	VOSTOK_UNREFERENCED_PARAMETER( custom_config );
	compiler.begin_technique( );
	compiler.begin_pass( "lens_flares", NULL, "lens_flares", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_stencil( false );
	compiler.set_alpha_blend( false, D3D_BLEND_SRC_ALPHA, D3D_BLEND_ONE );
	compiler.set_texture( "t_blurred_frame_bloom_only_color", "$user$blur3", 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
