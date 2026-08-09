#include "pch.h"
#include "effect_fstage_volume_cone_base_materials.h"
#include "shared_names.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_fstage_volume_cone_base_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	// FUNCTION BODY[0x7ba2c0]
	shader_configuration configuration;


	configuration.use_diffuse_texture	    = bool(config["use_tdiffuse"]);
	configuration.use_transparency_texture  = bool(config["use_ttransparency"]);

	compile_begin("vertex_base", "forward_volume_cone_base", compiler, &configuration, config);

		float   solid_transparency   = 1.0f;

		compiler.set_cull_mode(D3D_CULL_NONE);

		float4 volume_color = float4(config["constant_volume_color"]);
		float multiplier	= float (config["constant_volume_color_multiplier"]);

		compiler.set_constant("mode_direction_and_uv_tile", float4(float3(config["move_direction"]), float(config["uv_tile"])));

		compiler.set_constant("volume_color", float4(volume_color.xyz()*multiplier, volume_color.w));
		compiler.set_texture("t_position", r2_rt_p, 0, false, u32(-1));

		if( configuration.use_diffuse_texture)
			compiler.set_texture("t_diffuse", pcstr(config["texture_diffuse"]), 0, false, u32(-1));

		compiler.set_texture("t_spot_falloff", "fx/spot_falloff", 0, false, u32(-1));
		compiler.set_texture("t_sphere_falloff", "fx/sphere_falloff", 0, false, u32(-1));

		if (configuration.use_transparency_texture)
			compiler.set_texture("t_transparency", pcstr(config["texture_transparency"]), 0, false, u32(-1));

			solid_transparency = config.value_exists("constant_transparency") ? float(config["constant_transparency"]) : 1.0f;

		compiler.set_constant("attenuation_scale", float(config["attenuation_scale"]));

		compiler.set_constant("solid_transparency",   solid_transparency);

	compile_end(compiler);
}

} // namespace render
} // namespace vostok
