////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< requested_streamable_texture >::rebind< requested_streamable_texture > {
}; // struct std_allocator< requested_streamable_texture >::rebind< requested_streamable_texture >

STATIC_SIZE_ASSERT(std_allocator< requested_streamable_texture >::rebind< requested_streamable_texture >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
