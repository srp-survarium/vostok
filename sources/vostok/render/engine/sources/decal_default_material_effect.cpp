#include "pch.h"
#include "decal_default_material_effect.h"

#include <vostok/render/core/custom_config_value.h>
#include <vostok/render/core/dx11/effect_compiler.h>

#include "shared_names.h"

namespace vostok {
namespace render {

void decal_default_material_effect::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	shader_configuration configuration;

	configuration.use_diffuse_texture	= bool(config["use_tdiffuse"]);
	configuration.use_normal_texture	= m_is_forward ? false : bool(config["use_nmap"]);
	configuration.use_reflection		= config.value_exists("use_reflection") ? (bool(config["use_reflection"]) ? 1 : 0) : false;
	configuration.decal_material		= true;
	configuration.decal_type			= m_is_forward ? 1 : 0;

	for (u32 pass_index = 0; pass_index < 2; pass_index++)
	{
		compile_begin("vertex_base", "decal_base", compiler, &configuration, config);

			compiler.set_depth( false, false, D3D_COMPARISON_LESS_EQUAL );
			compiler.set_stencil( false );

			if (pass_index == 0)
			{
				// 5 target lines are likely retail-compiled-out source.
				compiler.set_cull_mode	(D3D_CULL_BACK);
			}
			else
			{
				// 5 target lines are likely retail-compiled-out source.
				compiler.set_cull_mode	(D3D_CULL_FRONT);
			}

			compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL );

			if (!m_is_forward)
				compiler.set_alpha_blend(true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD);

			if (configuration.use_diffuse_texture)
				compiler.set_texture	("t_base", pcstr(config["texture_diffuse"]), 0, true, 0);

			if (config.value_exists("constant_diffuse"))
				compiler.set_constant	("constant_diffuse", math::float4(config["constant_diffuse"]));

			if (config.value_exists("normal_multiplier"))
				compiler.set_constant	("constant_normal_multiplier", math::float3(config["normal_multiplier"]));

			compiler.set_constant(
				"smoothness_interpolation",
				config.value_exists("smoothness_interpolation") ? float(config["smoothness_interpolation"]) : 0.0f
			);

			if (config.value_exists("fresnel_multiplier"))
				compiler.set_constant("fresnel_multiplier", float(config["fresnel_multiplier"]));

			if (config.value_exists("diffuse_alpha"))
				compiler.set_constant("diffuse_alpha", float(config["diffuse_alpha"]));

			if (configuration.use_normal_texture)
				compiler.set_texture	("t_normal_map", pcstr(config["texture_normal"]), 0, true, 0);

			compiler.set_texture		("t_position", r2_rt_p, 0, false, 0);
			compiler.set_texture		("t_normal", r2_rt_n, 0, false, 0);

			if (config.value_exists("blend_with_geometry_normals"))
				compiler.set_constant			(
					"blend_with_geometry_normals",
					config.value_exists("blend_with_geometry_normals") ?
						(bool(config["blend_with_geometry_normals"]) ? 1.0f : -1.0f):
						-1.0f
				);

			if (configuration.use_reflection)
				compiler.set_texture("t_cubemap", pcstr(config["texture_cubemap"]), 0, true, 0);

		compile_end(compiler);
	}
}

} // namespace render
} // namespace vostok
