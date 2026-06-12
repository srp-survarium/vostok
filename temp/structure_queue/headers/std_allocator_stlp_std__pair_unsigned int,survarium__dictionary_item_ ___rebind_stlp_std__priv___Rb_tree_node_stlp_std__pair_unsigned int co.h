////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< std::pair< u32, dictionary_item > >::rebind< std::priv::_Rb_tree_node< std::pair< u32 const , dictionary_item > > > {
}; // struct std_allocator< std::pair< u32, dictionary_item > >::rebind< std::priv::_Rb_tree_node< std::pair< u32 const , dictionary_item > > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< u32, dictionary_item > >::rebind< std::priv::_Rb_tree_node< std::pair< u32 const , dictionary_item > > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
