////////////////////////////////////////////////////////////////////////////
//	Created		: 28.04.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_GAMMA_CORRECTION_H_INCLUDED
#define SHADERS_SM_4_0_GAMMA_CORRECTION_H_INCLUDED

#include "debug.h"

static const float  gamma_value				= 2.2f;

float3 convert_to_linear_space( in float3 color )
{
	return	pow( ABS_TO_REMOVE_WARNING(color), gamma_value );
}

float3 convert_from_linear_space( in float3 color )
{
	return	pow( ABS_TO_REMOVE_WARNING(color), 1.f/gamma_value );
}

float3 convert_from_linear_space( in float3 color, float gamma_correction_factor )
{
	return	pow( ABS_TO_REMOVE_WARNING(color), 1.f/(gamma_correction_factor*gamma_value) );
}

#endif // #ifndef SHADERS_SM_4_0_GAMMA_CORRECTION_H_INCLUDED