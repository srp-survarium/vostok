#include "pch.h"
#include "effect_shadow_map.h"
#include <vostok/render/core/dx11/effect_compiler.h>

namespace vostok {
namespace render {

void effect_shadow_map::compile(
	effect_compiler&			compiler,
	custom_config_value const&
)
{
	// 4 target lines are likely retail-compiled-out source.
	shader_configuration configuration;


	compiler.begin_technique( );
	compiler.begin_pass( "vertex_base_shadow", NULL, "depth_accumulate", configuration, NULL );
	compiler.set_constant( "wind_scale", 1.0f );
	compiler.set_depth( true, true );
	compiler.color_write_enable( D3D_COLOR_WRITE_ENABLE_NONE );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.end_pass( );
	compiler.end_technique( );


	compiler.begin_technique( );
	compiler.begin_pass( "vertex_base_shadow_batched", NULL, "depth_accumulate_batched", configuration, NULL );
	compiler.set_constant( "wind_scale", 1.0f );
	compiler.set_depth( true, true );
	compiler.color_write_enable( D3D_COLOR_WRITE_ENABLE_NONE );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
