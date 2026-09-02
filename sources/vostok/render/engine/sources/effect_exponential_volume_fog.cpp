// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_exponential_volume_fog.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_exponential_volume_fog::compile(
	effect_compiler& compiler,
	custom_config_value const&
)
{
	compiler.begin_technique( );
	compiler.begin_pass( "volume_fog", NULL, "volume_fog", shader_configuration( ), NULL );
	compiler.set_depth( true, false );
	compiler.set_cull_mode( D3D_CULL_FRONT );
	compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
	compiler.set_texture( "t_fog_noise", "engine/volume_fog_noise", 0, false, u32( -1 ) );
	compiler.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
