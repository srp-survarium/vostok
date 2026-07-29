////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CREATE_COLOR_GRADING_BASE_LUT___L2_PIXEL_H_INCLUDED
#define RENDER_CREATE_COLOR_GRADING_BASE_LUT___L2_PIXEL_H_INCLUDED

namespace vostok {
namespace render {

struct create_color_grading_base_lut::__l2::pixel {

	/* 0x0000 */	u32		clr;
	/* 0x0000 */	u8		r;
	/* 0x0001 */	u8		g;
	/* 0x0002 */	u8		b;
	/* 0x0003 */	u8		a;
}; // struct create_color_grading_base_lut::__l2::pixel

STATIC_SIZE_ASSERT(create_color_grading_base_lut::__l2::pixel, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CREATE_COLOR_GRADING_BASE_LUT___L2_PIXEL_H_INCLUDED
