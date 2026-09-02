// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_resolve_lighting.h"
#include <vostok/render/core/dx11/effect_compiler.h>

#include "shared_names.h"

namespace vostok {
namespace render {

void effect_resolve_lighting::compile(
	effect_compiler& compiler,
	custom_config_value const&
)
{
	compiler.begin_technique( );
	compiler.begin_pass( "resolve_lighting", NULL, "resolve_lighting", shader_configuration( ), NULL );

	compiler.set_texture( "t_diffuse", r2_rt_albedo, 0, false, u32( -1 ) );
	compiler.set_texture( "t_normal", r2_rt_n, 0, false, u32( -1 ) );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, u32( -1 ) );
	compiler.set_texture( "t_diffuse_accumulation", r2_rt_accum_diffuse, 0, false, u32( -1 ) );
	compiler.set_texture( "t_specular_accumulation", r2_rt_accum_specular, 0, false, u32( -1 ) );
	compiler.set_texture( "t_lpv_accumulation", "$user$lpv_accumulation", 0, false, u32( -1 ) );
	compiler.set_texture( "t_sun_translucensy_help_data", "$user$sun_translucensy_help_data", 0, false, u32( -1 ) );

	compiler.set_depth( false, false );
	compiler.set_stencil(
		true,
		0,
		0xff,
		0,
		D3D_COMPARISON_NOT_EQUAL,
		D3D_STENCIL_OP_KEEP,
		D3D_STENCIL_OP_KEEP,
		D3D_STENCIL_OP_KEEP
	);
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_fill_mode( D3D_FILL_SOLID );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
