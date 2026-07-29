////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERT_BONED_1W_H_INCLUDED
#define RENDER_VERT_BONED_1W_H_INCLUDED

namespace vostok {
namespace render {

struct vert_boned_1w {
	inline	void	invalidate		( ) { /* no source */ }

	inline			vert_boned_1w	( ) { /* no source */ }

	/* 0x0000 */	float3		P;
	/* 0x000c */	float3		N;
	/* 0x0018 */	float3		T;
	/* 0x0024 */	float3		B;
	/* 0x0030 */	float2		uv;
	/* 0x0038 */	u16			m;
}; // struct vert_boned_1w

STATIC_SIZE_ASSERT(vert_boned_1w, 0x3A);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERT_BONED_1W_H_INCLUDED
