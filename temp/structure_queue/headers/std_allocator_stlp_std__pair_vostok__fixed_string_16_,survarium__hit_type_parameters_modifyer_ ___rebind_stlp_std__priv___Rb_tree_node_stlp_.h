////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< std::pair< fixed_string< 16 >, hit_type_parameters_modifyer > >::rebind< std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > > {
}; // struct std_allocator< std::pair< fixed_string< 16 >, hit_type_parameters_modifyer > >::rebind< std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > >

STATIC_SIZE_ASSERT(std_allocator< std::pair< fixed_string< 16 >, hit_type_parameters_modifyer > >::rebind< std::priv::_Rb_tree_node< std::pair< fixed_string< 16 > const , hit_type_parameters_modifyer > > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
