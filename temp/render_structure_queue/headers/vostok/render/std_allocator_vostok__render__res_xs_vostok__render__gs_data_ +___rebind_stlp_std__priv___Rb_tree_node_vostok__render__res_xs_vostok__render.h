////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< res_xs< gs_data >* >::rebind< std::priv::_Rb_tree_node< res_xs< gs_data >* > > {
}; // struct std_allocator< res_xs< gs_data >* >::rebind< std::priv::_Rb_tree_node< res_xs< gs_data >* > >

STATIC_SIZE_ASSERT(std_allocator< res_xs< gs_data >* >::rebind< std::priv::_Rb_tree_node< res_xs< gs_data >* > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
