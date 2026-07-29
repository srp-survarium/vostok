////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SUN_CASCADE_H_INCLUDED
#define RENDER_SUN_CASCADE_H_INCLUDED

/* INCLUDES */
class vostok::fixed_vector<vostok::render::ray,8>;

namespace vostok {
namespace render {

struct sun_cascade {
	inline		sun_cascade	( ) { /* no source */ }
	inline		~sun_cascade( ) { /* no source */ }

	/* 0x0000 */	float4x4					transform;
	/* 0x0040 */	fixed_vector< ray, 8 >		rays;
	/* 0x0108 */	float						size;
	/* 0x010c */	float						bias;
	/* 0x0110 */	bool						reset_chain;
}; // struct sun_cascade

STATIC_SIZE_ASSERT(sun_cascade, 0x114);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SUN_CASCADE_H_INCLUDED
