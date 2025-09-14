#ifndef	FB_V2P_BASE_H
#define FB_V2P_BASE_H

struct	v2p_base
{
#if	V2P_PARAM_H_POSITION 
	float4	hposition		: SV_Position;
#endif

#if	V2P_PARAM_POSITION 
	float3	position		: TEXCOORD0;	// View space position 
#endif

#if	V2P_PARAM_NORMAL 
	float3	normal			: NORMAL;	// View space normal        
#endif

#if	V2P_PARAM_TC0
	float2	tc				: TEXCOORD2;	// Texture coordinates 0
#endif

#if	V2P_PARAM_TC1
	float4	tc1				: TEXCOORD3;	// Texture coordinates 1
#endif

#if	V2P_PARAM_COLOR0
	float4	color			: COLOR0;		// 
#endif

#if V2P_PARAM_TC_DETAIL		
	float2	tcdetail		: TEXCOORD4;	// 
#endif

#if V2P_PARAM_TANGENTS
 	float3	tbn_x			: TEXCOORD5;
 	float3	tbn_y			: TEXCOORD6;
 	float3	tbn_z			: TEXCOORD7;
#endif

};


void fill_v2p_base( v2p_common In, out v2p_base data)
{
#if	V2P_PARAM_H_POSITION 
	data.hposition	= In.hposition;
#endif

#if	V2P_PARAM_POSITION 
	data.position	= In.position;	// View space position 
#endif

#if	V2P_PARAM_NORMAL 
	data.normal		= In.normal;	// View space normal        
#endif

#if	V2P_PARAM_TC0
	data.tc			= In.tc;	// Texture coordinates 0
#endif

#if	V2P_PARAM_TC1
	data.tc1		= In.tc1;	// Texture coordinates 1
#endif

#if	V2P_PARAM_COLOR0
	data.color		= In.color;		// 
#endif

#if V2P_PARAM_TC_DETAIL	
	data.tcdetail	= In.tcdetail;	// 
#endif

#if V2P_PARAM_TANGENTS
	data.tbn_x		= In.tbn_x;
	data.tbn_y		= In.tbn_y;
	data.tbn_z		= In.tbn_z;
#endif		
}

void fill_v2p_common( v2p_base In, out v2p_common data)
{
#if	V2P_PARAM_H_POSITION 
	data.hposition	= In.hposition;
#else
	data.hposition	= float4( 0.f, 0.f, 0.f, 0.f);
#endif

#if	V2P_PARAM_POSITION 
	data.position	= In.position;	// View space position 
#else
	data.position	= float3( 0.f, 0.f, 0.f );
#endif

#if	V2P_PARAM_NORMAL 
	data.normal		= In.normal;	// View space normal        
#else
	data.normal		= float3( 0.f, 0.f, 0.f);
#endif

#if	V2P_PARAM_TC0
	data.tc			= In.tc;	// Texture coordinates 0
#else
	data.tc			= float2( 0.f, 0.f);
#endif

#if	V2P_PARAM_TC1
	data.tc1		= In.tc1;	// Texture coordinates 1
#else
	data.tc1		= float4( 0.f, 0.f, 0.f, 0.f);
#endif

#if	V2P_PARAM_COLOR0
	data.color		= In.color;		// 
#else
	data.color		= float4( 0.f, 0.f, 0.f, 0.f);
#endif

#if V2P_PARAM_TC_DETAIL	
	data.tcdetail	= In.tcdetail;	// 
#else
	data.tcdetail	= float2( 0.f, 0.f);// 
#endif

#if V2P_PARAM_TANGENTS
	data.tbn_x		= In.tbn_x;
	data.tbn_y		= In.tbn_y;
	data.tbn_z		= In.tbn_z;
#else
	data.tbn_x		= float3( 0.f, 0.f, 0.f);
	data.tbn_y		= float3( 0.f, 0.f, 0.f);
	data.tbn_z		= float3( 0.f, 0.f, 0.f);
#endif		
}





#endif // FB_V2P_BASE_H