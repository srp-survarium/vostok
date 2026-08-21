#ifndef VOSTOK_RENDER_ENGINE_EFFECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_EFFECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED

#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

class custom_config_value;
class effect_compiler;

class effect_sky_ambient_occlusion : public effect_descriptor {
public:
	virtual void compile(
		effect_compiler& compiler,
		custom_config_value const& config
	) override
	{
		shader_configuration configuration;

		compiler.begin_technique( );
		compiler.begin_pass( "sky_ambient_occlusion", NULL, "sky_ambient_occlusion", configuration, NULL );
		compiler.set_depth( true, false );
		compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE );
		compiler.set_stencil( true, 0xff, 0x40, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT, D3D_STENCIL_OP_INVERT );
		compiler.set_cull_mode( D3D_CULL_NONE );
		compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
		compiler.set_texture( "t_normal", "$user$normal", 0, false, u32( -1 ) );
		compiler.end_pass( );
		compiler.end_technique( );

		compiler.begin_technique( );
		compiler.begin_pass( "sky_ambient_occlusion", NULL, "sky_ambient_occlusion", configuration, NULL );
		compiler.set_depth( false, false );
		compiler.set_stencil( true, 0xff, 0x40, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT, D3D_STENCIL_OP_KEEP );
		compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE );
		compiler.set_cull_mode( D3D_CULL_BACK );
		compiler.set_texture( "t_position", "$user$position", 0, false, u32( -1 ) );
		compiler.set_texture( "t_normal", "$user$normal", 0, false, u32( -1 ) );
		compiler.end_pass( );
		compiler.end_technique( );
	}
};

STATIC_SIZE_ASSERT( effect_sky_ambient_occlusion, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_EFFECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED
