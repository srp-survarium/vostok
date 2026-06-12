////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< std::pair< u16, map< u16, material_pair const*, std::less< u16 > > > >::rebind< std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > > > {
}; // struct std_allocator< std::pair< u16, map< u16, material_pair const*, std::less< u16 > > > >::rebind< std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< u16, map< u16, material_pair const*, std::less< u16 > > > >::rebind< std::priv::_Rb_tree_node< std::pair< u16 const , map< u16, material_pair const*, std::less< u16 > > > > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
