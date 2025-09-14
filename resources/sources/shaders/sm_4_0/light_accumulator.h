////////////////////////////////////////////////////////////////////////////
//	Created		: 16.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_ACCUMULATOR_H_INCLUDED
#define SHADERS_SM_4_0_ACCUMULATOR_H_INCLUDED

struct light_factors {
	float	diffuse;
	float	specular;
}; // struct light_factors

struct light_accumulator {
	float4	diffuse;
	float4	specular;
}; // struct light_accumulator

#endif // #ifndef SHADERS_SM_4_0_ACCUMULATOR_H_INCLUDED