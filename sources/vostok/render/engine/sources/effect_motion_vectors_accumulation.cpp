#include "pch.h"
#include "effect_motion_vectors_accumulation.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_motion_vectors_accumulation::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	// FUNCTION BODY[0x7bc360]
	// claude@NOTE: ported from legacy make_vcm_effect (lower-confidence rename per
	// temp/render_legacy/triage_log.md; make_vcm shaders still ship); begin_pass adapted
	// from the legacy 2-arg generation
	VOSTOK_UNREFERENCED_PARAMETER	( config);
	compiler.begin_technique();
		compiler.begin_pass("make_vcm", NULL, "make_vcm", shader_configuration(), NULL);
			compiler.set_depth(true, false);
			compiler.set_cull_mode(D3D_CULL_NONE);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
