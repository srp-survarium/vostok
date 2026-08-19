////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef IK_UTILS_H_INCLUDED
#define IK_UTILS_H_INCLUDED

#include <vostok/game_core/api.h>
#include <vostok/math_float4x4.h>
#include <vostok/animation/skeleton.h>

namespace survarium {

VOSTOK_GAME_CORE_API float4x4	get_bone_matrix_in_object_space	( animation::skeleton_bone const& bone, animation::skeleton const& skeleton, float4x4 const* matrices );

inline float get_angle( float adjacent0, float adjacent1, float opposite )
{
	float angle_cos	= ( vostok::math::sqr( adjacent0 ) + vostok::math::sqr( adjacent1 ) - vostok::math::sqr( opposite ) ) / ( 2.0f * adjacent0 * adjacent1 );
	vostok::math::clamp( angle_cos, -1.0f, 1.0f );
	return vostok::math::acos( angle_cos );
}

inline float4x4 mix_transformations(
	float4x4 const&		first,
	float4x4 const&		second,
	const float			position_coeff,
	const float			orientation_coeff
)
{
	float3 const			translation	= first.c.xyz( ) * position_coeff + second.c.xyz( ) * ( 1.f - position_coeff );
	math::quaternion const	rotation	= ::slerp_optimized( math::quaternion( first ), math::quaternion( second ), orientation_coeff );
	return					math::create_matrix( rotation, translation );
}

inline float4x4 mix_transformations( float4x4 const& first, float4x4 const& second, float coeff )
{
	return mix_transformations( first, second, coeff, coeff );
}


} // namespace survarium

#endif // #ifndef IK_UTILS_H_INCLUDED
