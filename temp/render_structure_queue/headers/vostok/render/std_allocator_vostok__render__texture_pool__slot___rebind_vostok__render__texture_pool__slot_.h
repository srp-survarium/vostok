////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< texture_pool::slot >::rebind< texture_pool::slot > {
}; // struct std_allocator< texture_pool::slot >::rebind< texture_pool::slot >

STATIC_SIZE_ASSERT(std_allocator< texture_pool::slot >::rebind< texture_pool::slot >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
