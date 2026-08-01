#include "pch.h"
#include "register_samplers.h"

#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/dx11/sampler_state_descriptor.h>

namespace vostok {
namespace render {

// claude@NOTE: target statements 23-40 are a jump table over options::ref( )+0xb8
// producing the anisotropy level 0/2/4/8/16, which then selects
// D3D_FILTER_ANISOTROPIC vs D3D_FILTER_MIN_MAG_MIP_LINEAR for "s_anisotropic"
// (asm at 0x37..0x7d, then neg/sbb/and 40h/add 15h). The options member behind
// +0xb8 is not identified yet, so the switch is not reconstructed here; the
// sampler set and its order below are read off the target's register_sampler
// call sites.
void register_samplers( )
{
	resource_manager::ref( ).unregister_all_samplers( );

	sampler_state_descriptor sampler_sim_anisotropic;
	sampler_sim_anisotropic.set					( D3D_FILTER_ANISOTROPIC, D3D_TEXTURE_ADDRESS_WRAP );
	sampler_sim_anisotropic.set_max_anisotropy	( 16 );
	sampler_sim_anisotropic.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_anisotropic", resource_manager::ref( ).create_sampler_state( sampler_sim_anisotropic ) );

	sampler_state_descriptor sampler_sim;
	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_WRAP );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_base", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_BORDER );
	resource_manager::ref( ).register_sampler( "s_base_hud", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_WRAP );
	resource_manager::ref( ).register_sampler( "s_detail", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_POINT, D3D_TEXTURE_ADDRESS_BORDER );
	resource_manager::ref( ).register_sampler( "s_position", resource_manager::ref( ).create_sampler_state( sampler_sim ) );
	resource_manager::ref( ).register_sampler( "s_diffuse", resource_manager::ref( ).create_sampler_state( sampler_sim ) );
	resource_manager::ref( ).register_sampler( "s_accumulator", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_POINT, D3D_TEXTURE_ADDRESS_WRAP );
	resource_manager::ref( ).register_sampler( "s_material", resource_manager::ref( ).create_sampler_state( sampler_sim ) );
	resource_manager::ref( ).register_sampler( "s_normal", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_CLAMP );
	resource_manager::ref( ).register_sampler( "s_material1", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_POINT, D3D_TEXTURE_ADDRESS_CLAMP );
	resource_manager::ref( ).register_sampler( "s_nofilter", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	resource_manager::ref( ).register_sampler( "s_rtlinear", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_CLAMP );
	resource_manager::ref( ).register_sampler( "s_linear", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_BORDER );
	sampler_sim.set_border_color	( float4( 1.f, 1.f, 1.f, 0.f ) );
	resource_manager::ref( ).register_sampler( "s_border", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_POINT, D3D_TEXTURE_ADDRESS_CLAMP );
	resource_manager::ref( ).register_sampler( "s_prev_z", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, D3D_TEXTURE_ADDRESS_BORDER );
	sampler_sim.set_comparison_function( D3D_COMPARISON_LESS_EQUAL );
	resource_manager::ref( ).register_sampler( "s_shmap", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set_address_mode	( D3D_TEXTURE_ADDRESS_CLAMP, D3D_TEXTURE_ADDRESS_CLAMP, D3D_TEXTURE_ADDRESS_CLAMP );
	resource_manager::ref( ).register_sampler( "s_shmap_local", resource_manager::ref( ).create_sampler_state( sampler_sim ) );
}

} // namespace render
} // namespace vostok
