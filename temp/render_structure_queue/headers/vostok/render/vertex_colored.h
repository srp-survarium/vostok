////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERTEX_COLORED_H_INCLUDED
#define RENDER_VERTEX_COLORED_H_INCLUDED

/* INCLUDES */
class vostok::math::color;

namespace vostok {
namespace render {

struct vertex_colored {
	inline		vertex_colored	( ) { /* no source */ }
	inline		vertex_colored	( float3 const& arg_0, math::color const& arg_1 ) { /* no source */ }

	/* 0x0000 */	float3			position;
	/* 0x000c */	math::color		color;
}; // struct vertex_colored

STATIC_SIZE_ASSERT(vertex_colored, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERTEX_COLORED_H_INCLUDED
