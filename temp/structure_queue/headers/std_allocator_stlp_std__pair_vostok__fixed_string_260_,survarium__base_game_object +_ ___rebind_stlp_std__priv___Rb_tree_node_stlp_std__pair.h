////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< std::pair< fixed_string< 260 >, base_game_object* > >::rebind< std::priv::_Rb_tree_node< std::pair< fixed_string< 260 > const , base_game_object* > > > {
}; // struct std_allocator< std::pair< fixed_string< 260 >, base_game_object* > >::rebind< std::priv::_Rb_tree_node< std::pair< fixed_string< 260 > const , base_game_object* > > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< fixed_string< 260 >, base_game_object* > >::rebind< std::priv::_Rb_tree_node< std::pair< fixed_string< 260 > const , base_game_object* > > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
