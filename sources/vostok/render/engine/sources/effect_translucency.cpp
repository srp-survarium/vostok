#include "pch.h"
#include "effect_translucency.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_translucency::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	// FUNCTION BODY[0x7bd780]
	// claude@NOTE: ported from legacy effect_post_process_deferred_transparency
	// (rename proven in temp/render_legacy/triage_log.md)
	VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

	compiler.begin_technique();
		compiler.begin_pass("post_process0", NULL, "post_process_deferred_transparency", shader_configuration(), NULL);
			compiler.set_texture("t_position", r2_rt_p, 0, false, 0);				// set here?
			compiler.set_texture("t_normal", r2_rt_n, 0, false, 0);					// set here?
			compiler.set_depth( true, false);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
