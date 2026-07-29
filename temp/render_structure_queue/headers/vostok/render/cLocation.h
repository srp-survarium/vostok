////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CLOCATION_H_INCLUDED
#define RENDER_CLOCATION_H_INCLUDED

namespace vostok {
namespace render {

struct cLocation {

	/* 0x0000 */	float		dLongitude;
	/* 0x0004 */	float		dLatitude;
}; // struct cLocation

STATIC_SIZE_ASSERT(cLocation, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CLOCATION_H_INCLUDED
