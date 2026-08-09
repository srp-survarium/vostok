#include "pch.h"
#include "effect_lighting_stage_skin_base_materials.h"
#include "shared_names.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/facade/light_props.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_lighting_stage_skin_base_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	// FUNCTION BODY[0x5eca00]
	{
		shader_configuration configuration;


		compile_begin("unwrap_mesh", "skin_position_pass", compiler, &configuration, custom_config);
			compiler.set_alpha_blend(false);
		compile_end(compiler);
	}

	{
		shader_configuration configuration;

		configuration.use_diffuse_texture			 = bool(custom_config["use_tdiffuse"]);
		configuration.use_normal_texture			 = bool(custom_config["use_nmap"]);
		configuration.use_alpha_test				 = custom_config.value_exists("use_alpha_test") ? bool(custom_config["use_alpha_test"]) : false;

		configuration.use_specular_intensity_texture = custom_config.value_exists("use_tspecular_inensity") ? bool(custom_config["use_tspecular_inensity"]) : false;
		configuration.use_fresnel_texture = custom_config.value_exists("use_tfresnel") ? bool(custom_config["use_tfresnel"]) : false;
		configuration.use_roughness_texture = custom_config.value_exists("use_troughness") ? bool(custom_config["use_troughness"]) : false;
		// 4 target lines are likely retail-compiled-out source.
		configuration.light_type = 0;
		configuration.shadowed_light				= true;

		compile_begin("unwrap_mesh", "skin_forward_lighting", compiler, &configuration, custom_config);
			// 3 target lines are likely retail-compiled-out source.
			compiler.set_depth( false, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
			compiler.set_fill_mode(D3D_FILL_SOLID);
			compiler.set_alpha_blend(false);
			// 21 target lines are likely retail-compiled-out source.
			if( configuration.use_diffuse_texture)
				compiler.set_texture("t_base", pcstr(custom_config["texture_diffuse"]), 0, false, u32(-1));


			float4 solid_color_specular = float4(custom_config["constant_diffuse"]);
			solid_color_specular.w = 0.0f;
			// 3 target lines are likely retail-compiled-out source.
			if( configuration.use_normal_texture)
				compiler.set_texture("t_normal", pcstr(custom_config["texture_normal"]), 0, false, u32(-1));

			if( configuration.use_specular_intensity_texture)
				compiler.set_texture("t_specular_intensity", pcstr(custom_config["texture_specular_intensity"]), 0, false, u32(-1));

			if (custom_config.value_exists("constant_specular_color") && custom_config.value_exists("constant_specular_color_multiplier"))
				compiler.set_constant( "specular_color_parameter", float4(custom_config["constant_specular_color"]).xyz() * float3(custom_config["constant_specular_color_multiplier"]));


			compiler.set_constant( "solid_color_specular", solid_color_specular);

			float specular_power = 50.0f;
			// 4 target lines are likely retail-compiled-out source.
			if( configuration.use_roughness_texture)
				compiler.set_texture("t_specular_power", pcstr(custom_config["texture_specular_power"]), 0, false, u32(-1));
			else
			{
				if (custom_config.value_exists("constant_specular_power"))
					specular_power = float(custom_config["constant_specular_power"]);
			}
			compiler.set_constant("specular_power_parameter", specular_power);

			float   solid_transparency   = 1.0f;
			compiler.set_constant("solid_transparency",   solid_transparency);
		// 4 target lines are likely retail-compiled-out source.
		compile_end(compiler);
	}

	{
		shader_configuration configuration;
		configuration.use_organic_scattering_depth_texture		= custom_config.value_exists("use_scattering_depth_texture") ? bool(custom_config["use_scattering_depth_texture"]) : false;

		compiler.begin_technique();
			compiler.begin_pass("blur_irradiance_texture", NULL, "blur_skin_irradiance_texture", configuration, NULL);
				compiler.set_depth( false, false);
				compiler.set_cull_mode(D3D_CULL_NONE);
				compiler.set_fill_mode(D3D_FILL_SOLID);
				compiler.set_alpha_blend(false, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA);
				compiler.set_texture("t_skin_scattering", "$user$skin_scattering", 0, false, u32(-1));
				compiler.set_texture("t_skin_scattering_temp", "$user$skin_scattering_temp", 0, false, u32(-1));
				compiler.set_texture("t_blurring_stretch", "$user$skin_scattering_stretch", 0, false, u32(-1));

				float4 scattering_component_blurring_weights_and_color_multiplier = float4(1.2f, 0.3f, 0.1f, 1.0f);

				if (custom_config.value_exists("scattering_component_blurring_weights"))
					scattering_component_blurring_weights_and_color_multiplier = float4(custom_config["scattering_component_blurring_weights"]);

				scattering_component_blurring_weights_and_color_multiplier *= scattering_component_blurring_weights_and_color_multiplier.w;

				if (custom_config.value_exists("scattering_color_multiplier"))
					scattering_component_blurring_weights_and_color_multiplier.w = float(custom_config["scattering_color_multiplier"]);

				if (configuration.use_organic_scattering_depth_texture && custom_config.value_exists("texture_scattering_depth"))
					compiler.set_texture("t_scattering_depth", pcstr(custom_config["texture_scattering_depth"]), 0, false, u32(-1));

				compiler.set_constant("scattering_component_blurring_weights_and_color_multiplier", scattering_component_blurring_weights_and_color_multiplier);

			compiler.end_pass();
		compiler.end_technique();
	}

	{
		shader_configuration configuration;
		configuration.use_diffuse_texture = custom_config.value_exists("use_tdiffuse") ? bool(custom_config["use_tdiffuse"]) : false;
		configuration.use_normal_texture  = custom_config.value_exists("use_nmap") ? bool(custom_config["use_nmap"]) : false;
		configuration.use_specular_intensity_texture = custom_config.value_exists("use_tspecular_inensity") ? bool(custom_config["use_tspecular_inensity"]) : false;


		configuration.use_ao_texture							= custom_config.value_exists("use_ao_texture") ? bool(custom_config["use_ao_texture"]) : false;
		configuration.use_organic_scattering_amount_mask		= custom_config.value_exists("use_scattering_amount_mask") ? bool(custom_config["use_scattering_amount_mask"]) : false;
		configuration.use_organic_back_illumination_texture		= custom_config.value_exists("use_back_illumination_texture") ? bool(custom_config["use_back_illumination_texture"]) : false;
		configuration.use_organic_subdermal_texture				= custom_config.value_exists("use_subdermal_texture") ? bool(custom_config["use_subdermal_texture"]) : false;

		compile_begin("vertex_base", "skin_combine", compiler, &configuration, custom_config);
			// 5 target lines are likely retail-compiled-out source.
			compiler.set_depth( true, false);
			compiler.set_alpha_blend(true,D3D_BLEND_SRC_ALPHA,D3D_BLEND_ONE);

			float4	solid_color_specular (0.f, 0.f, 0.f, 0.f);

				if (configuration.use_diffuse_texture)
					compiler.set_texture("t_base", pcstr(custom_config["texture_diffuse"]), 0, false, u32(-1));

			solid_color_specular = float4(custom_config["constant_diffuse"]);

				if( configuration.use_normal_texture)
					compiler.set_texture("t_normal", pcstr(custom_config["texture_normal"]), 0, false, u32(-1));

				if (configuration.use_organic_scattering_amount_mask && custom_config.value_exists("texture_scattering_amount"))
					compiler.set_texture("t_sss_amount", pcstr(custom_config["texture_scattering_amount"]), 0, false, u32(-1));

				if (configuration.use_organic_back_illumination_texture && custom_config.value_exists("texture_back_illumination"))
					compiler.set_texture("t_back_color", pcstr(custom_config["texture_back_illumination"]), 0, false, u32(-1));

				if (configuration.use_organic_subdermal_texture && custom_config.value_exists("texture_subdermal"))
					compiler.set_texture("t_subdermal", pcstr(custom_config["texture_subdermal"]), 0, false, u32(-1));

				if (configuration.use_ao_texture && custom_config.value_exists("texture_ao"))
					compiler.set_texture("t_ao", pcstr(custom_config["texture_ao"]), 0, false, u32(-1));

				if( configuration.use_specular_intensity_texture)
					compiler.set_texture("t_specular_intensity", pcstr(custom_config["texture_specular_intensity"]), 0, false, u32(-1));

			if (custom_config.value_exists("constant_specular_color") && custom_config.value_exists("constant_specular_color_multiplier"))
				compiler.set_constant( "specular_color_parameter", float4(custom_config["constant_specular_color"]).xyz() * float3(custom_config["constant_specular_color_multiplier"]));

			if (custom_config.value_exists("subdermal_weight") && custom_config.value_exists("subdermal_weight"))
				compiler.set_constant( "subdermal_weight", float(custom_config["subdermal_weight"]));

			if (custom_config.value_exists("scattering_color_multiplier") && custom_config.value_exists("scattering_color_multiplier"))
				compiler.set_constant( "scattering_color_multiplier", float(custom_config["scattering_color_multiplier"]));
			// 3 target lines are likely retail-compiled-out source.
			compiler.set_constant( "solid_color_specular", solid_color_specular);

				compiler.set_texture("t_skin_scattering", "$user$skin_scattering", 0, false, u32(-1));
				compiler.set_texture("t_skin_scattering_blurred_0", "$user$skin_scattering_blurred0", 0, false, u32(-1));
				compiler.set_texture("t_skin_scattering_blurred_1", "$user$skin_scattering_blurred1", 0, false, u32(-1));
				compiler.set_texture("t_skin_scattering_blurred_2", "$user$skin_scattering_blurred2", 0, false, u32(-1));
				compiler.set_texture("t_skin_scattering_blurred_3", "$user$skin_scattering_blurred3", 0, false, u32(-1));
				compiler.set_texture("t_skin_scattering_blurred_4", "$user$skin_scattering_blurred4", 0, false, u32(-1));

		compile_end(compiler);
	}

}

} // namespace render
} // namespace vostok
