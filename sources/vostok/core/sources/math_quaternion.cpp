////////////////////////////////////////////////////////////////////////////
//	Created 	: 23.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

using vostok::math::quaternion;
using vostok::math::float4x4;

static float const zero_tolerance = 0.1f; // ?!

inline float sqrt_safe( float val )
{
	return val<=0.f ? 0 : sqrt( val );

}

quaternion::quaternion	( float4x4 const& matrix_raw )
{

	R_ASSERT		( matrix_raw.valid() );
	float4x4 matrix = matrix_raw;
	matrix.set_scale( float3(1.f, 1.f, 1.f) );
	float const	trace = matrix.e00 + matrix.e11 + matrix.e22;
	float		s;
	if (trace > 0.0f){
		s = sqrt(trace + 1.0f);
		w 		= s * 0.5f;
		s 		= 0.5f / s;

		x 		= ( matrix.e21 - matrix.e12 ) * s;
		y 		= ( matrix.e02 - matrix.e20 ) * s;
		z 		= ( matrix.e10 - matrix.e01 ) * s;
		return;
	}

	enum {
		A,
		E,
		I
	};

	int biggest;
	if (matrix.e00 > matrix.e11){
		if (matrix.e22 > matrix.e00)
			biggest = I;	
		else
			biggest = A;
	}
	else {
		if (matrix.e22 > matrix.e00)
			biggest = I;
		else
			biggest = E;
	}

	// in the unusual case the original trace fails to produce a good sqrt, try others...
	switch (biggest){
		case A:
			s = sqrt_safe( matrix.e00 - (matrix.e11 + matrix.e22) + 1.0f);
			if (s > zero_tolerance){
				x = s * 0.5f;
				s = 0.5f / s;
				w = (matrix.e21 - matrix.e12) * s;
				y = (matrix.e01 + matrix.e10) * s;
				z = (matrix.e02 + matrix.e20) * s;
				break;
			}
			// I
			s = sqrt_safe( matrix.e22 - (matrix.e00 + matrix.e11) + 1.0f);
			if (s > zero_tolerance){
				z = s * 0.5f;
				s = 0.5f / s;
				w = (matrix.e10 - matrix.e01) * s;
				x = (matrix.e20 + matrix.e02) * s;
				y = (matrix.e21 + matrix.e12) * s;
				break;
			}
			// E
			s = sqrt_safe( matrix.e11 - (matrix.e22 + matrix.e00) + 1.0f);
			if (s > zero_tolerance){
				y = s * 0.5f;
				s = 0.5f / s;
				w = (matrix.e02 - matrix.e20) * s;
				z = (matrix.e12 + matrix.e21) * s;
				x = (matrix.e10 + matrix.e01) * s;
				break;
			}
			break;
		case E:
			s = sqrt_safe( matrix.e11 - (matrix.e22 + matrix.e00) + 1.0f);
			if (s > zero_tolerance){
				y = s * 0.5f;
				s = 0.5f / s;
				w = (matrix.e02 - matrix.e20) * s;
				z = (matrix.e12 + matrix.e21) * s;
				x = (matrix.e10 + matrix.e01) * s;
				break;
			}
			// I
			s = sqrt_safe( matrix.e22 - (matrix.e00 + matrix.e11) + 1.0f);
			if (s > zero_tolerance){
				z = s * 0.5f;
				s = 0.5f / s;
				w = (matrix.e10 - matrix.e01) * s;
				x = (matrix.e20 + matrix.e02) * s;
				y = (matrix.e21 + matrix.e12) * s;
				break;
			}
			// A
			s = sqrt_safe( matrix.e00 - (matrix.e11 + matrix.e22) + 1.0f);
			if (s > zero_tolerance){
				x = s * 0.5f;
				s = 0.5f / s;
				w = (matrix.e21 - matrix.e12) * s;
				y = (matrix.e01 + matrix.e10) * s;
				z = (matrix.e02 + matrix.e20) * s;
				break;
			}
			break;
		case I:
			s = sqrt_safe( matrix.e22 - (matrix.e00 + matrix.e11) + 1.0f);
			if (s > zero_tolerance){
				z = s * 0.5f;
				s = 0.5f / s;
				w = (matrix.e10 - matrix.e01) * s;
				x = (matrix.e20 + matrix.e02) * s;
				y = (matrix.e21 + matrix.e12) * s;
				break;
			}
			// A
			s = sqrt_safe( matrix.e00 - (matrix.e11 + matrix.e22) + 1.0f);
			if (s > zero_tolerance){
				x = s * 0.5f;
				s = 0.5f / s;
				w = (matrix.e21 - matrix.e12) * s;
				y = (matrix.e01 + matrix.e10) * s;
				z = (matrix.e02 + matrix.e20) * s;
				break;
			}
			// E
			s = sqrt_safe( matrix.e11 - (matrix.e22 + matrix.e00) + 1.0f);
			if (s > zero_tolerance){
				y = s * 0.5f;
				s = 0.5f / s;
				w = (matrix.e02 - matrix.e20) * s;
				z = (matrix.e12 + matrix.e21) * s;
				x = (matrix.e10 + matrix.e01) * s;
				break;
			}
			break;
	}
	vector.normalize	( );
	R_ASSERT		( is_unit() );
}

quaternion slerp_optimized ( quaternion const& q0, quaternion const& q1, float const t )
{
	float cosom	=  q0.x * q1.x +
				   q0.y * q1.y +
				   q0.z * q1.z +
				   q0.w * q1.w;

	float		Scale0, Scale1, sign;
	if ( cosom < 0.f ) {
		cosom	= -cosom;
		sign	= -1.f;
	} else {
		sign	= 1.f;
	}

	if ( cosom < 1.0f - vostok::math::epsilon_5 ) {
		const float	omega	= acos( cosom );
		const float	i_sinom = 1.f / sin( omega );
		const float	t_omega	= t*omega;
		Scale0	= sin( omega - 	t_omega ) * i_sinom;
		Scale1	= sin( t_omega			) * i_sinom;
	}
	else {
		// has numerical difficulties around cosom == 0
		// in this case degenerate to linear interpolation
		Scale0	= 1.0f - t;
		Scale1	= t;
	}

	Scale1		*= sign;
	return		quaternion( q0.vector*Scale0 + q1.vector*Scale1 );
}

quaternion vostok::math::slerp ( quaternion const& q0, quaternion const& q1, const float t )
{
	R_ASSERT		( q0.is_unit() );
	R_ASSERT		( q1.is_unit() );
	R_ASSERT		( (t >= 0.f) && (t <= 1.f), "Quaternion::slerp - invalid 'time_delta' arrived: %f", t );

	return slerp_optimized( q0, q1, t );

}

quaternion extrapolated_slerp ( std::pair< quaternion, float > const* const begin, std::pair< quaternion, float > const* const end )
{
	quaternion	result	= begin->first;
	float		weight	= begin->second;
	for ( std::pair< quaternion, float > const* i = begin + 1; i != end; ++i ) {
		if ( i->second != 0.f ) {
			result	= slerp_optimized(
				result, i->first, i->second / ( i->second + weight )
			);
			weight	+= i->second;
		}
	}

	return		result;
}

quaternion vostok::math::weighted_blend ( std::pair< quaternion, float > const* const begin, std::pair< quaternion, float > const* const end )
{

	return extrapolated_slerp( begin, end );
}
