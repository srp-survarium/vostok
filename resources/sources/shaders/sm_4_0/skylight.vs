#include "common.h"

uniform float4	skylight_parameters6;	// xyz - wrap light direction

struct	v_in
{
	float4	position	: POSITION;
	float2	tc			: TEXCOORD0;
};

float4 	main	( v_in I, out float2 tc : TEXCOORD0, out float3 basis_x : TEXCOORD1, out float3 basis_y : TEXCOORD2, out float3 basis_z : TEXCOORD3 ) : SV_POSITION
{
	tc				= I.tc;

	float	angle	= ( -skylight_parameters6.z > 0 ) ? acos( skylight_parameters6.x ) + 3.14159265f : acos( -skylight_parameters6.x );

	float	s, c;
	sincos( angle, s, c );

	basis_x			= normalize( float3( c, 0, s ) );
	basis_y			= float3( 0, 1, 0 );

	float	s2		= sin( -angle );
	basis_z			= normalize( float3( s2, 0, c ) );

	return	I.position;
}
