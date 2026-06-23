////////////////////////////////////////////////////////////////////////////
//	Created		: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_FACADE_AMBIENT_VOLUME_PROPERTIES_H_INCLUDED
#define VOSTOK_RENDER_FACADE_AMBIENT_VOLUME_PROPERTIES_H_INCLUDED

namespace vostok {
namespace render {

struct ambient_volume_properties
{
	math::float4x4	transform;
	float			ambient_multiplier;
	bool			enabled;
}; // struct ambient_volume_properties

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_AMBIENT_VOLUME_PROPERTIES_H_INCLUDED
