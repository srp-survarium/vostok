////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_AMBIENT_VOLUME_PROPERTIES_H_INCLUDED
#define RENDER_AMBIENT_VOLUME_PROPERTIES_H_INCLUDED

namespace vostok {
namespace render {

struct ambient_volume_properties {
	inline		ambient_volume_properties	( ) { /* no source */ }

	/* 0x0000 */	float4x4	transform;
	/* 0x0040 */	float		ambient_multiplier;
	/* 0x0044 */	bool		enabled;
}; // struct ambient_volume_properties

STATIC_SIZE_ASSERT(ambient_volume_properties, 0x48);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_AMBIENT_VOLUME_PROPERTIES_H_INCLUDED
