////////////////////////////////////////////////////////////////////////////
//	Created		: 22.03.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

using vostok::math::float3_pod;
using vostok::math::float3;

// change angles xyz representation to be close to to_angles; average_distance = average distance criteria 
float3 vostok::math::to_close_xyz( float3_pod const& angles, float3_pod const& to_angles, float const average_distance )
{
#if 0
	float3 dist;
	//reduce distance to  pi,-pi 
	dist.x =	 angle_normalize_signed( angles.x - to_angles.x );
	dist.y =	 angle_normalize_signed( angles.y - to_angles.y );
	dist.z =	 angle_normalize_signed( angles.z - to_angles.z );
	
	float y_reverce_dist = angle_normalize_signed( -angles.y - to_angles.y );

	float y_dist = abs( y_reverce_dist ) + abs( dist.y );//.

	//float y_dist = angle_normalize_signed( abs( y_reverce_dist ) - abs( dist.y ) );//.

	float avr = ( abs( dist.x ) + abs( y_dist ) + abs( dist.z ) )/3.f;
	
	if( avr >= average_distance - math::epsilon_6  ) // than change triple representation
	{
		dist.x = angle_rotation_dist_hemi_flip( dist.x );
		dist.y = angle_rotation_dist_hemi_flip( y_reverce_dist );
		dist.z = angle_rotation_dist_hemi_flip( dist.z );
	}

	return		to_angles + dist;
#else // #if 0
	//reduce distance to  pi,-pi 
	float3 const distance			=
		float3 (
			angle_normalize_signed( angles.x - to_angles.x ),
			angle_normalize_signed( angles.y - to_angles.y ),
			angle_normalize_signed( angles.z - to_angles.z )
		);
	
	float const y_reverce_distance	= angle_normalize_signed( -angles.y - to_angles.y );
	float const y_distance			= abs( y_reverce_distance ) + abs( distance.y );//.
	float const average				= ( abs( distance.x ) + abs( y_distance ) + abs( distance.z ) )/3.f;
	if ( average < average_distance - math::epsilon_6  )
		return						to_angles + distance;

	// change triple representation
	return
		to_angles +
		float3(
			angle_rotation_dist_hemi_flip( distance.x ),
			angle_rotation_dist_hemi_flip( y_reverce_distance ),
			angle_rotation_dist_hemi_flip( distance.z )
		);
#endif // #if 0
}


float3 closest_point_on_segment( float3 const& point, float3 const& segment_origin, float3 const& segment_displacement )
{
	float domen_value	= ( ( point - segment_origin ) | segment_displacement ) / segment_displacement.squared_length( );
	vostok::math::clamp	( domen_value, 0.f, 1.f );

	return segment_origin + segment_displacement * domen_value;
}








bool line_line_intersect_non_parallel ( float3 const& p1, float3 const& d1, float3 const& p2, float3 const& d2, float3& pa, float3& pb, float& mua, float& mub )
{
	R_ASSERT( !vostok::math::is_zero( d1.x ) || !vostok::math::is_zero( d1.y ) || !vostok::math::is_zero( d1.z ) );
	R_ASSERT( !vostok::math::is_zero( d2.x ) || !vostok::math::is_zero( d2.y ) || !vostok::math::is_zero( d2.z ) );

	float3 const&	p34		= d2;
	float3 const&	p12		= d1;
	float3 const	p31		= p1 - p2;

	float const		d3412	= p34 | p12;
	float const		d3434	= p34 | p34;
	float const		d1212	= p12 | p12;

	float const		denom	= d1212 * d3434 - d3412 * d3412;
	R_ASSERT( !vostok::math::is_zero( denom ) );
	
	float const		d3134	= p31 | p34;
	float const		d3112	= p31 | p12;

	float const		numer	= d3134 * d3412 - d3112 * d3434;

	mua	= numer / denom;
	mub	= ( d3134 + d3412 * mua ) / d3434;

	pa	= p1 + mua * d1;
	pb	= p2 + mub * d2;

	return true;
}

float vostok::math::segment_to_segment_distance ( float3 const& p1, float3 const& p2, float3 const& p3, float3 const& p4 )
{
	float3 const d1		= p2 - p1;
	float3 const d2		= p4 - p3;
	float3 const cross	= d1 ^ d2;

	if( !math::is_zero( cross.x ) || !math::is_zero( cross.y ) || !math::is_zero( cross.z ) )
	{
		float3	pa;
		float3	pb;
		float	mua;
		float	mub;

		line_line_intersect_non_parallel( p1, d1, p3, d2, pa, pb, mua, mub );

		//segments cross
		if( mua > 0 && mua < 1 && mub > 0 && mub < 1 )
			return 0.f;
	}
	
	return math::min(( p1 - closest_point_on_segment( p1, p3, d2 ) ).squared_length( ),		// 1 
		   math::min(( p2 - closest_point_on_segment( p2, p3, d2 ) ).squared_length( ),		// 2
		   math::min(( p3 - closest_point_on_segment( p3, p1, d1 ) ).squared_length( ),		// 3
					 ( p4 - closest_point_on_segment( p4, p1, d1 ) ).squared_length( )		// 4
		   )																				// 5
		   )																				// 6
		   );
}
