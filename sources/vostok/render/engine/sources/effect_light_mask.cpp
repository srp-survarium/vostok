#include "pch.h"
#include "effect_light_mask.h"
#include "geometry_type.h"
#include "shared_names.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_light_mask::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	// FUNCTION BODY[0x7be000]
	VOSTOK_UNREFERENCED_PARAMETER	( config);





	compiler.begin_technique();
		compiler.begin_pass	("accum_mask", NULL, "dumb", shader_configuration(), NULL);
 			compiler.set_depth			( true, false, D3D_COMPARISON_LESS_EQUAL);


			compiler.set_stencil		( true, all_geometry_type, 0xff, 0xff, D3D_COMPARISON_LESS, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT);

			compiler.set_cull_mode		( D3D_CULL_FRONT);
			compiler.color_write_enable ( D3D_COLOR_WRITE_ENABLE_NONE);
		compiler.end_pass	();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass	( "stub_notransform_2pos", NULL, "accum_sun_mask", shader_configuration(), NULL);
			compiler.set_depth			( false, false);

			compiler.set_stencil		( true, all_geometry_type, 0xff, 0xff, D3D_COMPARISON_LESS, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT);
			compiler.set_cull_mode		( D3D_CULL_NONE);
			compiler.color_write_enable	( D3D_COLOR_WRITE_ENABLE_NONE);
			compiler.set_alpha_blend	( TRUE, D3D_BLEND_ZERO, D3D_BLEND_ONE);
			compiler.set_texture		( "t_normal", r2_rt_n, 0, false, u32(-1));
			compiler.set_texture		( "t_position", r2_rt_p, 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass	( "stub_notransform_t", NULL, "accum_sun_mask", shader_configuration(), NULL);
			compiler.set_depth			( false, false);
			compiler.set_stencil		( true, 0x80, 0x80, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INCR, D3D_STENCIL_OP_KEEP);
			compiler.set_cull_mode		( D3D_CULL_NONE);
			compiler.color_write_enable	( D3D_COLOR_WRITE_ENABLE_NONE);
			compiler.set_alpha_blend	( TRUE, D3D_BLEND_ZERO, D3D_BLEND_ONE);
			compiler.set_texture		( "t_normal", r2_rt_n, 0, false, u32(-1));
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
