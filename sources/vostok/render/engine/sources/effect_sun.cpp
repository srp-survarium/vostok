#include "pch.h"
#include "effect_sun.h"
#include "geometry_type.h"
#include "shared_names.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_sun::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	// FUNCTION BODY[0x7bdbd0]
	// claude@NOTE: ported from legacy effect_light_direct<lighting_model> (template collapsed -
	// shipped effect_sun is a plain class; rename proven in temp/render_legacy/triage_log.md)
	VOSTOK_UNREFERENCED_PARAMETER	( config);

//	bool		b_hw_smap = true;//RImplementation.o.HW_smap;
//	bool		b_hw_pcf  = false;//RImplementation.o.HW_smap_PCF;
	bool		blend = FALSE;	//RImplementation.o.fp16_blend;
	D3D_BLEND	dest = blend ? D3D_BLEND_ONE : D3D_BLEND_ZERO;

//	if ( RImplementation.o.sunfilter)	{ blend = FALSE; dest = D3D_BLEND_ZERO; }

		compiler.begin_technique( /*SE_SUN_NEAR*/);
			compiler.begin_pass( "stub_notransform_2pos", NULL, "accum_sun_near", shader_configuration(), NULL);
			compiler.set_depth			( false, false, D3D_COMPARISON_GREATER_EQUAL);
			//compiler.set_stencil		( true, 0x00, 0xff, 0x0f, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INCR, D3D_STENCIL_OP_KEEP);
			compiler.set_stencil		( true, 0x00, 0xff, 0xff, D3D_COMPARISON_NOT_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_KEEP);
			//compiler.set_alpha_blend	( blend, D3D_BLEND_ONE, dest);
			compiler.set_alpha_blend	( true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD);
			compiler.set_cull_mode		( D3D_CULL_NONE);
			compiler.set_texture		( "t_position",		r2_rt_p, 0, false, 0);
			compiler.set_texture		( "t_target_ex",	r2_rt_p_ex, 0, false, 0);
			compiler.set_texture		( "t_normal",		r2_rt_n, 0, false, 0);
			compiler.set_texture		( "t_decals_diffuse", r2_rt_decals_diffuse, 0, false, 0);
			compiler.set_texture		( "t_decals_normal", r2_rt_decals_normal, 0, false, 0);
			//compiler.set_texture		( "t_accumulator",	r2_rt_accum); //used when fp16 blend is not available
			compiler.set_texture		( "t_material",		r2_material, 0, true, 0);
			compiler.set_texture		( "t_emissive",		r2_rt_emissive, 0, false, 0);
			//compiler.def_sampler		( "s_lmap");
			compiler.set_texture		( "t_lmap",			r2_sunmask, 0, true, 0);
			compiler.set_texture		( "t_depth",		r2_rt_depth, 0, false, 0);
			compiler.set_texture		( "t_shmap0",		r2_rt_shadow_map0, 0, false, 0);
			compiler.set_texture		( "t_shmap1",		r2_rt_shadow_map1, 0, false, 0);
			compiler.set_texture		( "t_shmap2",		r2_rt_shadow_map2, 0, false, 0);
			compiler.set_texture		( "t_shmap3",		r2_rt_shadow_map3, 0, false, 0);

			compiler.set_texture		( "t_tangents",		r2_rt_tangents, 0, false, 0);
		//jitter( compiler);
		compiler.end_pass();
			compiler.end_technique();

	compiler.begin_technique( /*tech_sun_near_mat2*/)
		.begin_pass( "stub_notransform_2pos", NULL, "accum_sun_near_mat2", shader_configuration(), NULL)

			.set_depth			( false, false, D3D_COMPARISON_GREATER_EQUAL)
			.set_stencil		( true, 0x00, 0xff, 0x00, D3D_COMPARISON_LESS_EQUAL)
			.set_alpha_blend	( blend, D3D_BLEND_ONE, dest)
			.set_cull_mode		( D3D_CULL_NONE)
			.set_texture		( "t_position",		r2_rt_p, 0, false, 0)
			.set_texture		( "t_normal",		r2_rt_n, 0, false, 0)
			.set_texture		( "t_decals_diffuse", r2_rt_decals_diffuse, 0, false, 0)
			.set_texture		( "t_decals_normal", r2_rt_decals_normal, 0, false, 0)
			//.set_texture		( "t_accumulator",	r2_rt_accum) //used when fp16 blend is not available
			.set_texture		( "t_material",		r2_material, 0, true, 0)
			//.def_sampler		( "s_lmap")
			.set_texture		( "t_lmap",			r2_sunmask, 0, true, 0);

			//jitter( compiler);
		compiler.end_pass()
	.end_technique();

	compiler.begin_technique( /*tech_sun_near_clip*/)
		.begin_pass( "stub_notransform_2pos", NULL, "accum_sun_near_no_mask", shader_configuration(), NULL)

			.set_depth			( false, false, D3D_COMPARISON_GREATER_EQUAL)
			.set_stencil		( true, 0x00, 0xff, 0x00, D3D_COMPARISON_LESS_EQUAL)
			.set_alpha_blend	( blend, D3D_BLEND_ONE, dest)
			.set_cull_mode		( D3D_CULL_NONE)
			.set_texture		( "t_position",		r2_rt_p, 0, false, 0)
			.set_texture		( "t_normal",		r2_rt_n, 0, false, 0)
			.set_texture		( "t_decals_diffuse", r2_rt_decals_diffuse, 0, false, 0)
			.set_texture		( "t_decals_normal", r2_rt_decals_normal, 0, false, 0)
			//.set_texture		( "t_accumulator",	r2_rt_accum) //used when fp16 blend is not available
			.set_texture		( "t_material",		r2_material, 0, true, 0)
			//.def_sampler		( "s_lmap")
			.set_texture		( "t_lmap",			r2_sunmask, 0, true, 0);

			//jitter( compiler);
		compiler.end_pass()
	.end_technique();

	compiler.begin_technique( /*tech_sun_near_mat2_clip*/)
		.begin_pass( "stub_notransform_2pos", NULL, "accum_sun_near_mat2_no_mask", shader_configuration(), NULL)

			.set_depth			( false, false, D3D_COMPARISON_GREATER_EQUAL)
			.set_stencil		( true, 0x00, 0xff, 0x00, D3D_COMPARISON_LESS_EQUAL)
			.set_alpha_blend	( blend, D3D_BLEND_ONE, dest)
			.set_cull_mode		( D3D_CULL_NONE)
			.set_texture		( "t_position",		r2_rt_p, 0, false, 0)
			.set_texture		( "t_normal",		r2_rt_n, 0, false, 0)
			.set_texture		( "t_decals_diffuse", r2_rt_decals_diffuse, 0, false, 0)
			.set_texture		( "t_decals_normal", r2_rt_decals_normal, 0, false, 0)
			//.set_texture		( "t_accumulator",	r2_rt_accum) //used when fp16 blend is not available
			.set_texture		( "t_material",		r2_material, 0, true, 0)
			//.def_sampler		( "s_lmap")
			.set_texture		( "t_lmap",			r2_sunmask, 0, true, 0);

			//jitter( compiler);
		compiler.end_pass()
	.end_technique();

		compiler.begin_technique( /*tech_sun_near_branch*/)
			.begin_pass( "stub_notransform_2pos", NULL, "accum_sun_near_branch", shader_configuration(), NULL)

			.set_depth			( false, false, D3D_COMPARISON_GREATER_EQUAL)
			.set_stencil		( true, 0x00, 0xC0, 0x00, D3D_COMPARISON_EQUAL)
			.set_alpha_blend	( blend, D3D_BLEND_ONE, dest)
			.set_cull_mode		( D3D_CULL_NONE)
			.set_texture		( "t_position",		r2_rt_p, 0, false, 0)
			.set_texture		( "t_normal",		r2_rt_n, 0, false, 0)
			.set_texture		( "t_decals_diffuse", r2_rt_decals_diffuse, 0, false, 0)
			.set_texture		( "t_decals_normal", r2_rt_decals_normal, 0, false, 0)
			//.set_texture		( "t_accumulator",	r2_rt_accum) //used when fp16 blend is not available
			.set_texture		( "t_material",		r2_material, 0, true, 0)
			//.def_sampler		( "s_lmap")
			.set_texture		( "t_lmap",			r2_sunmask, 0, true, 0);

		//jitter( compiler);
		compiler.end_pass()
			.end_technique();

		compiler.begin_technique( /*tech_sun_near_mat2_branch*/)
			.begin_pass( "stub_notransform_2pos", NULL, "accum_sun_near_mat2_branch", shader_configuration(), NULL)

			.set_depth			( false, false, D3D_COMPARISON_GREATER_EQUAL)
			.set_stencil		( true, 0x00, 0xC0, 0x00, D3D_COMPARISON_EQUAL)
			.set_alpha_blend	( blend, D3D_BLEND_ONE, dest)
			.set_cull_mode		( D3D_CULL_NONE)
			.set_texture		( "t_position",		r2_rt_p, 0, false, 0)
			.set_texture		( "t_normal",		r2_rt_n, 0, false, 0)
			.set_texture		( "t_decals_diffuse", r2_rt_decals_diffuse, 0, false, 0)
			.set_texture		( "t_decals_normal", r2_rt_decals_normal, 0, false, 0)
			//.set_texture		( "t_accumulator",	r2_rt_accum) //used when fp16 blend is not available
			.set_texture		( "t_material",		r2_material, 0, true, 0)
			//.def_sampler		( "s_lmap")
			.set_texture		( "t_lmap",			r2_sunmask, 0, true, 0);

		//jitter( compiler);
		compiler.end_pass()
			.end_technique();
}

} // namespace render
} // namespace vostok
