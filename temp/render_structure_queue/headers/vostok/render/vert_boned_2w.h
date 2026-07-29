////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VERT_BONED_2W_H_INCLUDED
#define RENDER_VERT_BONED_2W_H_INCLUDED

namespace vostok {
namespace render {

struct vert_boned_2w {
	inline	void	invalidate		( ) { /* no source */ }

	inline			vert_boned_2w	( ) { /* no source */ }

	/* 0x0000 */	u16			m[2];
	/* 0x0004 */	float3		P;
	/* 0x0010 */	float3		N;
	/* 0x001c */	float3		T;
	/* 0x0028 */	float3		B;
	/* 0x0034 */	float		w;
	/* 0x0038 */	float2		uv;
}; // struct vert_boned_2w

STATIC_SIZE_ASSERT(vert_boned_2w, 0x40);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VERT_BONED_2W_H_INCLUDED
