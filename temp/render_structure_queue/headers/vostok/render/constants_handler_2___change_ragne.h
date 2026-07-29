////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CONSTANTS_HANDLER_CHANGE_RAGNE_H_INCLUDED
#define RENDER_CONSTANTS_HANDLER_CHANGE_RAGNE_H_INCLUDED

namespace vostok {
namespace render {

struct constants_handler< 2 >::change_ragne {
	inline		change_ragne( ) { /* no source */ }

	/* 0x0000 */	u32		start;
	/* 0x0004 */	u32		end;
}; // struct constants_handler< 2 >::change_ragne

STATIC_SIZE_ASSERT(constants_handler< 2 >::change_ragne, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CONSTANTS_HANDLER_CHANGE_RAGNE_H_INCLUDED
