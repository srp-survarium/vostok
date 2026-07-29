////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADOW_CASCADE_VOLUME_POLYGON_H_INCLUDED
#define RENDER_SHADOW_CASCADE_VOLUME_POLYGON_H_INCLUDED

/* INCLUDES */
class vostok::math::plane;

namespace vostok {
namespace render {

struct shadow_cascade_volume::polygon {
	inline		polygon	( ) { /* no source */ }

	/* 0x0000 */	s32				points[4];
	/* 0x0010 */	math::plane		plane;
}; // struct shadow_cascade_volume::polygon

STATIC_SIZE_ASSERT(shadow_cascade_volume::polygon, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADOW_CASCADE_VOLUME_POLYGON_H_INCLUDED
