////////////////////////////////////////////////////////////////////////////
//	Created		: 17.11.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef	FB_V_PARTICLE_BEAMTRAIL_H
#define FB_V_PARTICLE_BEAMTRAIL_H

struct v_particle_beamtrail
{
	float3	position		: POSITION;
	float4	color			: TEXCOORD0;
	float2	tc				: TEXCOORD1;
}; // struct v_particle_beamtrail

#endif // FB_V_PARTICLE_BEAMTRAIL_H