////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< res_pass* >::rebind< std::priv::_Rb_tree_node< res_pass* > > {
}; // struct std_allocator< res_pass* >::rebind< std::priv::_Rb_tree_node< res_pass* > >

STATIC_SIZE_ASSERT(std_allocator< res_pass* >::rebind< std::priv::_Rb_tree_node< res_pass* > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
