#ifndef VOSTOK_RENDER_ENGINE_EFFECT_ENVIRONMENT_PROBE_LIGHTING_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_ENVIRONMENT_PROBE_LIGHTING_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

template <
	bool t_option_0,
	bool t_option_1,
	bool t_option_2
>
class effect_environment_probe_lighting : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler& compiler,
		custom_config_value const& config
	) override
	{
		shader_configuration configuration;
		configuration.env_probe_clip_by_normal = t_option_0;
		configuration.env_probe_with_shadows = t_option_1;
		configuration.env_probe_geometry_type = t_option_2;

		compiler.begin_technique( );
		compiler.begin_pass( "light", NULL, "environment_probe_lighting", configuration, NULL );
		compiler.set_depth( true, false );
		compiler.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_ONE );
		compiler.set_stencil( true, 0xff, 0x40, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT, D3D_STENCIL_OP_INVERT );
		compiler.set_cull_mode( D3D_CULL_BACK );
		compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
		compiler.set_texture( "t_normal", "$user$normal", 0, false, u32( -1 ) );
		compiler.set_texture( "t_diffuse", "$user$albedo", 0, false, u32( -1 ) );
		compiler.set_texture( "t_probe_cubemap", "environment_probes/environment_probe", 0, false, u32( -1 ) );
		compiler.set_texture( "t_ssao_accumulator", "$user$ssao_accumulator_full_x", 0, false, u32( -1 ) );
		compiler.color_write_enable( static_cast<D3D11_COLOR_WRITE_ENABLE>( D3D_COLOR_WRITE_ENABLE_RED | D3D_COLOR_WRITE_ENABLE_GREEN | D3D_COLOR_WRITE_ENABLE_BLUE ) );
		compiler.end_pass( );
		compiler.end_technique( );

		compiler.begin_technique( );
		compiler.begin_pass( "light", NULL, "environment_probe_lighting", configuration, NULL );
		compiler.set_depth( false, false );
		compiler.set_stencil( true, 0xff, 0x40, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT, D3D_STENCIL_OP_KEEP );
		compiler.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_ONE );
		compiler.set_cull_mode( D3D_CULL_FRONT );
		compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
		compiler.set_texture( "t_normal", "$user$normal", 0, false, u32( -1 ) );
		compiler.set_texture( "t_diffuse", "$user$albedo", 0, false, u32( -1 ) );
		compiler.set_texture( "t_probe_cubemap", "environment_probes/environment_probe", 0, false, u32( -1 ) );
		compiler.set_texture( "t_ssao_accumulator", "$user$ssao_accumulator_full_x", 0, false, u32( -1 ) );
		compiler.color_write_enable( static_cast<D3D11_COLOR_WRITE_ENABLE>( D3D_COLOR_WRITE_ENABLE_RED | D3D_COLOR_WRITE_ENABLE_GREEN | D3D_COLOR_WRITE_ENABLE_BLUE ) );
		compiler.end_pass( );
		compiler.end_technique( );
	}

};

typedef effect_environment_probe_lighting<
	false,
	false,
	false
> effect_environment_probe_lighting_000;

STATIC_SIZE_ASSERT( effect_environment_probe_lighting_000, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_ENVIRONMENT_PROBE_LIGHTING_H_INCLUDED
