////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERT_BONED_4W_H_INCLUDED
#define RENDER_VERT_BONED_4W_H_INCLUDED

namespace vostok {
namespace render {

struct vert_boned_4w {
	inline	void	invalidate		( ) { /* no source */ }

	inline			vert_boned_4w	( ) { /* no source */ }

	/* 0x0000 */	u16			m[4];
	/* 0x0008 */	float3		P;
	/* 0x0014 */	float3		N;
	/* 0x0020 */	float3		T;
	/* 0x002c */	float3		B;
	/* 0x0038 */	float		w[3];
	/* 0x0044 */	float2		uv;
}; // struct vert_boned_4w

STATIC_SIZE_ASSERT(vert_boned_4w, 0x4C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERT_BONED_4W_H_INCLUDED
