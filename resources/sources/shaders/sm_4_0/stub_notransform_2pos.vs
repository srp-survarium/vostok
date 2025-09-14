#include "common_iostructs.h"

uniform float4		screen_res;		// Screen resolution (x-Width,y-Height, zw - 1/resolution)

struct sun_mask_position2
{
	float4 P 		: POSITIONT;
	float3 eye_ray 	: POSITION;
	float2 Tex0		: TEXCOORD0;
};

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_sun main ( sun_mask_position2 I )
{
	v2p_sun O;

	O.HPos.x = I.P.x * screen_res.z * 2 - 1;
	O.HPos.y = (I.P.y * screen_res.w * 2 - 1)*-1;
	O.HPos.zw = I.P.zw;
	O.HPos.w = 1;
	
	O.Tex0 = I.Tex0;

	O.eye_ray = I.eye_ray;

 	return O;
}