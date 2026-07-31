#include "pch.h"
#include "effect_post_process_mlaa.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_post_process_mlaa::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	// FUNCTION BODY[0x7bc130]
	VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

	compiler.begin_technique();
		compiler.begin_pass("post_process_mlaa", NULL, "mlaa_color_edge_detection", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_texture("t_frame_color", r2_rt_generic1, 0, false, 0);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("post_process_mlaa", NULL, "mlaa_blending_weight_calculation", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_texture("t_edges", r2_rt_mlaa_edges, 0, false, 0);
			compiler.set_texture("t_area", "engine/mlaa_area_map_129", 0, true, 0);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("post_process_mlaa", NULL, "mlaa_neighborhood_blending", shader_configuration(), NULL);
			compiler.set_depth(false, false);
			compiler.set_texture("t_edges", r2_rt_mlaa_edges, 0, false, 0);
			compiler.set_texture("t_blend", r2_rt_mlaa_blended_weights, 0, false, 0);
			compiler.set_texture("t_frame_color", r2_rt_generic1, 0, false, 0);
			compiler.set_texture("t_area", "engine/mlaa_area_map_129", 0, true, 0);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
