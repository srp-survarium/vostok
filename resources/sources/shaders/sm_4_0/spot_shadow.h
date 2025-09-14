////////////////////////////////////////////////////////////////////////////
//	Created		: 07.04.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_SPOT_SHADOW_H_INCLUDED
#define SHADERS_SM_4_0_SPOT_SHADOW_H_INCLUDED

#include "common_defines.h"

Texture2D					shadowmap_texture;
SamplerComparisonState		s_shmap;

float4x4					view_to_light_matrix;
float						shadow_z_bias;
float						shadow_transparency;

#define USE_HW_SHADOWS 1

float shadow_map_size;// = GLOBAL_SPOT_SHADOWMAP_SIZE;

float4 light_to_texture_space(float4 position, float z_bias)
{
	float4 result 	= position;
	result.x 		= (position.x * 0.5f + 0.5f);// * shadow_map_scaler;
	result.y 		= (-position.y * 0.5f + 0.5f);// * shadow_map_scaler;
	result.z 		-= z_bias;
	return 			result;
}

static const float2 kernel_offset_4[4] = 
{
	float2( -1.0f, -1.0f),
	float2(  1.0f, -1.0f),
	float2( -1.0f,  1.0f),
	float2(  1.0f,  1.0f)
};

static const float2 kernel_offset_8[8] = 
{
	float2( 1.0f,  1.0f),
	float2( 1.0f,  0.0f),
	float2( 1.0f, -1.0f),
	float2( 0.0f,  1.0f),
	float2( 0.0f, -1.0f), 
	float2(-1.0f,  1.0f),
	float2(-1.0f,  0.0f),
	float2(-1.0f, -1.0f)
};

static const float2 poisson_disk[16] = { 
    float2( -0.94201624, -0.39906216 ), 
	float2( 0.94558609, -0.76890725 ), 
	float2( -0.094184101, -0.92938870 ), 
	float2( 0.34495938, 0.29387760 ), 
	float2( -0.91588581, 0.45771432 ), 
	float2( -0.81544232, -0.87912464 ), 
	float2( -0.38277543, 0.27676845 ), 
	float2( 0.97484398, 0.75648379 ), 
	float2( 0.44323325, -0.97511554 ), 
	float2( 0.53742981, -0.47373420 ), 
	float2( -0.26496911, -0.41893023 ), 
	float2( 0.79197514, 0.19090188 ), 
	float2( -0.24188840, 0.99706507 ), 
	float2( -0.81409955, 0.91437590 ), 
	float2( 0.19984126, 0.78641367 ), 
	float2( 0.14383161, -0.14100790 ),
};

float4 sample_2d_shadow_map(in float2 shadow_tc, in float depth, in float2 spot_frustum_correction)
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

float4 sample_hw_2d_shadow_map(in float2 shadow_tc, in float depth, in float2 spot_frustum_correction)
{
	float4 shadow_result = float4(0.0f, 0.0f, 0.0f, 0.0f);

#if 0	
	UNROLL
	for (int i=0; i<4; i++)
	{
		float2 tc 		= shadow_tc.xy + 2.0*kernel_offset_4[i] / shadow_map_size;
		shadow_result 	+= shadowmap_texture.SampleCmpLevelZero(s_shmap, tc.xy, depth).rrrr;
	}
	shadow_result *= float4(0.25f, 0.25f, 0.25f, 0.25f);
#else
	UNROLL
	for (int i = 0; i < 16; i++)
	{
		float2  tc 		= shadow_tc.xy + poisson_disk[i] / shadow_map_size;
		shadow_result 	+= shadowmap_texture.SampleCmpLevelZero(s_shmap, tc.xy, depth).rrrr;
	}
	shadow_result /= 16.0f;
/*
	UNROLL
	for (int i=0; i<8; i++)
	{
		float2 tc 		= shadow_tc.xy + kernel_offset_8[i] / shadow_map_size;
		//tc 				= clamp(tc, 0.0f, shadow_map_scaler - 0.0f / shadow_size);
		shadow_result 	+= shadowmap_texture.SampleCmpLevelZero(s_shmap, tc.xy, depth).rrrr;
	}
	shadow_result *= float4(0.125f, 0.125f, 0.125f, 0.125f);
	*/
#endif
	return shadow_result;
}

float4 get_spotlight_shadow_factor(in float3 view_space_position, in float3x4 view_to_world_matrix)
{
#if CONFIG_SHADOWED_LIGHT
	
	//float4(view_space_position, 1.0f)
	float4 world_space_position = float4(mul(view_to_world_matrix, float4(view_space_position, 1.0f)), 1.0f);
	float4 projection_position	= mul(view_to_light_matrix, world_space_position);
	float4 shadow_tc			= light_to_texture_space(projection_position / projection_position.w, shadow_z_bias);
	
#if USE_HW_SHADOWS
	return saturate( sample_hw_2d_shadow_map(shadow_tc, shadow_tc.z, 0.0f) + shadow_transparency );
#else
	return saturate( sample_2d_shadow_map(shadow_tc, shadow_tc.z, 0.0f) + shadow_transparency );
#endif // USE_HW_SHADOWS
	
#else
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
#endif // #if CONFIG_SHADOWED_LIGHT
}

float4 get_pointlight_shadow_factor(in float3 view_space_position, in float3x4 view_to_world_matrix)
{
#if CONFIG_SHADOWED_LIGHT
	
	float4 world_space_position = float4(mul(view_to_world_matrix, float4(view_space_position, 1.0f)), 1.0f);
	float4 projection_position	= mul(view_to_light_matrix, world_space_position);
	float4 shadow_tc			= light_to_texture_space(projection_position / projection_position.w, shadow_z_bias);
	
#if USE_HW_SHADOWS
	return saturate( sample_hw_2d_shadow_map(shadow_tc, shadow_tc.z, 0.0f / shadow_map_size) + shadow_transparency );
#else
	return saturate( sample_2d_shadow_map(shadow_tc, shadow_tc.z, 0.0f / shadow_map_size) + shadow_transparency );
#endif // USE_HW_SHADOWS
	
#else
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
#endif // #if CONFIG_SHADOWED_LIGHT
}

#endif // #ifndef SHADERS_SM_4_0_SPOT_SHADOW_H_INCLUDED






