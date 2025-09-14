////////////////////////////////////////////////////////////////////////////
//	Created		: 13.05.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_BRDF_H_INCLUDED
#define SHADERS_SM_4_0_BRDF_H_INCLUDED

#include "light_accumulator.h"
#include "common_functions.h"
#include "debug.h"

static const float	pi								= 3.14159267;
static const float	specular_normalization_factor	= 1.f/(2.f*pi);
Texture2D			t_emissive;

// TODO: remove tangents render target
Texture2D 			t_tangents;

uniform int	lighting_model;

// modified Phong BRDF
light_factors phong_brdf		(
		float3 light_direction,
		float3 view_direction,
		float3 surface_normal,
		float specular_power
	)
{
	light_factors				result;
	
	result.diffuse				= max( 0.f, dot( light_direction, surface_normal ) );
	
	float3 reflected_light_direction	= reflect( -light_direction, surface_normal );
	result.specular				= 
		pow(
			max( dot( reflected_light_direction, view_direction ), 0.f ),
			clamp( specular_power * (1.f - dot( -light_direction, surface_normal )) / 2.f, 0.001f, 255.0f) 
		) * (specular_power + 2.f) * specular_normalization_factor;

	return						result;
}

// pure Blinn-Phong BRDF
light_factors blinn_phong_brdf	(
		float3 light_direction,
		float3 view_direction,
		float3 surface_normal,
		float specular_power
	)
{
	light_factors				result;
	
	result.diffuse				= max( 0.f, dot( light_direction, surface_normal ) );

	float3 halfway_direction	= normalize(light_direction + view_direction);
	result.specular				= 
		pow(
			max( dot( surface_normal, halfway_direction ), 0.f ),
			clamp(specular_power, 0.1f, 255.0f)
		) * (specular_power + 2.f) * specular_normalization_factor;

	return						result;
}

// pure Naty Hoffman BRDF
// http://renderwonk.com/publications/s2010-shading-course/hoffman/s2010_physically_based_shading_hoffman_b.pdf
light_factors hoffman_brdf		(
		float3 light_direction,
		float3 view_direction,
		float3 surface_normal,
		float specular_power,
		float2 texture_coordinates,
		float4 material_parameters
	)
{
	light_factors				result;

	float light_intensity_on_surface	= max( 0.f, dot( light_direction, surface_normal ) );
	result.diffuse				= light_intensity_on_surface;

	//float4 material_parameters	= t_emissive.Sample( s_accumulator, texture_coordinates );
	float f0					= material_parameters.x;

	float3 halfway_direction	= normalize(light_direction + view_direction);
	float view_dot_halfway		= dot( view_direction, halfway_direction );
	float fresnel_factor		= f0 + (1.f - f0)*pow( 1.f - view_dot_halfway, 5.f );
	float specular_level		= 100.f * clamp(specular_power, 0.f, 1.f);
	result.specular				= 
		((specular_level + 2.f) / 8) *
		pow (
			max( dot( surface_normal, halfway_direction ), 0.f ),
			specular_level
		) *
		fresnel_factor *
		light_intensity_on_surface
	;

	return						result;
}



