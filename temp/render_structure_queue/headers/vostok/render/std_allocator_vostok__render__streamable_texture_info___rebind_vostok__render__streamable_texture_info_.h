////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< streamable_texture_info >::rebind< streamable_texture_info > {
}; // struct std_allocator< streamable_texture_info >::rebind< streamable_texture_info >

STATIC_SIZE_ASSERT(std_allocator< streamable_texture_info >::rebind< streamable_texture_info >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
