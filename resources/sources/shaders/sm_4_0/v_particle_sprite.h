////////////////////////////////////////////////////////////////////////////
//	Created		: 17.11.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef	FB_V_PARTICLE_SPRITE_H
#define FB_V_PARTICLE_SPRITE_H

struct v_particle_sprite
{
	float3	position		: POSITION;
	float4	color			: TEXCOORD0;
	float2	tc				: TEXCOORD1;  // Optimize uv_index, add index.
	float2	size			: TEXCOORD2;
	float	rotation		: TEXCOORD3;
	float3	prev_position	: TEXCOORD4;
#if CONFIG_USE_SUBUV
	float2	size_tc			: TEXCOORD5;
	float4	blend_tc		: TEXCOORD6;
#endif // #if CONFIG_USE_SUBUV
}; // struct v_particle_sprite

#endif // FB_V_PARTICLE_SPRITE_H