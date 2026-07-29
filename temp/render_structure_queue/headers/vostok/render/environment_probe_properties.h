////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENVIRONMENT_PROBE_PROPERTIES_H_INCLUDED
#define RENDER_ENVIRONMENT_PROBE_PROPERTIES_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<260>;

namespace vostok {
namespace render {

struct environment_probe_properties {
	inline		environment_probe_properties( ) { /* no source */ }

	/* 0x0000 */	fixed_string< 260 >		texture_name;
	/* 0x0110 */	float4x4				transform;
	/* 0x0150 */	float3					location;
	/* 0x015c */	float					radius;
	/* 0x0160 */	float					diffuse_multiplier;
	/* 0x0164 */	float					specular_multiplier;
	/* 0x0168 */	u32						preview_mip;
	/* 0x016c */	u32						cubemap_resolution;
	/* 0x0170 */	bool					texture_invalidated;
	/* 0x0171 */	bool					enabled;
	/* 0x0172 */	bool					clip_by_normal;
	/* 0x0173 */	bool					with_shadows;
	/* 0x0174 */	u32						geometry;
}; // struct environment_probe_properties

STATIC_SIZE_ASSERT(environment_probe_properties, 0x178);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENVIRONMENT_PROBE_PROPERTIES_H_INCLUDED
