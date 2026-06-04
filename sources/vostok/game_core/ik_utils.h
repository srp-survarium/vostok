////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef IK_UTILS_H_INCLUDED
#define IK_UTILS_H_INCLUDED

namespace survarium {

// STATE[72.58%|PARTIAL]: law of cosines, logic byte-exact. Residual is inline-vs-call of
// vostok::math::acos: the target keeps the out-of-line call vostok::math::acos (smaller frame
// sub esp,14h), our /GL LTCG inlines it to the _CIacos intrinsic with extra movss/fld slot
// round-trips (sub esp,1Ch). acos has a standalone symbol in BOTH indexes (target 0x27c80,
// base 0x37930) -> per-call-site whole-program inline decision, not steerable from this body
// (documented inline-vs-call LTCG class). Source is correct; not a clean DONE. See md.
inline float get_angle( float adjacent0, float adjacent1, float opposite )
{
	float angle_cos	= ( vostok::math::sqr( adjacent0 ) + vostok::math::sqr( adjacent1 ) - vostok::math::sqr( opposite ) ) / ( 2.0f * adjacent0 * adjacent1 );
	vostok::math::clamp( angle_cos, -1.0f, 1.0f );
	return vostok::math::acos( angle_cos );

	// FUNCTION BODY
	// <0xcafd6>|0x006|+0x04f:'16'		angle_cos
	// <0xcb025>|0x055|+0x016:'17'		clamp
	// <0xcb03b>|0x06b|+0x00e:'18'		return acos
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::mix_transformations(vostok::math::float4x4 const&, vostok::math::float4x4 const&, const float, const float)
inline float4x4 mix_transformations(
	float4x4 const&		first,
	float4x4 const&		second,
	float				position_coeff,
	float				orientation_coeff
)
{
	return vostok::math::float4x4();

	// FUNCTION BODY
	// <0xacd09>|0x009|+0x039:'23'
	// <0xacd42>|0x042|+0x059:'24'
	// <0xacd9b>|0x09b|+0x00f:'25'
	// ******
}

// STATE[BLOCKED]: thin forwarder to the 4-arg overload (position_coeff == orientation_coeff).
// Source is plausibly correct, but the 4-arg target it forwards to is still a STUB (its real
// body is a quaternion slerp needing the file-local slerp_optimized from core/math_quaternion.cpp,
// no source in tree). Both overloads exist out-of-line in the TARGET (3-arg 0x9cdc0, 4-arg 0x9cd00)
// but NEITHER survives in the BASE (DSE'd whole-program because the stub collapses) -> report.json
// scores both None. Not DONE - no base bytes to score. Unblock = build the 4-arg body. See md.
inline float4x4 mix_transformations( float4x4 const& first, float4x4 const& second, float coeff )
{
	return mix_transformations( first, second, coeff, coeff );

	// FUNCTION BODY
	// <0xacdc0>|0x000|+0x01d:'30'
	// ******
}


} // namespace survarium

#endif // #ifndef IK_UTILS_H_INCLUDED
