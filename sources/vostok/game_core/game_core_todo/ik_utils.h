////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef IK_UTILS_H_INCLUDED
#define IK_UTILS_H_INCLUDED

namespace survarium {

// STATE[STUB]
// vostok::math::float4x4 survarium::mix_transformations(vostok::math::float4x4 const&, vostok::math::float4x4 const&, const float, const float)
vostok::math::float4x4 mix_transformations(
	vostok::math::float4x4 const&		first,
	vostok::math::float4x4 const&		second,
	float								position_coeff,
	float								orientation_coeff)
{
	return vostok::math::float4x4();
	// FUNCTION BODY
	// <0xacd09>|0x000|0x000:'23'
	// <0xacd42>|0x039|0x039:'24'
	// <0xacd9b>|0x092|0x059:'25'
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::mix_transformations(vostok::math::float4x4 const&, vostok::math::float4x4 const&, const float)
vostok::math::float4x4 mix_transformations( vostok::math::float4x4 const& first, vostok::math::float4x4 const& second, float coeff )
{
	return vostok::math::float4x4();
	// FUNCTION BODY
	// <0xacdc0>|0x000|0x000:'30'
	// ******
}

} // namespace survarium

#endif // #ifndef IK_UTILS_H_INCLUDED
