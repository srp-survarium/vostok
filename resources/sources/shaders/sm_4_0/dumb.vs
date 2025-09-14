#include "common.h"

float4 	main( float3 position: POSITION)	: SV_POSITION
{
	return  mul		( m_WVP, 	float4( position, 1.f));
}
