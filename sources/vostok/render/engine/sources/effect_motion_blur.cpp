#include "pch.h"
#include "effect_motion_blur.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_motion_blur::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	VOSTOK_UNREFERENCED_PARAMETER( custom_config );
	compiler.begin_technique( );
	compiler.begin_pass( "motion_blur", NULL, "motion_blur", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_texture( "t_frame_color", r2_rt_present, 0, false, u32( -1 ) );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, u32( -1 ) );
	compiler.set_texture( "t_object_motion_vectors", "$user$object_motion_vectors", 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "motion_blur", NULL, "motion_blur_apply", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_texture( "t_motion_blur_result", r2_rt_present, 0, false, u32( -1 ) );
	compiler.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
