#include "pch.h"
#include "effect_fstage_default_materials.h"
#include "blend_mode.h"
#include "shared_names.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_fstage_default_materials::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	// FUNCTION BODY[0x7bb070]
	shader_configuration configuration;



	configuration.use_emissive				= bool(config["use_temissive"]) ? 2 : 1;
	configuration.use_transparency_texture  = bool(config["use_ttransparency"]);
	configuration.use_soft_edges = config.value_exists("use_soft_edges") ? bool(config["use_soft_edges"]) : false;

	compile_begin("vertex_base", "forward_base", compiler, &configuration, config);






		compiler.set_texture("t_position", "$user$position", 0, false, u32(-1));
		compiler.set_texture("t_particle_lighting", "$user$particle_lighting", 0, false, u32(-1));

		float4	solid_color_specular (0.f, 0.f, 0.f, 0.f);
		float   solid_transparency   = 1.0f;


		if( configuration.use_emissive==2)
		{
			compiler.set_texture("t_base", pcstr(config["texture_emissive"]), 0, false, u32(-1));
		}
		solid_color_specular = float4(
			float4(config["constant_emissive"]).xyz() * float(config["constant_emissive_multiplier"]),
			0.0f);
		if (configuration.use_transparency_texture)
		{
			compiler.set_texture("t_transparency", pcstr(config["texture_transparency"]), 0, false, u32(-1));
		}
		solid_transparency = config.value_exists("constant_transparency") ? float(config["constant_transparency"]) : 1.0f;
		compiler.set_constant("solid_transparency",   solid_transparency);

		compiler.set_constant("solid_color_specular", solid_color_specular);
		compiler.set_constant("constant_tile_uv", config.value_exists("constant_tile_u") && config.value_exists("constant_tile_v") ?
				float2(float(config["constant_tile_u"]), float(config["constant_tile_v"])) :
				float2(1.0f, 1.0f));










		compiler.set_cull_mode(D3D_CULL_NONE);

	compile_end(compiler);
}

} // namespace render
} // namespace vostok
