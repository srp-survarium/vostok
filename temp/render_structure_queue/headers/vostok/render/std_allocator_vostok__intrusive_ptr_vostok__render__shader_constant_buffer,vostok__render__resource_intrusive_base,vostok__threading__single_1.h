////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< shader_constant_buffer_ptr >::rebind< intrusive_ptr< shader_constant_buffer, resource_intrusive_base, threading::single_threading_policy > > {
}; // struct std_allocator< shader_constant_buffer_ptr >::rebind< intrusive_ptr< shader_constant_buffer, resource_intrusive_base, threading::single_threading_policy > >

STATIC_SIZE_ASSERT(std_allocator< shader_constant_buffer_ptr >::rebind< intrusive_ptr< shader_constant_buffer, resource_intrusive_base, threading::single_threading_policy > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
