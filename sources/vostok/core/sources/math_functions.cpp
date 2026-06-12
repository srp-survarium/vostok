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

float3 vostok::math::closest_point_on_segment( float3 const& point, float3 const& segment_origin, float3 const& segment_displacement )
{
	float domen_value	= ( ( point - segment_origin ) | segment_displacement ) / segment_displacement.squared_length( );
	math::clamp			( domen_value, 0.f, 1.f );

	return segment_origin + segment_displacement * domen_value;
}

/*
   Calculate the line segment PaPb that is the shortest route between
   two lines P1P2 and P3P4. Calculate also the values of mua and mub where
      Pa = P1 + mua (P2 - P1)
      Pb = P3 + mub (P4 - P3)
   Return FALSE if no solution exists.
	https://paulbourke.net/geometry/pointlineplane/lineline.c
*/
bool vostok::math::line_line_intersect_non_parallel ( float3 const& p1, float3 const& d1, float3 const& p2, float3 const& d2, float3& pa, float3& pb, float& mua, float& mub )
{
	R_ASSERT( !math::is_zero( d1.x ) || !math::is_zero( d1.y ) || !math::is_zero( d1.z ) );
	R_ASSERT( !math::is_zero( d2.x ) || !math::is_zero( d2.y ) || !math::is_zero( d2.z ) );

	float3 const&	p34		= d2;
	float3 const&	p12		= d1;
	float3 const	p31		= p1 - p2;

	float const		d3412	= p34 | p12;
	float const		d3434	= p34 | p34;
	float const		d1212	= p12 | p12;

	float const		denom	= d1212 * d3434 - d3412 * d3412;
	R_ASSERT( !math::is_zero( denom ) );
	
	float const		d3134	= p31 | p34;
	float const		d3112	= p31 | p12;

	float const		numer	= d3134 * d3412 - d3112 * d3434;

	mua	= numer / denom;
	mub	= ( d3134 + d3412 * mua ) / d3434;

	pa	= p1 + mua * d1;
	pb	= p2 + mub * d2;

	return true;
}

#if 0
#define EPSILON 0.000001
// dist3D_Segment_to_Segment():
//    Input:  two 3D line segments S1 and S2
//    Return: the shortest distance between S1 and S2
//
// @TODO: Understand the algorithm
// https://web.archive.org/web/20111008212356/http://softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm#dist3D_Segment_to_Segment()
float vostok::math::segment_to_segment_distance ( 
		float3 const& v0, 
		float3 const& v1, 
		float3 const& u0, 
		float3 const& u1 
	)
{
	float3   u = v1 - v0;
	float3   v = u1 - u0;
	float3   w = v0 - u0;

	float a = u | u;
	float b = u | v;
	float c = v | v;
	float d = u | w;
	float e = v | w;
	float D = a*c - b*b;
    float    sc, sN, sD = D;      // sc = sN / sD, default sD = D >= 0
    float    tc, tN, tD = D;      // tc = tN / tD, default tD = D >= 0

    // compute the line parameters of the two closest points
	if (D < EPSILON) { // the lines are almost parallel
		sN = 0.0;        // force using point P0 on segment S1
		sD = 1.0;        // to prevent possible division by 0.0 later
		tN = e;
		tD = c;
	}
	else {                // get the closest points on the infinite lines
		sN = (b*e - c*d);
		tN = (a*e - b*d);
		if (sN < 0.0) {       // sc < 0 => the s=0 edge is visible
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {  // sc > 1 => the s=1 edge is visible
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0) {           // tc < 0 => the t=0 edge is visible
		tN = 0.0;
		// recompute sc for this edge
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) {      // tc > 1 => the t=1 edge is visible
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else {
			sN = (-d + b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	sc = (vostok::math::abs(sN) < EPSILON ? 0.0f : sN / sD);
	tc = (vostok::math::abs(tN) < EPSILON ? 0.0f : tN / tD);

	// get the difference of the two closest points
	float3   dP = w + (sc * u) - (tc * v);  // = S1(sc) - S2(tc)

	return length(dP);   // return the closest distance
}
#endif // #if 0

// sushi@TODO: Doesn't seem to match exactly
// sushi@TODO: We need a test framework for .obj functions to make a comparison
// sushi@TODO: Squared distance is returned
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