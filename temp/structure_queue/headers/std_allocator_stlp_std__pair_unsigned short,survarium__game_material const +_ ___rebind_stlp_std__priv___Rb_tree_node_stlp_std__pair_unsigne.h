////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< std::pair< u16, game_material const* > >::rebind< std::priv::_Rb_tree_node< std::pair< u16 const , game_material const* > > > {
}; // struct std_allocator< std::pair< u16, game_material const* > >::rebind< std::priv::_Rb_tree_node< std::pair< u16 const , game_material const* > > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< u16, game_material const* > >::rebind< std::priv::_Rb_tree_node< std::pair< u16 const , game_material const* > > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
