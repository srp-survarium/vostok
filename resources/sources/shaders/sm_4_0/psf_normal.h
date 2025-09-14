////////////////////////////////////////////////////////////////////////////
//	Created		: 13.10.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

// $DEFINES$: CONFIG_TNORMAL

#ifndef	FB_NORMAL_H
#define FB_NORMAL_H


#if CONFIG_NORMAL
float4 min_t_normal;
float4 max_t_normal;
#endif // #if CONFIG_NORMAL

float3	psf_tnormal_detailed( Texture2D _t_normal, float3 normal_details, float3x3 tangent_trans, float2 tc)
{
	float3 normal;
	normal.xy = _t_normal.Sample( s_normal, tc).wy * 2.f - 1.f;
	normal.z  = sqrt( saturate(1.0f - dot(normal.xy, normal.xy)));
	normal.y  = -normal.y;
	
	normal 	  = normalize(normal);
	
	normal	  += normal_details;
	
	normal 	  = mul( tangent_trans, normal);
	return normalize( normal);
}

float3	psf_tnormal( Texture2D _t_normal, float3x3 tangent_trans, float2 tc, float3 multiplier)
{
	float3 normal;
	normal.xy = _t_normal.Sample( s_normal, tc).wy * 2.f - 1.f;
	normal.z  = sqrt( saturate(1.0f - dot(normal.xy, normal.xy)));
	normal.y  = -normal.y;
	normal 	  = normalize(normal);
	normal 	  = mul( tangent_trans, normal * multiplier);
	return normalize( normal);
}

float3	psf_tnormal( Texture2D _t_normal, float2 tc, float3 multiplier)
{
	float3 normal;
	normal.xy = _t_normal.Sample( s_normal, tc).wy * 2.f - 1.f;
	normal.z  = sqrt( saturate(1.0f - dot(normal.xy, normal.xy)));
	normal.y  = -normal.y;
	normal 	  = normalize(normal * multiplier);
	return normal;
}

float3	psf_tnormal( Texture2DArray _t_normal, float3x3 tangent_trans, float2 tc, float tex_ind, float3 multiplier)
{
	float3 normal;
	normal.xy = _t_normal.Sample( s_normal, float3(tc, tex_ind)).wy * 2.f - 1.f;
	normal.z  = sqrt( saturate(1.0f - dot(normal.xy, normal.xy)));
	normal.y  = -normal.y;
	normal 	  = normalize(normal);
	normal 	  = mul( tangent_trans, normal * multiplier);
	return normalize( normal);
}

float3	psf_tnormal( Texture2DArray _t_normal, float2 tc, float tex_ind, float3 multiplier)
{
	float3 normal;
	normal.xy = _t_normal.Sample( s_normal, float3(tc, tex_ind)).wy * 2.f - 1.f;
	normal.z  = sqrt( saturate(1.0f - dot(normal.xy, normal.xy)));
	normal.y  = -normal.y;
	normal 	  = normalize(normal * multiplier);
	return normal;
}

#endif // FB_NORMAL_H