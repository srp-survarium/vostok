#include "pch.h"
#include "effect_olta.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_olta::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	VOSTOK_UNREFERENCED_PARAMETER( custom_config );
	compiler.begin_technique( );
	compiler.begin_pass( "olta", NULL, "olta_blend", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_texture( "t_frame_color", r2_rt_present, 0, false, u32( -1 ) );
	compiler.set_texture( "t_olta", "$user$one_layer_transparency_alpha", 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "olta", NULL, "olta_blur", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_texture( "t_frame_color", r2_rt_albedo, 0, false, u32( -1 ) );
	compiler.set_texture( "t_olta", "$user$one_layer_transparency_alpha", 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
