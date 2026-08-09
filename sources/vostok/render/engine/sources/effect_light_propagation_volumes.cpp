#include "pch.h"
#include "effect_light_propagation_volumes.h"
#include <vostok/render/core/dx11/effect_compiler.h>
#include "shared_names.h"

namespace vostok {
namespace render {

void effect_light_propagation_volumes::compile(
	effect_compiler&			compiler,
	custom_config_value const&	config
)
{
	VOSTOK_UNREFERENCED_PARAMETER( config );





	compiler.begin_technique( );
	compiler.begin_pass( "lpv_inject_lighting", "lpv_inject_lighting", "lpv_inject_lighting", shader_configuration( ), NULL );
	compiler.set_depth( false, false );


	compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_MAX, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_MAX );



	compiler.end_pass( );
	compiler.end_technique( );


	compiler.begin_technique( );
	compiler.begin_pass( "lpv_inject_occluders", "lpv_inject_occluders", "lpv_inject_occluders", shader_configuration( ), NULL );
	compiler.set_depth( false, false );




	compiler.set_alpha_blend( false );



	compiler.end_pass( );
	compiler.end_technique( );


	compiler.begin_technique( );
	compiler.begin_pass( "lpv_inject_occluders_camera_view", "lpv_inject_occluders_camera_view", "lpv_inject_occluders_camera_view", shader_configuration( ), NULL );
	compiler.set_depth( false, false );

	compiler.set_alpha_blend( false );


	compiler.end_pass( );
	compiler.end_technique( );


	compiler.begin_technique( );
	compiler.begin_pass( "lpv_inject_occluder_geometry", "lpv_inject_occluder_geometry", "lpv_inject_occluders", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_alpha_blend( false );


	compiler.end_pass( );
	compiler.end_technique( );


	compiler.begin_technique( );
	compiler.begin_pass( "lpv_propagate_lighting", "lpv_propagate_lighting", "lpv_propagate_lighting", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD );
	compiler.end_pass( );
	compiler.end_technique( );


	compiler.begin_technique( );
	compiler.begin_pass( "lpv_accumulate_propagation", "lpv_accumulate_propagation", "lpv_accumulate_propagation", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_alpha_blend( true, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD, D3D_BLEND_ONE, D3D_BLEND_ONE, D3D_BLEND_OP_ADD );
	compiler.end_pass( );
	compiler.end_technique( );
}

} // namespace render
} // namespace vostok
