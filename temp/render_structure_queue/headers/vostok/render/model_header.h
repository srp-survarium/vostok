////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MODEL_HEADER_H_INCLUDED
#define RENDER_MODEL_HEADER_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb;
class vostok::math::sphere;

namespace vostok {
namespace render {

struct model_header {
	inline		model_header( ) { /* no source */ }

	/* 0x0000 */	u8				platform_id;
	/* 0x0001 */	u8				type;
	/* 0x0002 */	u16				version;
	/* 0x0004 */	math::aabb		bb;
	/* 0x001c */	math::sphere	bs;
}; // struct model_header

STATIC_SIZE_ASSERT(model_header, 0x2C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MODEL_HEADER_H_INCLUDED
