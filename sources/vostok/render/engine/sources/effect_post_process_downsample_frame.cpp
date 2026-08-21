#include "pch.h"
#include "effect_post_process_downsample_frame.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_post_process_downsample_frame::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	VOSTOK_UNREFERENCED_PARAMETER( custom_config );
	compiler.begin_technique( );
	compiler.begin_pass( "post_process_downsample_frame", NULL, "post_process_downsample_frame", shader_configuration( ), NULL );
	compiler.set_texture( "t_frame_color", r2_rt_generic0, 0, false, u32( -1 ) );
	compiler.set_depth( false, false );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "post_process_downsample_frame", NULL, "post_process_downsample_frame_blur", shader_configuration( ), NULL );
	compiler.set_texture( "t_frame_color", "$user$final_frame_downsampled", 0, false, u32( -1 ) );
	compiler.set_depth( false, false );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "post_process_downsample_frame", NULL, "post_process_downsample_frame_blur", shader_configuration( ), NULL );
	compiler.set_texture( "t_frame_color", "$user$final_frame_downsampled_temp", 0, false, u32( -1 ) );
	compiler.set_depth( false, false );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
