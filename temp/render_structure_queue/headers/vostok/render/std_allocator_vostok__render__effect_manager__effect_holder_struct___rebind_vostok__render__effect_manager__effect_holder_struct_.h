////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< effect_manager::effect_holder_struct >::rebind< effect_manager::effect_holder_struct > {
}; // struct std_allocator< effect_manager::effect_holder_struct >::rebind< effect_manager::effect_holder_struct >

STATIC_SIZE_ASSERT(std_allocator< effect_manager::effect_holder_struct >::rebind< effect_manager::effect_holder_struct >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
