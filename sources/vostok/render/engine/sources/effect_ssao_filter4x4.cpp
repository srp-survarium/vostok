#include "pch.h"
#include "effect_ssao_filter4x4.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_ssao_filter4x4::compile(
	effect_compiler&			compiler,
	custom_config_value const&	custom_config
)
{

	VOSTOK_UNREFERENCED_PARAMETER( custom_config );
	compiler.begin_technique( );
	compiler.begin_pass( "post_process_fxaa", NULL, "ssao_filter4x4", shader_configuration( ), NULL );
	compiler.set_depth( true, false );
	compiler.set_alpha_blend( false );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, u32( -1 ) );
	compiler.set_texture( "t_ssao_accumulator", r2_rt_ssao_accumulator, 0, false, u32( -1 ) );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "post_process_fxaa", NULL, "ssao_filter4x4_1", shader_configuration( ), NULL );
	compiler.set_depth( true, false );
	compiler.set_alpha_blend( false );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, u32( -1 ) );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "post_process_fxaa", NULL, "temporal_mask", shader_configuration( ), NULL );
	compiler.set_depth( true, false );
	compiler.set_alpha_blend( false );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, u32( -1 ) );
	compiler.set_texture( "t_ssao_prev_z", "$user$ssao_prev_accumulator_z", 0, false, u32( -1 ) );

	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "post_process_fxaa", NULL, "ssao_filter_upsample", shader_configuration( ), NULL );
	compiler.set_depth( true, false );
	compiler.set_alpha_blend( false );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, u32( -1 ) );
	compiler.set_texture( "t_ssao_accumulator", r2_rt_ssao_accumulator, 0, false, u32( -1 ) );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );

	compiler.begin_technique( );
	compiler.begin_pass( "post_process_fxaa", NULL, "ssao_filter_upsample_temporal", shader_configuration( ), NULL );
	compiler.set_depth( true, false );
	compiler.set_alpha_blend( false );
	compiler.set_texture( "t_position", r2_rt_p, 0, false, u32( -1 ) );
	compiler.set_texture( "t_ssao_accumulator", r2_rt_ssao_accumulator, 0, false, u32( -1 ) );
	compiler.set_texture( "t_ssao_prev_result", "$user$ssao_prev_accumulator_full", 0, false, u32( -1 ) );
	compiler.set_texture( "t_ssao_prev_z", "$user$ssao_prev_accumulator_z", 0, false, u32( -1 ) );
	compiler.set_texture( "t_ssao_temporal_mask", "$user$ssao_temporal_mask", 0, false, u32( -1 ) );
	compiler.set_stencil( true, 0, 0xff, 0, D3D_COMPARISON_NOT_EQUAL );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
