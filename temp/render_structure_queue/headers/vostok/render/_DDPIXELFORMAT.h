////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER__DDPIXELFORMAT_H_INCLUDED
#define RENDER__DDPIXELFORMAT_H_INCLUDED

/* INCLUDES */
struct vostok::render::_DDPIXELFORMAT::<unnamed-tag>::<unnamed-type-MultiSampleCaps>;

namespace vostok {
namespace render {

struct _DDPIXELFORMAT {

	/* 0x0000 */	unsigned long		dwSize;
	/* 0x0004 */	unsigned long		dwFlags;
	/* 0x0008 */	unsigned long		dwFourCC;
	/* 0x000c */	unsigned long		dwRGBBitCount;
	/* 0x000c */	unsigned long		dwYUVBitCount;
	/* 0x000c */	unsigned long		dwZBufferBitDepth;
	/* 0x000c */	unsigned long		dwAlphaBitDepth;
	/* 0x000c */	unsigned long		dwLuminanceBitCount;
	/* 0x000c */	unsigned long		dwBumpBitCount;
	/* 0x000c */	unsigned long		dwPrivateFormatBitCount;
	/* 0x0010 */	unsigned long		dwRBitMask;
	/* 0x0010 */	unsigned long		dwYBitMask;
	/* 0x0010 */	unsigned long		dwStencilBitDepth;
	/* 0x0010 */	unsigned long		dwLuminanceBitMask;
	/* 0x0010 */	unsigned long		dwBumpDuBitMask;
	/* 0x0010 */	unsigned long		dwOperations;
	/* 0x0014 */	unsigned long		dwGBitMask;
	/* 0x0014 */	unsigned long		dwUBitMask;
	/* 0x0014 */	unsigned long		dwZBitMask;
	/* 0x0014 */	unsigned long		dwBumpDvBitMask;
	/* 0x0014 */	_DDPIXELFORMAT::< unnamed-tag >::< unnamed-type-MultiSampleCaps >	MultiSampleCaps;
	/* 0x0018 */	unsigned long		dwBBitMask;
	/* 0x0018 */	unsigned long		dwVBitMask;
	/* 0x0018 */	unsigned long		dwStencilBitMask;
	/* 0x0018 */	unsigned long		dwBumpLuminanceBitMask;
	/* 0x001c */	unsigned long		dwRGBAlphaBitMask;
	/* 0x001c */	unsigned long		dwYUVAlphaBitMask;
	/* 0x001c */	unsigned long		dwLuminanceAlphaBitMask;
	/* 0x001c */	unsigned long		dwRGBZBitMask;
	/* 0x001c */	unsigned long		dwYUVZBitMask;
}; // struct _DDPIXELFORMAT

STATIC_SIZE_ASSERT(_DDPIXELFORMAT, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER__DDPIXELFORMAT_H_INCLUDED
