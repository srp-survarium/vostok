////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef IK_UTILS_H_INCLUDED
#define IK_UTILS_H_INCLUDED

namespace survarium {

// STATE[STUB]
inline float get_angle( float adjacent0, float adjacent1, float opposite )
{
	// LOCALS
	// float 						angle_cos
	// ******

	return 0.0f;

	// FUNCTION BODY
	// <0xcafd6>|0x006|+0x04f:'16'
	// <0xcb025>|0x055|+0x016:'17'
	// <0xcb03b>|0x06b|+0x00e:'18'
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

// STATE[STUB]
// vostok::math::float4x4 survarium::mix_transformations(vostok::math::float4x4 const&, vostok::math::float4x4 const&, const float)
inline float4x4 mix_transformations( float4x4 const& first, float4x4 const& second, float coeff )
{
	return vostok::math::float4x4();

	// FUNCTION BODY
	// <0xacdc0>|0x000|+0x01d:'30'
	// ******
}


} // namespace survarium

#endif // #ifndef IK_UTILS_H_INCLUDED
