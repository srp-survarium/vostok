////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER__DDSCAPS2_H_INCLUDED
#define RENDER__DDSCAPS2_H_INCLUDED

namespace vostok {
namespace render {

struct _DDSCAPS2 {

	/* 0x0000 */	unsigned long		dwCaps;
	/* 0x0004 */	unsigned long		dwCaps2;
	/* 0x0008 */	unsigned long		dwCaps3;
	/* 0x000c */	unsigned long		dwCaps4;
	/* 0x000c */	unsigned long		dwVolumeDepth;
}; // struct _DDSCAPS2

STATIC_SIZE_ASSERT(_DDSCAPS2, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER__DDSCAPS2_H_INCLUDED
