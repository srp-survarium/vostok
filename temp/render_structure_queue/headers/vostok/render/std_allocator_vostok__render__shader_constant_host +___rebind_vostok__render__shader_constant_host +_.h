////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< shader_constant_host* >::rebind< shader_constant_host* > {
}; // struct std_allocator< shader_constant_host* >::rebind< shader_constant_host* >

STATIC_SIZE_ASSERT(std_allocator< shader_constant_host* >::rebind< shader_constant_host* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
