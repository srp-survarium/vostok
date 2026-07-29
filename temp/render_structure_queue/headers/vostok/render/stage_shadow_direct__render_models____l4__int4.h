////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_SHADOW_DIRECT_RENDER_MODELS___L4_INT4_H_INCLUDED
#define RENDER_STAGE_SHADOW_DIRECT_RENDER_MODELS___L4_INT4_H_INCLUDED

namespace vostok {
namespace render {

struct stage_shadow_direct::render_models::__l4::int4 {
	inline		int4(
					s32		arg_0,
					s32		arg_1,
					s32		arg_2,
					s32		arg_3
				) { /* no source */ }

	/* 0x0000 */	s32		x;
	/* 0x0004 */	s32		y;
	/* 0x0008 */	s32		z;
	/* 0x000c */	s32		w;
}; // struct stage_shadow_direct::render_models::__l4::int4

STATIC_SIZE_ASSERT(stage_shadow_direct::render_models::__l4::int4, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_SHADOW_DIRECT_RENDER_MODELS___L4_INT4_H_INCLUDED
