////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< res_input_layout* >::rebind< std::priv::_Rb_tree_node< res_input_layout* > > {
}; // struct std_allocator< res_input_layout* >::rebind< std::priv::_Rb_tree_node< res_input_layout* > >

STATIC_SIZE_ASSERT(std_allocator< res_input_layout* >::rebind< std::priv::_Rb_tree_node< res_input_layout* > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
