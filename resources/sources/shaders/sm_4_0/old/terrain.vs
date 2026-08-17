//#include "terrain.h"

/*	$DEFINES$:
		CONFIG_VERTEX_INPUT_TYPE,
		CONFIG_TNORMAL,
		CONFIG_PARALLAX,
*/


#include "common_cbuffers.h" // for unpack_bx4
#include "common_functions.h" // for unpack_bx4

#include "common.h"
#include "v2p_common.h"
#include "v_common.h"

#include "vsf_tangent_space_transform.h"
//#include "vsf_copy_tc.h"

#define V2P_PARAM_TC0			1

#if CONFIG_PARALLAX || CONFIG_TNORMAL
#	define V2P_PARAM_TANGENTS		1
#endif

#include "v_terrain.h"
#include "v2p_terrain.h"

Texture2D					t_height;
uniform float4				terrain_size;
uniform float4				start_corner;

#define 	max_height	 	256.f

v2p_terrain main( v_terrain In)
{
	v_common 		input;			
	fill_v_common	( In, input);

	v2p_common 			  output;			
	zero_v2p_common		( output);
	
	input.tc 		= float2( input.position.x /*- start_corner.x*/, -input.position.y/*-(input.position.y + start_corner.z)*/);
	float height 	= t_height.Load( int3( input.tc.xy, 0) ).x;

	input.position 	= float3( input.position.x+start_corner.x, height*max_height - max_height/2.f, input.position.y+start_corner.z);
	
	output.hposition	= mul(m_WVP, float4(input.position,1));
	output.position		= float3 ( mul(m_WV, 	float4(input.position,1)).xyz);
	
	float h_0, h_1, h_2, h_3;
	int texture_width, texture_height;
	t_height.GetDimensions( texture_width, texture_height);
	
	h_0 = t_height.Load( int3( input.tc, 0) + int3( 0,(input.tc.y>0?-1:0), 0)).x;
	h_1 = t_height.Load( int3( input.tc, 0) + int3((input.tc.x>0?-1:0), 0, 0)).x;
	h_2 = t_height.Load( int3( input.tc, 0) + int3( (input.tc.x<texture_width-1?1:0), 0, 0)).x;
	h_3 = t_height.Load( int3( input.tc, 0) + int3( 0, (input.tc.y<texture_height-1?1:0), 0)).x;
	
	
	h_0 = h_0*max_height;
	h_1 = h_1*max_height;
	h_2 = h_2*max_height;
	h_3 = h_3*max_height;
		
	float3 n;
	float texelAspect = 0.999f;
	n.z = -(h_0 - h_3) * texelAspect * ( input.tc.y<=0 || input.tc.y>=texture_height - 1 ? 2:1);
	n.x = (h_1 - h_2) * texelAspect * ( input.tc.x<=0 || input.tc.x>=texture_width - 1 ? 2:1);
	n.y = 2.f;
	
	input.normal 	= normalize( n );
	input.binormal 	= normalize( cross( input.normal, float3(1,0,0) ));
	input.tangent 	= cross( input.binormal, input.normal );
	
	vsf_tangent_space_transform( input.normal, input.tangent, input.binormal, output.tbn_x, output.tbn_y, output.tbn_z);
	
/*		float3x3 TBN = mul((float3x3)m_WV, 
			transpose(float3x3(
				(input.tangent),
				(input.binormal),
				(input.normal)
			))
			);
		output.tbn_x 	 = TBN[0];
		output.tbn_y 	 = TBN[1];
		output.tbn_z 	 = TBN[2];
	*/

	output.normal		= mul( (float3x3)m_WV, input.normal);//input.N);

//	output.c0.rgba  = input.c0;
//	output.c1.bgra  = input.c1;	

	//if(	CONFIG_TNORMAL || CONFIG_PARALLAX)
		output.tc		= input.tc;//input.TC0;

	//vsf_copy_tc( input, output);
	
	v2p_terrain result;
	fill_v2p_terrain ( output, result);
	
	return result;
}