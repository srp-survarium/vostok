#include "pch.h"
#include "effect_temporal_antialiasing.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_temporal_antialiasing::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{
	VOSTOK_UNREFERENCED_PARAMETER( custom_config );




	compiler.begin_technique( );
	compiler.begin_pass( "temporal_antialiasing", NULL, "temporal_antialiasing", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_texture( "t_current_frame_color", r2_rt_albedo, 0, false, u32( -1 ) );
	compiler.set_texture( "t_previous_frame_color", "$user$previous_present", 0, false, u32( -1 ) );
	compiler.set_texture( "t_object_motion_vectors", "$user$object_motion_vectors", 0, false, u32( -1 ) );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
