// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_SPEEDTREE_CONVERT_TYPE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SPEEDTREE_CONVERT_TYPE_H_INCLUDED
#include <speedtree/core/core.h>

#include <vostok/math_float3.h>
#include <vostok/math_float4.h>
#include <vostok/math_float4x4.h>

namespace vostok {
namespace render {

SpeedTree::Vec3 vostok_to_speedtree( float3 const& value );
SpeedTree::Mat4x4 vostok_to_speedtree( float4x4 const& value );

float3 speedtree_to_vostok( SpeedTree::Vec3 const& value );
float4 speedtree_to_vostok( SpeedTree::Vec4 const& value );
float4x4 speedtree_to_vostok( SpeedTree::Mat4x4 const& value );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_SPEEDTREE_CONVERT_TYPE_H_INCLUDED
