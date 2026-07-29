////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< res_shader_technique* >::rebind< std::priv::_Rb_tree_node< res_shader_technique* > > {
}; // struct std_allocator< res_shader_technique* >::rebind< std::priv::_Rb_tree_node< res_shader_technique* > >

STATIC_SIZE_ASSERT(std_allocator< res_shader_technique* >::rebind< std::priv::_Rb_tree_node< res_shader_technique* > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
