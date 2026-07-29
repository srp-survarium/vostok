////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKY_AMBIENT_OCCLUSION_PROPERTIES_H_INCLUDED
#define RENDER_SKY_AMBIENT_OCCLUSION_PROPERTIES_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<260>;

namespace vostok {
namespace render {

struct sky_ambient_occlusion_properties {
	inline		sky_ambient_occlusion_properties( ) { /* no source */ }

	/* 0x0000 */	fixed_string< 260 >		texture_name;
	/* 0x0110 */	float3					location;
	/* 0x011c */	float					width;
	/* 0x0120 */	float					height;
	/* 0x0124 */	float					depth;
	/* 0x0128 */	bool					enabled;
	/* 0x0129 */	bool					texture_invalidated;
}; // struct sky_ambient_occlusion_properties

STATIC_SIZE_ASSERT(sky_ambient_occlusion_properties, 0x12C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKY_AMBIENT_OCCLUSION_PROPERTIES_H_INCLUDED
