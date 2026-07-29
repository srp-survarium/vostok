////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER__DDPIXELFORMAT___H_INCLUDED
#define RENDER__DDPIXELFORMAT___H_INCLUDED

namespace vostok {
namespace render {

struct _DDPIXELFORMAT::< unnamed-tag >::< unnamed-type-MultiSampleCaps > {

	/* 0x0000 */	u16		wFlipMSTypes;
	/* 0x0002 */	u16		wBltMSTypes;
}; // struct _DDPIXELFORMAT::< unnamed-tag >::< unnamed-type-MultiSampleCaps >

STATIC_SIZE_ASSERT(_DDPIXELFORMAT::< unnamed-tag >::< unnamed-type-MultiSampleCaps >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER__DDPIXELFORMAT___H_INCLUDED
