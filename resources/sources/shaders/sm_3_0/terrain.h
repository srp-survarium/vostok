struct terrain_vert
{
	float3 P 		: POSITION;
	float3 N 		: NORMAL;
	float4 c0 		: COLOR0;
	float4 c1 		: COLOR1;
	float2 TC0 		: TEXCOORD0;
	float2 TC1 		: TEXCOORD1;
	float2 TC2 		: TEXCOORD2;
	float4 TC_inds	: TEXCOORD3;		
};

struct terrain_vert_out
{
	half4 	hpos	: POSITION;
	half4  	P 		: TEXCOORD3;
	half3 	N		: NORMAL;
	half4 	c0 		: COLOR0;
	half4 	c1		: COLOR1;
	half2 	tc0 	: TEXCOORD0;
	half2 	tc1 	: TEXCOORD1;
	half2 	tc2 	: TEXCOORD2;
	half4 	tc_inds	: TEXCOORD4;
};