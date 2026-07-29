////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERT_BONED_3W_H_INCLUDED
#define RENDER_VERT_BONED_3W_H_INCLUDED

namespace vostok {
namespace render {

struct vert_boned_3w {
	inline	void	invalidate		( ) { /* no source */ }

	inline			vert_boned_3w	( ) { /* no source */ }

	/* 0x0000 */	u16			m[3];
	/* 0x0006 */	float3		P;
	/* 0x0012 */	float3		N;
	/* 0x001e */	float3		T;
	/* 0x002a */	float3		B;
	/* 0x0036 */	float		w[2];
	/* 0x003e */	float2		uv;
}; // struct vert_boned_3w

STATIC_SIZE_ASSERT(vert_boned_3w, 0x46);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERT_BONED_3W_H_INCLUDED
