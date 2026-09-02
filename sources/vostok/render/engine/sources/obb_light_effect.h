// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_OBB_LIGHT_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_OBB_LIGHT_EFFECT_H_INCLUDED
#include <vostok/render/core/effect_descriptor.h>
#include <vostok/render/core/dx11/effect_compiler.h>

#include "geometry_type.h"
#include "shared_names.h"

namespace vostok {
namespace render {

class custom_config_value;

template < bool t_shadowed >
class obb_light_effect : public effect_descriptor {
	virtual void compile(
		effect_compiler& compiler,
		custom_config_value const& config
	) override
	{
		VOSTOK_UNREFERENCED_PARAMETER	( config);

		shader_configuration configuration;
		configuration.shadowed_light		= t_shadowed;

		compiler.begin_technique(); // front side of obb
			compiler.begin_pass	( "light", NULL, "obb_light_accumulator", configuration, NULL);
				compiler.set_depth			( true, false, D3D_COMPARISON_LESS_EQUAL);
				compiler.set_stencil		( true, 0xff, check_all_geometry_type, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT, D3D_STENCIL_OP_INVERT);
				compiler.set_alpha_blend	( true, D3D_BLEND_ONE, D3D_BLEND_ONE);
				compiler.set_cull_mode		( D3D_CULL_BACK);
				compiler.set_texture		( "t_position",    r2_rt_p, 0, false, -1);
				compiler.set_texture		( "t_normal",      r2_rt_n, 0, false, -1);
				compiler.set_texture		( "t_diffuse",     r2_rt_albedo, 0, false, -1);
				compiler.set_texture		( "t_material",    r2_material, 0, false, -1);
				compiler.set_texture		( "t_target_ex",   r2_rt_p_ex, 0, false, -1);
				compiler.set_texture		( "t_emissive",    r2_rt_emissive, 0, false, -1);
				compiler.set_texture		( "t_decals_diffuse", r2_rt_decals_diffuse, 0, false, -1);
				compiler.set_texture		( "t_decals_normal", r2_rt_decals_normal, 0, false, -1);
			compiler.end_pass();
		compiler.end_technique();

		compiler.begin_technique(); // back side of obb
			compiler.begin_pass	( "light", NULL, "obb_light_accumulator", shader_configuration(), NULL);
				compiler.set_depth			( false, false, D3D_COMPARISON_LESS_EQUAL);
				compiler.set_stencil		( true, 0xff, check_all_geometry_type, 0xff, D3D_COMPARISON_EQUAL, D3D_STENCIL_OP_KEEP, D3D_STENCIL_OP_INVERT, D3D_STENCIL_OP_KEEP);
				compiler.set_alpha_blend	( true, D3D_BLEND_ONE, D3D_BLEND_ONE);
				compiler.set_cull_mode		( D3D_CULL_FRONT);
				compiler.set_texture		( "t_position",    r2_rt_p, 0, false, -1);
				compiler.set_texture		( "t_normal",      r2_rt_n, 0, false, -1);
				compiler.set_texture		( "t_diffuse",     r2_rt_albedo, 0, false, -1);
				compiler.set_texture		( "t_material",    r2_material, 0, false, -1);
				compiler.set_texture		( "t_target_ex",   r2_rt_p_ex, 0, false, -1);
				compiler.set_texture		( "t_emissive",    r2_rt_emissive, 0, false, -1);
				compiler.set_texture		( "t_decals_diffuse", r2_rt_decals_diffuse, 0, false, -1);
				compiler.set_texture		( "t_decals_normal", r2_rt_decals_normal, 0, false, -1);
			compiler.end_pass();
		compiler.end_technique();
	}

public:
	obb_light_effect( ) { }
	virtual ~obb_light_effect( ) { }
};

typedef obb_light_effect< false > obb_light_effect_unshadowed;

STATIC_SIZE_ASSERT( obb_light_effect_unshadowed, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_OBB_LIGHT_EFFECT_H_INCLUDED
