////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< std::pair< resource_manager::shader_name_config_pair, res_xs_hw< vs_data >* > >::rebind< std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< vs_data >* > > > {
}; // struct std_allocator< std::pair< resource_manager::shader_name_config_pair, res_xs_hw< vs_data >* > >::rebind< std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< vs_data >* > > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< resource_manager::shader_name_config_pair, res_xs_hw< vs_data >* > >::rebind< std::priv::_Rb_tree_node< std::pair< resource_manager::shader_name_config_pair const , res_xs_hw< vs_data >* > > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
