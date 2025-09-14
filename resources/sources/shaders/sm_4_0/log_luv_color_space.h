////////////////////////////////////////////////////////////////////////////
//	Created		: 26.04.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_LOG_LUV_COLOR_SPACE_H_INCLUDED
#define SHADERS_SM_4_0_LOG_LUV_COLOR_SPACE_H_INCLUDED

// to_log_luv matrix, for encoding
static const float3x3 to_log_luv = float3x3 (
    0.2209, 0.3390, 0.4184,
    0.1138, 0.6780, 0.7319,
    0.0102, 0.1130, 0.2969
);

// Inverse to_log_luv matrix, for decoding
static const float3x3 from_log_luv = float3x3 (
    6.0014, -2.7008, -1.7996,
   -1.3320,  3.1029, -5.7721,
    0.3008, -1.0882,  5.6268
);

void log_luv_set_lightness	( inout float4 color_in_log_luv, float lightness )
{
	float log_lightness		= 2 * log2(lightness) + 127;
    color_in_log_luv.w		= frac( log_lightness );
    color_in_log_luv.z		= ( log_lightness  - (floor(color_in_log_luv.w*255.0f))/255.0f)/255.0f;
}

float log_luv_get_lightness	( inout float4 color_in_log_luv )
{
	float log_lightness		= color_in_log_luv.z * 255 + color_in_log_luv.w;
	return					exp2( (log_lightness - 127) / 2 );
}

float4 log_luv_encode		( in float3 color_in_rgb )
{
    float4 result;
    float3 Xp_Y_XYZp		= mul(color_in_rgb, to_log_luv);
    Xp_Y_XYZp				= max(Xp_Y_XYZp, float3(1e-6, 1e-6, 1e-6));
    result.xy				= Xp_Y_XYZp.xy / Xp_Y_XYZp.z;
	log_luv_set_lightness	( result, Xp_Y_XYZp.y );
    return					result;
}

float3 log_luv_decode		( in float4 color_in_log_luv )
{
    float3 Xp_Y_XYZp;
    Xp_Y_XYZp.y				= log_luv_get_lightness( color_in_log_luv );
    Xp_Y_XYZp.z				= Xp_Y_XYZp.y / color_in_log_luv.y;
    Xp_Y_XYZp.x				= color_in_log_luv.x * Xp_Y_XYZp.z;
    float3 color_in_rgb		= mul(Xp_Y_XYZp, from_log_luv);
    return					max( color_in_rgb, 0 );
}

float4 log_luv_blend		( in float4 left_in_log_luv, in float4 right_in_log_luv )
{
	float4 result;
	float left_lightness	= log_luv_get_lightness( left_in_log_luv );
	float right_lightness	= log_luv_get_lightness( right_in_log_luv );
	result.xy				= 
		lerp (
			left_in_log_luv.xy,
			right_in_log_luv.xy,
			saturate(
				(
					right_lightness
					/
					max( left_lightness + right_lightness, 1e-6 )
				)
			)
		);

	log_luv_set_lightness	( result, left_lightness + right_lightness );
	return					result;
}

#endif // #ifndef SHADERS_SM_4_0_LOG_LUV_COLOR_SPACE_H_INCLUDED