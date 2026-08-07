#include "pch.h"
#include "effect_translucency.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_translucency::compile(
	effect_compiler&			compiler,
	custom_config_value const&
)
{
	compiler.begin_technique( );
	compiler.begin_pass( "translucency", NULL, "translucency", shader_configuration( ), NULL );

	compiler.set_texture( "t_diffuse", r2_rt_albedo, 0, false, 0 );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, 0 );
	compiler.set_texture( "t_normal", r2_rt_n, 0, false, 0 );

	compiler.set_texture( "t_sun_translucensy_help_data", "$user$sun_translucensy_help_data", 0, false, 0 );

	compiler.set_depth( false, false );
	compiler.set_stencil( true, 0xff, 0x04, 0xff, D3D_COMPARISON_EQUAL );
	compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE );

	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_fill_mode( D3D_FILL_SOLID );

	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
