struct v2p_terrain
{
	float4 	hposition	: SV_POSITION;
	float3  position 	: TEXCOORD1;
	float3 	normal		: NORMAL;

#if	V2P_PARAM_TC0
	float2 	tc 			: TEXCOORD3;
#endif

#if	V2P_PARAM_TC1
	float4 	tc1			: TEXCOORD4;
#endif

#if	V2P_PARAM_TANGENTS
 	float3	tbn_x		: TEXCOORD5;
 	float3	tbn_y		: TEXCOORD6;
 	float3	tbn_z		: TEXCOORD7;
#endif
};

void fill_v2p_terrain( v2p_common In, out v2p_terrain data)
{
	data.hposition	= In.hposition;
	data.position	= In.position;	// View space position 
	data.normal		= In.normal;	// View space normal        

#if	V2P_PARAM_TC0
	data.tc			= In.tc;	// Texture coordinates 0
#endif

#if	V2P_PARAM_TC1
	data.tc1		= In.tc1;	// Texture coordinates 1
#endif

#if V2P_PARAM_TANGENTS
	data.tbn_x		= In.tbn_x;
	data.tbn_y		= In.tbn_y;
	data.tbn_z		= In.tbn_z;
#endif		
}

void fill_v2p_common( v2p_terrain In, out v2p_common data)
{
	data.hposition	= In.hposition;
	data.position	= In.position;	// View space position 

	data.normal		= In.normal;	// View space normal        
	data.color		= float4( 0.f, 0.f, 0.f, 0.f);
	data.tcdetail	= float2( 0.f, 0.f);// 

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




