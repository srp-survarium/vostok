////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< std::pair< fs_new::virtual_path_string, res_texture* > >::rebind< std::priv::_Rb_tree_node< std::pair< fs_new::virtual_path_string const , res_texture* > > > {
}; // struct std_allocator< std::pair< fs_new::virtual_path_string, res_texture* > >::rebind< std::priv::_Rb_tree_node< std::pair< fs_new::virtual_path_string const , res_texture* > > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< fs_new::virtual_path_string, res_texture* > >::rebind< std::priv::_Rb_tree_node< std::pair< fs_new::virtual_path_string const , res_texture* > > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
