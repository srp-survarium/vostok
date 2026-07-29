////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER__DDSURFACEDESC2_H_INCLUDED
#define RENDER__DDSURFACEDESC2_H_INCLUDED

/* INCLUDES */
struct vostok::render::_DDCOLORKEY;
struct vostok::render::_DDPIXELFORMAT;
struct vostok::render::_DDSCAPS2;

namespace vostok {
namespace render {

struct _DDSURFACEDESC2 {

	/* 0x0000 */	unsigned long		dwSize;
	/* 0x0004 */	unsigned long		dwFlags;
	/* 0x0008 */	unsigned long		dwHeight;
	/* 0x000c */	unsigned long		dwWidth;
	/* 0x0010 */	long				lPitch;
	/* 0x0010 */	unsigned long		dwLinearSize;
	/* 0x0014 */	unsigned long		dwBackBufferCount;
	/* 0x0014 */	unsigned long		dwDepth;
	/* 0x0018 */	unsigned long		dwMipMapCount;
	/* 0x0018 */	unsigned long		dwRefreshRate;
	/* 0x0018 */	unsigned long		dwSrcVBHandle;
	/* 0x001c */	unsigned long		dwAlphaBitDepth;
	/* 0x0020 */	unsigned long		dwReserved;
	/* 0x0024 */	void*				lpSurface;
	/* 0x0028 */	_DDCOLORKEY			ddckCKDestOverlay;
	/* 0x0028 */	unsigned long		dwEmptyFaceColor;
	/* 0x0030 */	_DDCOLORKEY			ddckCKDestBlt;
	/* 0x0038 */	_DDCOLORKEY			ddckCKSrcOverlay;
	/* 0x0040 */	_DDCOLORKEY			ddckCKSrcBlt;
	/* 0x0048 */	_DDPIXELFORMAT		ddpfPixelFormat;
	/* 0x0048 */	unsigned long		dwFVF;
	/* 0x0068 */	_DDSCAPS2			ddsCaps;
	/* 0x0078 */	unsigned long		dwTextureStage;
}; // struct _DDSURFACEDESC2

STATIC_SIZE_ASSERT(_DDSURFACEDESC2, 0x7C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER__DDSURFACEDESC2_H_INCLUDED
