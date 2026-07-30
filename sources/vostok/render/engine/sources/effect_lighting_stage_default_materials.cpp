#include "pch.h"
#include "effect_lighting_stage_default_materials.h"

namespace vostok {
namespace render {

bool effect_lighting_stage_default_materials::should_recompile_when_global_changes(
	vector< fs_new::virtual_path_string > const&
) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ed720]
	return false;
}

void effect_lighting_stage_default_materials::compile(
	effect_compiler&,
	custom_config_value const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x5ed760]
}

} // namespace render
} // namespace vostok
