////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< fixed_string< 260 > >::rebind< fixed_string< 260 > > {
}; // struct std_allocator< fixed_string< 260 > >::rebind< fixed_string< 260 > >

STATIC_SIZE_ASSERT(std_allocator< fixed_string< 260 > >::rebind< fixed_string< 260 > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
