////////////////////////////////////////////////////////////////////////////
//	Created		: 16.11.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef	FRESNEL_POWER_H
#define FRESNEL_POWER_H

float psf_fresnel( float3 normal, float3 eye_dir, float power)
{
	return pow(1.0 - dot(eye_dir, normal), power);
}



#endif // FRESNEL_POWER_H
