////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< shader_constant_buffer* >::rebind< std::priv::_Rb_tree_node< shader_constant_buffer* > > {
}; // struct std_allocator< shader_constant_buffer* >::rebind< std::priv::_Rb_tree_node< shader_constant_buffer* > >

STATIC_SIZE_ASSERT(std_allocator< shader_constant_buffer* >::rebind< std::priv::_Rb_tree_node< shader_constant_buffer* > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
