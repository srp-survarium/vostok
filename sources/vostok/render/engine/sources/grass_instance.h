#ifndef VOSTOK_RENDER_ENGINE_GRASS_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_GRASS_INSTANCE_H_INCLUDED

#include <vostok/math_color.h>
#include <vostok/math_float4x4.h>

namespace vostok {
namespace render {

struct grass_template;

struct grass_instance {
	grass_instance(
		u32 in_id,
		grass_template* in_template,
		math::color const& in_color,
		float4x4 const& in_transform,
		u8 in_layer,
		float in_wind_scale
	);

	grass_template* m_template;
	math::color m_color;
	float4x4 m_transform;
	float m_wind_scale;
	u32 m_index;
	u8 m_layer_id;
};

STATIC_SIZE_ASSERT( grass_instance, 0x54 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_GRASS_INSTANCE_H_INCLUDED
