#include "pch.h"
#include "effect_gather_luminance_histogram.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"
#include <vostok/render/core/custom_config_value.h>

namespace vostok {
namespace render {

void effect_gather_luminance_histogram::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	VOSTOK_UNREFERENCED_PARAMETER( custom_config );
	compiler.begin_technique( );
	compiler.begin_pass( "gather_luminance", NULL, "gather_luminance_in_range", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "gather_luminance", NULL, "gather_luminance_count", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "gather_luminance2", NULL, "gather_luminance_histogram", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_texture( "t_frame_luminance", r2_rt_frame_luminance, 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "gather_luminance2", NULL, "gather_luminance_histogram_downsample_scene_color", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_texture( "t_frame_color", r2_rt_generic0, 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
