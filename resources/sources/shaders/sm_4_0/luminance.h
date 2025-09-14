////////////////////////////////////////////////////////////////////////////
//	Created		: 05.05.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_LUMINANCE_H_INCLUDED
#define SHADERS_SM_4_0_LUMINANCE_H_INCLUDED

//static const float3 luminance_weights = float3(0.3576, 0.7152, 0.1192);
static const float3 luminance_weights = float3(0.2125f, 0.7154f, 0.0721f);
//static const float3 luminance_weights = float3(0.3390f, 0.6780f, 0.1130f);

static const float  pixel_luminance_epsilon	= 0.001f;
static const float  num_luminance_pixels 	= 256.0f * 256.0f;

float get_luminance	( float3 color )
{
	return dot( color, luminance_weights );
}

#endif // #ifndef SHADERS_SM_4_0_LUMINANCE_H_INCLUDED