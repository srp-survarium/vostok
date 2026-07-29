////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< render_model_instance_impl* >::rebind< void* > {
}; // struct std_allocator< render_model_instance_impl* >::rebind< void* >

STATIC_SIZE_ASSERT(std_allocator< render_model_instance_impl* >::rebind< void* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
