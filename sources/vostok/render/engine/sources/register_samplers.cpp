#include "pch.h"
#include "register_samplers.h"

#include <vostok/render/core/options.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/dx11/sampler_state_descriptor.h>

namespace vostok {
namespace render {

void register_samplers( )
{
	resource_manager::ref( ).unregister_all_samplers( );

	u32 max_anisotropy = math::clamp_r( options::ref( ).current.m_max_anisotropic, 0u, 4u );

	switch ( options::ref( ).current.m_max_anisotropic )
	{
		case 0 :
			max_anisotropy			= 0;
			break;
		case 1 :
			max_anisotropy			= 2;
			break;
		case 2 :
			max_anisotropy			= 4;
			break;
		case 3 :
			max_anisotropy			= 8;
			break;
		case 4 :
			max_anisotropy			= 16;
			break;
		default :
			break;
	}

	sampler_state_descriptor sampler_sim_anisotropic;
	sampler_sim_anisotropic.set					( max_anisotropy ? D3D_FILTER_ANISOTROPIC : D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_WRAP );
	sampler_sim_anisotropic.set_max_anisotropy	( max_anisotropy );
	sampler_sim_anisotropic.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_anisotropic", resource_manager::ref( ).create_sampler_state( sampler_sim_anisotropic ) );

	sampler_state_descriptor sampler_sim;
	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_WRAP );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_base", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_CLAMP );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_base_hud", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_WRAP );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_detail", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_POINT, D3D_TEXTURE_ADDRESS_CLAMP );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_position", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	resource_manager::ref( ).register_sampler( "s_diffuse", resource_manager::ref( ).create_sampler_state( sampler_sim ) );
	resource_manager::ref( ).register_sampler( "s_accumulator", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_WRAP );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_material", resource_manager::ref( ).create_sampler_state( sampler_sim ) );
	resource_manager::ref( ).register_sampler( "s_normal", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_CLAMP );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_material1", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_POINT, D3D_TEXTURE_ADDRESS_WRAP );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_nofilter", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D_TEXTURE_ADDRESS_CLAMP );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_rtlinear", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_WRAP );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_linear", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_BORDER );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	resource_manager::ref( ).register_sampler( "s_border", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_MIN_MAG_MIP_POINT, D3D_TEXTURE_ADDRESS_BORDER );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	sampler_sim.set_border_color	( float4( 0.f, 0.f, 0.f, 0.f ) );
	resource_manager::ref( ).register_sampler( "s_prev_z", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	sampler_sim.set					( D3D_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, D3D_TEXTURE_ADDRESS_CLAMP );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	sampler_sim.set_border_color	( float4( 1.f, 1.f, 1.f, 1.f ) );
	sampler_sim.set_comparison_function( D3D_COMPARISON_LESS_EQUAL );
	resource_manager::ref( ).register_sampler( "s_shmap", resource_manager::ref( ).create_sampler_state( sampler_sim ) );
	sampler_sim.set_border_color	( float4( 1.f, 1.f, 1.f, 1.f ) );
	sampler_sim.set					( D3D_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, D3D_TEXTURE_ADDRESS_CLAMP );
	sampler_sim.set_max_anisotropy	( 1 );
	sampler_sim.set_mip				( 0.f, 0.f, FLT_MAX );
	sampler_sim.set_comparison_function( D3D_COMPARISON_LESS_EQUAL );

	resource_manager::ref( ).register_sampler( "s_shmap_local", resource_manager::ref( ).create_sampler_state( sampler_sim ) );

	resource_manager::ref( ).bind_samplers_to_shaders( );
}

} // namespace render
} // namespace vostok
