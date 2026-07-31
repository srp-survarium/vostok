#include "pch.h"
#include "effect_gather_luminance.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_gather_luminance::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	// FUNCTION BODY[0x7b4f90]
	VOSTOK_UNREFERENCED_PARAMETERS(custom_config);

	compiler.begin_technique();
		compiler.begin_pass("gather_luminance", NULL, "gather_log_luminance", shader_configuration(), NULL);
			compiler.set_depth( false, false);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("gather_luminance", NULL, "gather_luminance", shader_configuration(), NULL);
			compiler.set_depth( false, false);
		compiler.end_pass();
	compiler.end_technique();

	compiler.begin_technique();
		compiler.begin_pass("gather_luminance", NULL, "gather_exp_luminance", shader_configuration(), NULL);
			compiler.set_depth( false, false);
		compiler.end_pass();
	compiler.end_technique();
}

} // namespace render
} // namespace vostok
