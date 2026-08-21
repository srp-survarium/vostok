#include "pch.h"
#include "effect_system_colored.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_system_colored::compile(
	effect_compiler&			c,
	custom_config_value const&	config
)
{

	VOSTOK_UNREFERENCED_PARAMETER	( config );

	c.begin_technique( /*solid*/)
		.begin_pass	( "color", NULL, "color", shader_configuration(), NULL)
			.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA)

		.end_pass	()
	.end_technique();

	c.begin_technique( /*stenciled*/)
		.begin_pass	( "color_top", NULL, "color_doted", shader_configuration(), NULL)
			.set_depth	( true, true, D3D_COMPARISON_ALWAYS)

		.end_pass	()
	.end_technique();

	c.begin_technique( /*voided*/)
		.begin_pass	( "color", NULL, "color", shader_configuration(), NULL)
			.color_write_enable( D3D_COLOR_WRITE_ENABLE_NONE)

		.end_pass	()
	.end_technique();

	c.begin_technique( /*cover*/)
		.begin_pass	( "color_cover", NULL, "color_cover", shader_configuration(), NULL)
			.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA)
			.set_texture("t_position", r2_rt_p, 0, false, u32(-1))
			.set_texture("t_random_rotates", "engine/ssao_rotate", 0, true, u32(-1))
		.end_pass	()
	.end_technique();

	c.begin_technique( /*wireframe*/)
		.begin_pass	( "color", NULL, "color_wireframe", shader_configuration(), NULL)
			.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA)
			.set_fill_mode( D3D_FILL_WIREFRAME)
		.end_pass	()
	.end_technique();
}

} // namespace render
} // namespace vostok
