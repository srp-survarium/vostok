////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LPV_VERTEX_H_INCLUDED
#define RENDER_LPV_VERTEX_H_INCLUDED

/* INCLUDES */
class vostok::math::color;

namespace vostok {
namespace render {

struct lpv_vertex {
	inline		lpv_vertex	( ) { /* no source */ }

	/* 0x0000 */	float3			position;
	/* 0x000c */	math::color		normal;
	/* 0x0010 */	math::color		clr;
}; // struct lpv_vertex

STATIC_SIZE_ASSERT(lpv_vertex, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LPV_VERTEX_H_INCLUDED
