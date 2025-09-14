////////////////////////////////////////////////////////////////////////////
//	Created		: 20.04.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SHADERS_SM_4_0_LIGHT_FUNCTIONS_H_INCLUDED
#define SHADERS_SM_4_0_LIGHT_FUNCTIONS_H_INCLUDED

float nearest_point_on_segment	( float value, float segment_start, float segment_finish )
{
	return	min( max( value, segment_start ), segment_finish );
}

float3 nearest_point_on_segment	( float3 value, float3 segment_start, float3 segment_finish )
{
	return	min( max( value, segment_start ), segment_finish );
}

#endif // #ifndef SHADERS_SM_4_0_LIGHT_FUNCTIONS_H_INCLUDED