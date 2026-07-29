////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TRAMPLE_DESC_H_INCLUDED
#define RENDER_TRAMPLE_DESC_H_INCLUDED

namespace vostok {
namespace render {

struct trample_desc {
	inline		trample_desc( ) { /* no source */ }

	/* 0x0000 */	float3		position;
	/* 0x000c */	float		radius;
	/* 0x0010 */	float		multiplier;
}; // struct trample_desc

STATIC_SIZE_ASSERT(trample_desc, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TRAMPLE_DESC_H_INCLUDED
