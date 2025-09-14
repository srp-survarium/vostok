#ifndef	V_COMMON_H
#define V_COMMON_H

struct	v_common
{
	float3	position		: POSITION;		// 
	float3	normal			: NORMAL;		// 
	float2	tc				: TEXCOORD0;	// 
	float2	tc1				: TEXCOORD1;	// 
	float4	color			: COLOR0;		// 
	float4	color1			: COLOR1;		// 
	float2	tc_detail		: TEXCOORD1;	// 
	float3	tangent			: TANGENT;		// 
	float3	binormal		: BINORMAL;		// 
};

#endif // V_COMMON_H