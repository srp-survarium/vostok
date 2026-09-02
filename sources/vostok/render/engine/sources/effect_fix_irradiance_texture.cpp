// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_fix_irradiance_texture.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>
namespace vostok {
namespace render {

void effect_fix_irradiance_texture::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{

	VOSTOK_UNREFERENCED_PARAMETER( custom_config );

	compiler.begin_technique( );
	compiler.begin_pass( "blur_irradiance_texture", NULL, "fix_irradiance_texture", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_fill_mode( D3D_FILL_SOLID );
	compiler.set_texture( "t_skin_scattering_temp", r2_rt_skin_scattering_temp, 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
