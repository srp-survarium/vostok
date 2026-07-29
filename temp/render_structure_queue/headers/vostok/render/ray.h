////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RAY_H_INCLUDED
#define RENDER_RAY_H_INCLUDED

namespace vostok {
namespace render {

struct ray {
	inline		ray	( ) { /* no source */ }
	inline		ray	( float3 const& arg_0, float3 const& arg_1 ) { /* no source */ }

	/* 0x0000 */	float3		direction;
	/* 0x000c */	float3		origin;
}; // struct ray

STATIC_SIZE_ASSERT(ray, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RAY_H_INCLUDED
