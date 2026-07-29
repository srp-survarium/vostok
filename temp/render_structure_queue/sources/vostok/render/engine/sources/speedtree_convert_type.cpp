////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\speedtree_convert_type.h"

namespace vostok {
namespace render {

// STATE[STUB]
SpeedTree::Vec3 vostok_to_speedtree( float3 const& v )
{
	// FUNCTION BODY[0x709580]: 1
	// <0x709580>|0x000|+0x010:'15'
	// ******
}

// STATE[STUB]
SpeedTree::Mat4x4 vostok_to_speedtree( float4x4 const& m )
{
	// FUNCTION BODY[0x7095b0]: 3
	// <0x7095b0>|0x000|+0x00a:'25'
	// <0x7095ba>|0x00a|+0x02b:'26'
	// <0x7095e5>|0x035|+0x002:'27'
	// ******
}

// STATE[STUB]
float3 speedtree_to_vostok( SpeedTree::Vec3 const& v )
{
	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0x709560]: 1
	// <0x709560>|0x000|+0x010:'32'
	// ******
}

// STATE[STUB]
float4 speedtree_to_vostok( SpeedTree::Vec4 const& v )
{
	return vostok::math::float4(1., 1., 1., 1.);

	// FUNCTION BODY[0x709540]: 1
	// <0x709540>|0x000|+0x016:'37'
	// ******
}

// STATE[STUB]
float4x4 speedtree_to_vostok( SpeedTree::Mat4x4 const& m )
{
	return vostok::math::float4x4();

	// FUNCTION BODY[0x7095a0]: 3
	// <0>
	// <0x7095a0>|0x000|+0x00c:'43'
	// <0x7095ac>|0x00c|+0x002:'44'
	// ******
}

} // namespace render
} // namespace vostok