/*
// pure Szirmay-Kalos BRDF
// http://66.147.242.191/~westmand/siggraph/computer-graphics/Anisotropic_BRDF_Model_SIGGRAPH_Computer-Graphics_vol-44_num-1_2010.pdf
light_factors szirmay_kalos_brdf(
		float3 light_direction,
		float3 view_direction,
		float3 surface_normal,
		float2 surface_roughness,
		float2 texture_coordinates,
		float4 material_parameters
	)
{
	light_factors				result;

	// yellow satin
	//float f0										= 0.207;
	//float anisotropic_roughness_surface_factor_x	= 0.129;
	//float anisotropic_roughness_surface_factor_y	= 1.084;
	//float alpha_model_parameter					= 0.197;

	// red velvet
	//float f0										= 0.041;
	//float anisotropic_roughness_surface_factor_x	= 2.487;
	//float anisotropic_roughness_surface_factor_y	= 2.487;
	//float alpha_model_parameter					= 0.000;

	// brushed aluminium
	//float f0										= 0.999;
	//float anisotropic_roughness_surface_factor_x	= 0.035;
	//float anisotropic_roughness_surface_factor_y	= 0.129;
	//float alpha_model_parameter					= 0.005;

	//float4 material_parameters					= t_emissive.Sample( s_accumulator, texture_coordinates );
	float f0										= material_parameters.x;//saturate( 0.5 * pow(material_parameters.x, 2) );
	float alpha_model_parameter					 	= material_parameters.y;
	
	float2 isotropic_roughness_surface_factor		= max( surface_roughness, (0.01f).xx);
	float anisotropic_roughness_surface_factor_x	= isotropic_roughness_surface_factor.x;
	float anisotropic_roughness_surface_factor_y	= isotropic_roughness_surface_factor.y;
	
	// we use Schlicks approximation formula
	// where f0 - is cumulative zero degree material fresnel factor
	float3 halfway_direction	= normalize(light_direction + view_direction);
	float view_dot_halfway		= dot( view_direction, halfway_direction );
	float view_dot_halfway_plus	= max( view_dot_halfway, 0.f );
	float fresnel_factor		= f0 + (1.f - f0)*pow( 1.f - view_dot_halfway_plus, 5.f );
	
	float light_dot_normal		= dot( light_direction, surface_normal );
	float light_dot_normal_plus	= pow( max( light_dot_normal, 0.f ), 1 );

	// Half Lambert
	//result.diffuse			= (light_dot_normal/2 + 0.5) * max(1.f - fresnel_factor, 0);
	result.diffuse				= light_dot_normal_plus;// * ( 1.f - fresnel_factor );

	// compute normalized distribution functions
	// Beckmann distribution function in case of isotropic materials
	float cosine_theta			= dot( surface_normal, halfway_direction );
	float square_cosine_theta	= square( cosine_theta );
	float square_sine_theta		= 1.f - square_cosine_theta;
	float square_tangent_theta	= square_sine_theta/square_cosine_theta;

	float2 packed_tangent		= 2*(material_parameters.ba - 0.5);
	float3 surface_tangent		= 
		normalize(
			surface_normal.x == 0.f ?
			(
				surface_normal.y == 0.f ?
				float3( packed_tangent.xy, -dot( packed_tangent.xy, surface_normal.xy )/surface_normal.z ):
				float3( packed_tangent.x, -dot( packed_tangent.xy, surface_normal.xz )/surface_normal.y, packed_tangent.y )
			):
			float3( -dot( packed_tangent.xy, surface_normal.yz )/surface_normal.x, packed_tangent.xy )
		);
	
	float3 surface_bitangent	= normalize( cross(surface_normal, surface_tangent) );
	float square_cosine_phi		= saturate( square( dot( normalize( surface_tangent*dot( surface_tangent, halfway_direction ) + surface_bitangent*dot( surface_bitangent, halfway_direction ) ), surface_bitangent ) ) );
	float square_sine_phi		= 1.f - square_cosine_phi;

	// float distribution_function	=
		// exp(
			// -square_tangent_theta
			// *(
				// square_cosine_phi / square( anisotropic_roughness_surface_factor_x )
				// +
				// square_sine_phi / square( anisotropic_roughness_surface_factor_y )
			// )
		// );
	float distribution_function	= exp( -square_tangent_theta / square( isotropic_roughness_surface_factor ) );

	float normalized_distribution_function	=
		distribution_function/
		(
			anisotropic_roughness_surface_factor_x * anisotropic_roughness_surface_factor_y * square(square_cosine_theta)
		);

	result.specular					=
		light_dot_normal > 0.f
		?
			fresnel_factor * normalized_distribution_function /
			(
				4.f * view_dot_halfway_plus *
				pow(
					ABS_TO_REMOVE_WARNING(
						light_dot_normal *
						dot(view_direction, surface_normal)
					),
					alpha_model_parameter
				)
			)
		:
			0.f;
	result.specular				= clamp(result.specular, 0.0f, 4.0f);
	return						result;
}
*/
// pure Szirmay-Kalos BRDF
// http://66.147.242.191/~westmand/siggraph/computer-graphics/Anisotropic_BRDF_Model_SIGGRAPH_Computer-Graphics_vol-44_num-1_2010.pdf
light_factors szirmay_kalos_brdf(
		float3 light_direction,
		float3 view_direction,
		float3 surface_normal,
		float2 surface_roughness,
		float2 texture_coordinates,
		float4 material_parameters
	)
{
	light_factors				result;

	// yellow satin
	//float f0										= 0.207;
	//float anisotropic_roughness_surface_factor_x	= 0.129;
	//float anisotropic_roughness_surface_factor_y	= 1.084;
	//float alpha_model_parameter					= 0.197;

	// red velvet
	//float f0										= 0.041;
	//float anisotropic_roughness_surface_factor_x	= 2.487;
	//float anisotropic_roughness_surface_factor_y	= 2.487;
	//float alpha_model_parameter					= 0.000;

	// brushed aluminium
	//float f0										= 0.999;
	//float anisotropic_roughness_surface_factor_x	= 0.035;
	//float anisotropic_roughness_surface_factor_y	= 0.129;
	//float alpha_model_parameter					= 0.005;

	//float4 material_parameters					= t_emissive.Sample( s_accumulator, texture_coordinates );
	float f0										= material_parameters.x;//saturate( 0.5 * pow(material_parameters.x, 2) );
	float alpha_model_parameter					 	= material_parameters.y;
	
	float2 isotropic_roughness_surface_factor		= max(surface_roughness, (0.01f).xx);
	float anisotropic_roughness_surface_factor_x	= isotropic_roughness_surface_factor.x;
	float anisotropic_roughness_surface_factor_y	= isotropic_roughness_surface_factor.y;
	
	// we use Schlicks approximation formula
	// where f0 - is cumulative zero degree material fresnel factor
	float3 halfway_direction	= normalize(light_direction + view_direction);
	float view_dot_halfway		= dot( view_direction, halfway_direction );
	float view_dot_halfway_plus	= max( view_dot_halfway, 0.f );
	float fresnel_factor		= f0 + (1.f - f0)*pow( 1.f - view_dot_halfway_plus, 5.f );

	float light_dot_normal		= dot( light_direction, surface_normal );
//	float light_dot_normal_plus	= pow( max( light_dot_normal, 0.f ), 1 );
	float light_dot_normal_plus	= max( light_dot_normal, 0.f );
	//float light_dot_normal_plus	= light_dot_normal;
	
	// Half Lambert
	//result.diffuse			= (light_dot_normal/2 + 0.5) * max(1.f - fresnel_factor, 0);
	result.diffuse				= abs(light_dot_normal_plus);// * ( 1.f - fresnel_factor );
	
	// compute normalized distribution functions
	// Beckmann distribution function in case of isotropic materials
	float cosine_theta			= dot( surface_normal, halfway_direction );
	float square_cosine_theta	= square( cosine_theta );
	float square_sine_theta		= 1.f - square_cosine_theta;
	float square_tangent_theta	= square_sine_theta/square_cosine_theta;
	
	float3 surface_tangent		= t_tangents.Sample(s_accumulator, texture_coordinates).xyz * 2.0f - 1.0f;
	float3 half_projection		= normalize(halfway_direction - surface_normal * dot(surface_normal, halfway_direction));
	float square_cosine_phi		= saturate(square( dot(half_projection, surface_tangent) ));
	float square_sine_phi		= 1.0f - square_cosine_phi;
	
	float distribution_function	=
		 exp(
			 -square_tangent_theta
			 *(
				 square_cosine_phi / square( anisotropic_roughness_surface_factor_x )
				 +
				 square_sine_phi / square( anisotropic_roughness_surface_factor_y )
			 )
		 );
	//float distribution_function	= exp( -square_tangent_theta / square( isotropic_roughness_surface_factor ) );
	
	float normalized_distribution_function	=
		distribution_function /
		(
			pi * anisotropic_roughness_surface_factor_x * anisotropic_roughness_surface_factor_y * square(square_cosine_theta)
		);
	
	result.specular					=
		light_dot_normal > 0.f
		?
			light_dot_normal_plus * fresnel_factor * normalized_distribution_function /
			(
				4.f * view_dot_halfway_plus *
				pow(
					ABS_TO_REMOVE_WARNING(
						light_dot_normal *
						dot(view_direction, surface_normal)
					),
					alpha_model_parameter
				)
			)
		:
			0.f;
	result.specular				= clamp(result.specular, 0.0f, 4.0f);
	
	return						result;
}
light_factors brdf				(
		float3 light_direction,
		float3 view_direction,
		float3 surface_normal,
		float2 surface_roughness,
		float2 texture_coordinates,
		float4 material_parameters
	)
{
	light_factors result;

	//specular_power				= max( specular_power, .02f );
	//specular_power				= clamp( specular_power, 0.01f, 254.0f );
	
//	switch ( lighting_model ) {
//		case 0 : {
//			result				= phong_brdf( light_direction, view_direction, surface_normal, specular_power );
//			break;
//		}
//		case 1 : {
//			result				= blinn_phong_brdf( light_direction, view_direction, surface_normal, specular_power );
//			break;
//		}
//		case 2 : {
//			result				= hoffman_brdf( light_direction, view_direction, surface_normal, specular_power, texture_coordinates, material_parameters );
//			break;
//		}
//		case 3 : {
//    			result				= szirmay_kalos_brdf( light_direction, view_direction, surface_normal, specular_power, texture_coordinates, material_parameters );
//	    		break;
//		}
//	}

//	result					= hoffman_brdf( light_direction, view_direction, surface_normal, surface_roughness, texture_coordinates, material_parameters );
	result					= szirmay_kalos_brdf( light_direction, view_direction, surface_normal, surface_roughness, texture_coordinates, material_parameters );

	//result.diffuse			= abs( result.diffuse );
	//result.specular			= abs( result.specular );

	return						result;
}

#endif // #ifndef SHADERS_SM_4_0_BRDF_H_INCLUDED