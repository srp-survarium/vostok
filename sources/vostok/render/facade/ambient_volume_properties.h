#ifndef VOSTOK_RENDER_FACADE_AMBIENT_VOLUME_PROPERTIES_H_INCLUDED
#define VOSTOK_RENDER_FACADE_AMBIENT_VOLUME_PROPERTIES_H_INCLUDED

#include <vostok/math_float4x4.h>

namespace vostok {
namespace render {

struct ambient_volume_properties {
	float4x4	transform;
	float		ambient_multiplier;
	bool		enabled;
};

STATIC_SIZE_ASSERT( ambient_volume_properties, 0x48 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_AMBIENT_VOLUME_PROPERTIES_H_INCLUDED
