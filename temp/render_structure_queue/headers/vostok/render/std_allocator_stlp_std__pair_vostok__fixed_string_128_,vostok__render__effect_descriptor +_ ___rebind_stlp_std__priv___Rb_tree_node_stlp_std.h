////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
#define RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED

namespace vostok {
namespace render {

struct std_allocator< std::pair< fixed_string< 128 >, effect_descriptor* > >::rebind< std::priv::_Rb_tree_node< std::pair< fixed_string< 128 > const , effect_descriptor* > > > {
}; // struct std_allocator< std::pair< fixed_string< 128 >, effect_descriptor* > >::rebind< std::priv::_Rb_tree_node< std::pair< fixed_string< 128 > const , effect_descriptor* > > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< fixed_string< 128 >, effect_descriptor* > >::rebind< std::priv::_Rb_tree_node< std::pair< fixed_string< 128 > const , effect_descriptor* > > >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_REBIND_H_INCLUDED
