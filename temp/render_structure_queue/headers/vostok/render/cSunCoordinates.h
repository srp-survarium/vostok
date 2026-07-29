////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CSUNCOORDINATES_H_INCLUDED
#define RENDER_CSUNCOORDINATES_H_INCLUDED

namespace vostok {
namespace render {

struct cSunCoordinates {

	/* 0x0000 */	float		dZenithAngle;
	/* 0x0004 */	float		dAzimuth;
}; // struct cSunCoordinates

STATIC_SIZE_ASSERT(cSunCoordinates, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CSUNCOORDINATES_H_INCLUDED
