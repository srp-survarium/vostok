////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< std::pair< post_process_parameters*, fs_new::virtual_path_string > >::rebind< std::priv::_Rb_tree_node< std::pair< post_process_parameters* const, fs_new::virtual_path_string > > > {
}; // struct std_allocator< std::pair< post_process_parameters*, fs_new::virtual_path_string > >::rebind< std::priv::_Rb_tree_node< std::pair< post_process_parameters* const, fs_new::virtual_path_string > > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< post_process_parameters*, fs_new::virtual_path_string > >::rebind< std::priv::_Rb_tree_node< std::pair< post_process_parameters* const, fs_new::virtual_path_string > > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
