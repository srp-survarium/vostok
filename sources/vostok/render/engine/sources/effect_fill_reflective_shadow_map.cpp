#include "pch.h"
#include "effect_fill_reflective_shadow_map.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_fill_reflective_shadow_map::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{

	shader_configuration configuration;


	compile_begin( "vertex_base_lpv", "fill_reflective_shadow_map", compiler, &configuration, config );
	compile_end( compiler );

	compile_begin( "vertex_base", "fill_reflective_shadow_map_position", compiler, &configuration, config );
	compile_end( compiler );

	compile_begin( "vertex_base", "fill_view_space_depth", compiler, &configuration, config );
	compiler.set_depth( true, true );

	compile_end( compiler );
}

} // namespace render
} // namespace vostok
