// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "effect_skylight.h"
#include <vostok/render/core/dx11/effect_compiler.h>
namespace vostok {
namespace render {

void effect_skylight::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{
	compiler.begin_technique( );
	compiler.begin_pass( "skylight", NULL, "skylight", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
	compiler.set_texture( "t_normal", "$user$normal", 0, false, u32( -1 ) );
	compiler.set_texture( "t_ssao_accumulator", "$user$ssao_accumulator_full_x", 0, false, u32( -1 ) );
	compiler.color_write_enable( static_cast<D3D11_COLOR_WRITE_ENABLE>( D3D_COLOR_WRITE_ENABLE_RED | D3D_COLOR_WRITE_ENABLE_GREEN | D3D_COLOR_WRITE_ENABLE_BLUE ) );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "skylight", NULL, "skylight", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
	compiler.set_texture( "t_normal", "$user$normal", 0, false, u32( -1 ) );
	compiler.set_texture( "t_ssao_accumulator", "$user$ssao_accumulator_full_x", 0, false, u32( -1 ) );
	compiler.color_write_enable( static_cast<D3D11_COLOR_WRITE_ENABLE>( D3D_COLOR_WRITE_ENABLE_RED | D3D_COLOR_WRITE_ENABLE_GREEN | D3D_COLOR_WRITE_ENABLE_BLUE ) );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
