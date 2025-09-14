////////////////////////////////////////////////////////////////////////////
//	Created		: 21.09.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_SHADERS_SM_4_0_SPHERICAL_HARMONICS_H_INCLUDED
#define XRAY_SHADERS_SM_4_0_SPHERICAL_HARMONICS_H_INCLUDED

static const float sh_pi = 3.14159265f;

float4 sh_c_normalize(in float4 res)
{
	// extract direction
	float l 		= dot(res.gba, res.gba);
	res.gba 		/= max(0.05f, sqrt(l));
	res.r 			= 1.0;
	return res;
}

float low_order_sh_dot_abs(float4 sh_0, float4 sh_1)
{
   return sh_0.x * sh_1.x + abs(dot(sh_0.yzw, sh_1.yzw));
}

float4 low_order_sh_project(float3 direction)
{
	float4 sh_coefficients;
	
	sh_coefficients.x =  0.282094792f;
	sh_coefficients.y = -0.4886025119f * direction.y;
	sh_coefficients.z =  0.4886025119f * direction.z;
	sh_coefficients.w = -0.4886025119f * direction.x;
	
	return sh_coefficients;
}


float4 low_order_sh_project_clampled_cosine_lobe(float3 direction)
{
	float4 result;
	
	result.x = sh_pi * 0.282094792f;
	result.y = ((2.0f * sh_pi) / 3.0f) * -0.4886025119f * direction.y;
	result.z = ((2.0f * sh_pi) / 3.0f) *  0.4886025119f * direction.z;
	result.w = ((2.0f * sh_pi) / 3.0f) * -0.4886025119f * direction.x;
	
	return result;
}


// http://www.crytek.com/cryengine/cryengine3/presentations/light-propagation-volumes-in-cryengine-3
float4 low_order_sh_rotate(float3 direction, float2 zh_coefficients) 
{ 
	// compute sine and cosine of thetta angle 
	// beware of singularity when both x and y are 0 (no need to rotate at all) 
	float2 theta12_cs = 0.0f;
	
	if (length(direction.xy) > 0.001f)
		theta12_cs		= normalize(direction.xy);
	
	// compute sine and cosine of phi angle 
	float2 phi12_cs;
	phi12_cs.x			= sqrt(1.0f - direction.z * direction.z); 
	phi12_cs.y			= direction.z; 
	
	float4 result; 
	
	// The first band is rotation-independent 
	result.x			= zh_coefficients.x; 
	// rotating the second band of SH 
	result.y			= zh_coefficients.y * phi12_cs.x * theta12_cs.y; 
	result.z			= -zh_coefficients.y * phi12_cs.y; 
	result.w			= zh_coefficients.y * phi12_cs.x * theta12_cs.x;
	
	return result; 
}

float4 low_order_sh_project_cone(float3 direction, float angle)
{
	float2 zh_coefficients = float2(
		0.5f * (1.0f - cos(angle)),
		0.75f * sin(angle) * sin(angle)
	);
	return low_order_sh_rotate(direction, zh_coefficients);
}

float4 low_order_sh_project_cone_60degrees(float3 direction)
{
	return low_order_sh_rotate(direction, float2(0.25f, 0.5f));
}

float4 low_order_sh_project_cone_90degrees(float3 direction)
{
	return low_order_sh_rotate(direction, float2(0.5f, 0.75f));
}

#endif	//	XRAY_SHADERS_SM_4_0_SPHERICAL_HARMONICS_H_INCLUDED
