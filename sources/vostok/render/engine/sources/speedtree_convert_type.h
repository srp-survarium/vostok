////////////////////////////////////////////////////////////////////////////
//	Created 	: 08.02.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_SPEEDTREE_CONVERT_TYPE_H_INCLUDED
#define VOSTOK_RENDER_SPEEDTREE_CONVERT_TYPE_H_INCLUDED

#include <vostok/render/facade/model.h>

namespace SpeedTree {
	class Vec3;
	class Mat4x4;
}

namespace vostok {
namespace render {

SpeedTree::Vec3		vostok_to_speedtree (math::float3 const& v);
SpeedTree::Vec4		vostok_to_speedtree (math::float4 const& v);
SpeedTree::Mat4x4	vostok_to_speedtree (math::float4x4 const& m);

math::float3		speedtree_to_vostok (SpeedTree::Vec3 const& v);
math::float4		speedtree_to_vostok (SpeedTree::Vec4 const& v);
math::float4x4		speedtree_to_vostok (SpeedTree::Mat4x4 const& m);

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_SPEEDTREE_CONVERT_TYPE_H_INCLUDED