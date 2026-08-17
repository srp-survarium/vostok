////////////////////////////////////////////////////////////////////////////
//	Created		: 15.10.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

/*
	$DEFINES$:
		CONFIG_VERTEX_INPUT_TYPE,
		CONFIG_USE_SUBUV,
*/

#define V2P_PARAM_H_POSITION 	1
#define V2P_PARAM_POSITION 		1
#define V2P_PARAM_TC0		 	1
#define V2P_PARAM_TC1		 	1
#define V2P_PARAM_COLOR0	 	1

#include "v_particle_beamtrail.h"
#include "v2p_common.h"
#include "v2p_base.h"

uniform float4x4 m_VP;
uniform float3x4 m_W;

uniform float3x4 m_V;
uniform float3x4 m_WV;

struct vertex_output_struct
{
	float3	position		: TEXCOORD0;
	float4	color			: TEXCOORD1;
	float2	tc				: TEXCOORD2;
};

vertex_output_struct main (in v_particle_beamtrail input,
						   out float4 hposition: SV_Position)
{
	vertex_output_struct output;
	
	output.color	  		= input.color;
	output.tc		  		= input.tc;
	// m_W identity if not using local space transform.
	hposition  				= mul(m_VP,float4(mul(m_W,float4(input.position.xyz,1)).xyz,1));
	output.position			= mul(m_WV, float4(input.position.xyz,1)).xyz;
	
	return output;
}




