#include "pch.h"
#include "effect_ssao_accumulation.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_ssao_accumulation::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{

	VOSTOK_UNREFERENCED_PARAMETER( custom_config );




	compiler.begin_technique( );
	compiler.begin_pass( "post_process0", NULL, "ssao_accumulation", shader_configuration( ), NULL );
	compiler.set_depth( false, false );

	compiler.set_alpha_blend( false );
	compiler.set_texture( "t_random_rotates", "engine/ssao_rotate", 0, false, u32( -1 ) );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, u32( -1 ) );
	compiler.set_texture( "t_normal", r2_rt_n, 0, false, u32( -1 ) );
	compiler.set_texture( "t_normal_and_depth", "$user$gbuffer_position_normal_downsampled_x2", 0, false, u32( -1 ) );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "post_process0", NULL, "ssao_accumulation", shader_configuration( ), NULL );
	compiler.set_depth( false, false );

	compiler.set_alpha_blend( false );
	compiler.set_texture( "t_random_rotates", "engine/ssao_rotate", 0, false, u32( -1 ) );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, u32( -1 ) );
	compiler.set_texture( "t_normal", r2_rt_n, 0, false, u32( -1 ) );
	compiler.set_texture( "t_normal_and_depth", "$user$gbuffer_position_normal_downsampled_x2", 0, false, u32( -1 ) );
	compiler.set_stencil( false );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
