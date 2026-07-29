////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER__DDCOLORKEY_H_INCLUDED
#define RENDER__DDCOLORKEY_H_INCLUDED

namespace vostok {
namespace render {

struct _DDCOLORKEY {

	/* 0x0000 */	unsigned long		dwColorSpaceLowValue;
	/* 0x0004 */	unsigned long		dwColorSpaceHighValue;
}; // struct _DDCOLORKEY

STATIC_SIZE_ASSERT(_DDCOLORKEY, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER__DDCOLORKEY_H_INCLUDED
