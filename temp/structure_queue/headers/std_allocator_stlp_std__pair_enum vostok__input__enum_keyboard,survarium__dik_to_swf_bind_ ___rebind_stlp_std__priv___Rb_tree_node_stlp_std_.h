////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< std::pair< enum input::enum_keyboard, dik_to_swf_bind > >::rebind< std::priv::_Rb_tree_node< std::pair< enum input::enum_keyboard const , dik_to_swf_bind > > > {
}; // struct std_allocator< std::pair< enum input::enum_keyboard, dik_to_swf_bind > >::rebind< std::priv::_Rb_tree_node< std::pair< enum input::enum_keyboard const , dik_to_swf_bind > > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< enum input::enum_keyboard, dik_to_swf_bind > >::rebind< std::priv::_Rb_tree_node< std::pair< enum input::enum_keyboard const , dik_to_swf_bind > > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
