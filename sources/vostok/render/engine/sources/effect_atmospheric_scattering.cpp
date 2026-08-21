#include "pch.h"
#include "effect_atmospheric_scattering.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_atmospheric_scattering::compile(
	effect_compiler& compiler,
	custom_config_value const& config
)
{

	VOSTOK_UNREFERENCED_PARAMETER( config );
	compiler.begin_technique( );
	compiler.begin_pass( "make_mie_rayleigh_texture", NULL, "make_mie_rayleigh_texture", shader_configuration( ), NULL );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "atmosphere", NULL, "atmosphere", shader_configuration( ), NULL );
	compiler.set_texture( "t_mie_scattering", "$user$mie_scattering", 0, false, u32( -1 ) );
	compiler.set_texture( "t_rayleigh_scattering", "$user$rayleigh_scattering", 0, false, u32( -1 ) );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_depth( false, false );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "atmosphere_clouds", NULL, "atmosphere_clouds_alpha_blend", shader_configuration( ), NULL );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_depth( false, false );
	compiler.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "atmosphere_clouds", NULL, "atmosphere_clouds_modulate", shader_configuration( ), NULL );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_depth( false, false );
	compiler.set_alpha_blend( true, D3D_BLEND_ZERO, D3D_BLEND_SRC_COLOR );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "atmosphere_sun_moon", NULL, "atmosphere_sun_moon", shader_configuration( ), NULL );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_depth( false, false );
	compiler.set_alpha_blend( true, D3D_BLEND_INV_DEST_ALPHA, D3D_BLEND_DEST_ALPHA );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "atmosphere_sun_moon", NULL, "atmosphere_sun_moon", shader_configuration( ), NULL );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_depth( false, false );
	compiler.set_alpha_blend( true, D3D_BLEND_DEST_ALPHA, D3D_BLEND_INV_DEST_ALPHA );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "atmospheric_scattering_on_geometry", NULL, "atmospheric_scattering_on_geometry_mul", shader_configuration( ), NULL );
	compiler.set_stencil( true, 0, 0xff, 0xff, D3D_COMPARISON_NOT_EQUAL );
	compiler.set_texture( "t_mie_scattering", "$user$mie_scattering", 0, false, u32( -1 ) );
	compiler.set_texture( "t_rayleigh_scattering", "$user$rayleigh_scattering", 0, false, u32( -1 ) );
	compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
	compiler.set_alpha_blend( true, D3D_BLEND_ZERO, D3D_BLEND_SRC_COLOR );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_depth( false, false );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "atmospheric_scattering_on_geometry", NULL, "atmospheric_scattering_on_geometry_add", shader_configuration( ), NULL );
	compiler.set_stencil( true, 0, 0xff, 0xff, D3D_COMPARISON_NOT_EQUAL );
	compiler.set_texture( "t_mie_scattering", "$user$mie_scattering", 0, false, u32( -1 ) );
	compiler.set_texture( "t_rayleigh_scattering", "$user$rayleigh_scattering", 0, false, u32( -1 ) );
	compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
	compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_depth( false, false );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
