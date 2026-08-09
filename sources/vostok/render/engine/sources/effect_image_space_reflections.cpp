#include "pch.h"
#include "effect_image_space_reflections.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_image_space_reflections::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	VOSTOK_UNREFERENCED_PARAMETER( custom_config );




	compiler.begin_technique( );
	compiler.begin_pass( "image_space_reflections", NULL, "image_space_reflections", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_texture( "t_final_frame_donwsampled", "$user$final_frame_downsampled", 0, false, u32( -1 ) );
	compiler.set_texture( "t_normal", r2_rt_n, 0, false, u32( -1 ) );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, u32( -1 ) );
	compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
