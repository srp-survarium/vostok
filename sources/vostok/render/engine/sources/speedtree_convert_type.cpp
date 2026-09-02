// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "speedtree_convert_type.h"
namespace vostok {
namespace render {

SpeedTree::Vec3 vostok_to_speedtree( float3 const& value )
{
	return SpeedTree::Vec3( value.elements );
}

SpeedTree::Mat4x4 vostok_to_speedtree( float4x4 const& value )
{
	SpeedTree::Mat4x4 result;
	memory::copy(
		&result.m_afRowCol[0],
		sizeof( result.m_afRowCol ),
		&value.elements[0],
		sizeof( value.elements )
	);
	return result;
}

float3 speedtree_to_vostok( SpeedTree::Vec3 const& value )
{
	return float3( value.x, value.y, value.z );
}

float4 speedtree_to_vostok( SpeedTree::Vec4 const& value )
{
	return float4( value.x, value.y, value.z, value.w );
}

float4x4 speedtree_to_vostok( SpeedTree::Mat4x4 const& value )
{
	float4x4 result;
	memory::copy(
		&result.elements[0],
		sizeof( result.elements ),
		&value.m_afRowCol[0],
		sizeof( value.m_afRowCol )
	);
	return result;
}

} // namespace render
} // namespace vostok
