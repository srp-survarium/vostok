////////////////////////////////////////////////////////////////////////////
//	Created		: 20.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_POSITIONAL_LIGHT_H_INCLUDED
#define SHADERS_SM_4_0_POSITIONAL_LIGHT_H_INCLUDED

cbuffer positional_lights_constants {
	float3		light_position;				// in world space
	float		light_attenuation_power;
	float		light_range;
}

#endif // #ifndef SHADERS_SM_4_0_POSITIONAL_LIGHT_H_INCLUDED