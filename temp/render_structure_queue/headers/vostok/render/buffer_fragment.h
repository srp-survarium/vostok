////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BUFFER_FRAGMENT_H_INCLUDED
#define RENDER_BUFFER_FRAGMENT_H_INCLUDED

namespace vostok {
namespace render {

struct buffer_fragment {

	/* 0x0000 */	u32			start;
	/* 0x0004 */	u32			size;
	/* 0x0008 */	void*		buffer;
}; // struct buffer_fragment

STATIC_SIZE_ASSERT(buffer_fragment, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BUFFER_FRAGMENT_H_INCLUDED
