////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_FRAME_HISTOGRAM_INFO_H_INCLUDED
#define RENDER_FRAME_HISTOGRAM_INFO_H_INCLUDED

/* INCLUDES */
struct vostok::render::frame_histogram_info;

namespace vostok {
namespace render {

struct frame_histogram_info {

	/* 0x0000 */	float						time;
	/* 0x0004 */	float						mem;
	/* 0x0008 */	u32							dips;
	/* 0x000c */	frame_histogram_info*		next;
}; // struct frame_histogram_info

STATIC_SIZE_ASSERT(frame_histogram_info, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_FRAME_HISTOGRAM_INFO_H_INCLUDED
