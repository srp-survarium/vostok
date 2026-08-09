#include "pch.h"
#include "effect_downsample_reflective_shadow_map.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_downsample_reflective_shadow_map::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{

	VOSTOK_UNREFERENCED_PARAMETER( config );
	// 3 target lines are likely retail-compiled-out source.
	compiler.begin_technique( );
	compiler.begin_pass( "lpv_downsample_reflective_shadow_map", NULL, "lpv_downsample_reflective_shadow_map", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	// 3 target lines are likely retail-compiled-out source.
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
