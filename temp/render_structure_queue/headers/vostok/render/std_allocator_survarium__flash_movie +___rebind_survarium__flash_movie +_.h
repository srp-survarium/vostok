////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< survarium::flash_movie* >::rebind< survarium::flash_movie* > {
}; // struct std_allocator< survarium::flash_movie* >::rebind< survarium::flash_movie* >

STATIC_SIZE_ASSERT(std_allocator< survarium::flash_movie* >::rebind< survarium::flash_movie* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
