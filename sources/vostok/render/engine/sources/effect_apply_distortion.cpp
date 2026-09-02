// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_apply_distortion.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>
namespace vostok {
namespace render {

void effect_apply_distortion::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{

	VOSTOK_UNREFERENCED_PARAMETER( custom_config );
	compiler.begin_technique( );
	compiler.begin_pass( "apply_distortion", NULL, "apply_distortion", shader_configuration( ), NULL );
	compiler.set_texture( "t_base", r2_rt_generic0, 0, false, u32( -1 ) );
	compiler.set_texture( "t_distortion", r2_rt_distortion, 0, false, u32( -1 ) );
	compiler.set_texture( "t_distortion_mask", "$user$distortion_mask", 0, false, u32( -1 ) );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_fill_mode( D3D_FILL_SOLID );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "apply_distortion", NULL, "apply_distortion_copy_result", shader_configuration( ), NULL );
	compiler.set_texture( "t_base", r2_rt_generic1, 0, false, u32( -1 ) );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_fill_mode( D3D_FILL_SOLID );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
