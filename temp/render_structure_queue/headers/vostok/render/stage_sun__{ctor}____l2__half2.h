////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_SUN_{CTOR}___L2_HALF2_H_INCLUDED
#define RENDER_STAGE_SUN_{CTOR}___L2_HALF2_H_INCLUDED

/* INCLUDES */
class vostok::math::half;

namespace vostok {
namespace render {

struct stage_sun::{ctor}::__l2::half2 {
	inline		half2	( ) { /* no source */ }

	/* 0x0000 */	math::half		x;
	/* 0x0002 */	math::half		y;
}; // struct stage_sun::{ctor}::__l2::half2

STATIC_SIZE_ASSERT(stage_sun::{ctor}::__l2::half2, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_SUN_{CTOR}___L2_HALF2_H_INCLUDED
