#ifndef	FB_COMMON_H
#define FB_COMMON_H

struct	v2p_common
{
	float4	hposition		: SV_Position;
	float3	position		: TEXCOORD0;	// View space position 
	float3	normal			: TEXCOORD1;	// View space normal        
	float2	tc				: TEXCOORD2;	// Texture coordinates 0
	float4	tc1				: TEXCOORD3;	// Texture coordinates 1
	float4	color			: COLOR0;		// 
	float2	tcdetail		: TEXCOORD4;	// 
	float3	tbn_x			: TEXCOORD5;
	float3	tbn_y			: TEXCOORD6;
	float3	tbn_z			: TEXCOORD7;
};

void zero_v2p_common( out v2p_common data)
{
	data.hposition	=		float4( 0.f, 0.f, 0.f, 0.f);
	data.position	=		float3( 0.f, 0.f, 0.f);
	data.normal		=		float3( 0.f, 0.f, 0.f);
	data.tc			=		float2( 0.f, 0.f);
	data.tc1		=		float4( 0.f, 0.f, 0.f, 0.f);
	data.color		=		float4( 0.f, 0.f, 0.f, 0.f);
	data.tcdetail	=		float2( 0.f, 0.f);
	data.tbn_x		=		float3( 0.f, 0.f, 0.f);
	data.tbn_y		=		float3( 0.f, 0.f, 0.f);
	data.tbn_z		=		float3( 0.f, 0.f, 0.f);
}


#endif // FB_COMMON_H