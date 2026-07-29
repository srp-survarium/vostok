////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_SECTOR_DOUBLE_QUERY_PREVENTER_FRUSTUM_IMAGE_H_INCLUDED
#define RENDER_CULLING_SECTOR_DOUBLE_QUERY_PREVENTER_FRUSTUM_IMAGE_H_INCLUDED

/* INCLUDES */
class vostok::math::color;

namespace vostok {
namespace render {
namespace culling {

struct sector_double_query_preventer::frustum_image {
	inline		frustum_image	( ) { /* no source */ }

	/* 0x0000 */	float3			vertices[8];
	/* 0x0060 */	math::color		c;
}; // struct sector_double_query_preventer::frustum_image

STATIC_SIZE_ASSERT(sector_double_query_preventer::frustum_image, 0x64);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_SECTOR_DOUBLE_QUERY_PREVENTER_FRUSTUM_IMAGE_H_INCLUDED
