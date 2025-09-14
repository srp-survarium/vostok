#ifndef	FB_V_BASE_H
#define FB_V_BASE_H

struct	v_base
{
#if	V_PARAM_POSITION 
	float3	position		: POSITION;	// View space position 
#endif

#if	V_PARAM_NORMAL 
	float3	normal			: NORMAL;	// View space normal        
#endif

#if	V_PARAM_TC0
	float2	tc				: TEXCOORD0;	// Texture coordinates
#endif

#if	V_PARAM_TC1
	float2	tc1				: TEXCOORD1;	// Texture coordinates
#endif

#if	V_PARAM_COLOR0 
	float4	color			: COLOR0;		// 
#endif

#if	V_PARAM_COLOR1 
	float4	color1			: COLOR1;		// 
#endif

#if V_PARAM_TC_DETAIL
	float2	tcdetail		: TEXCOORD2;	// 
#endif

#if V_PARAM_TANGENTS
	float3	tangent		:TANGENT;	// tangent
	float3	binormal	:BINORMAL;	// binormal
#endif

};

void fill_v_common( v_base input, out v_common output)
{
#if	V_PARAM_POSITION 
	output.position	= input.position;
#else
	output.position	= float3( 0.f, 0.f, 0.f );
#endif

#if	V_PARAM_NORMAL 
	output.normal		= input.normal;
#else
	output.normal		= float4( 0.f, 0.f, 0.f, 0.f );
#endif

#if	V_PARAM_TC0
	output.tc			= input.tc;
#else
	output.tc			= float2( 0.f, 0.f );
#endif

#if	V_PARAM_TC1
	output.tc1			= input.tc1;
#else
	output.tc1			= float2( 0.f, 0.f );
#endif

#if	V_PARAM_COLOR0 
	output.color		= input.color;
#else
	output.color		= float4( 0.f, 0.f, 0.f, 0.f );
#endif

#if	V_PARAM_COLOR1
	output.color1		= input.color1;
#else
	output.color1		= float4( 0.f, 0.f, 0.f, 0.f );
#endif

#if V_PARAM_TC_DETAIL
	output.tc_detail	= input.tc_detail;
#else
	output.tc_detail	= float2( 0.f, 0.f );
#endif

#if V_PARAM_TANGENTS
	output.tangent		= input.tangent;
	output.binormal		= input.binormal;
#else
	output.tangent		= float3( 0.f, 0.f, 0.f);
	output.binormal		= float3( 0.f, 0.f, 0.f);
#endif
}

#endif // FB_V_BASE_H