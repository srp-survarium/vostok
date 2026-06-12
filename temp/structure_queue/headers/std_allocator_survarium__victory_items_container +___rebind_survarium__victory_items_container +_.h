////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< victory_items_container* >::rebind< victory_items_container* > {
}; // struct std_allocator< victory_items_container* >::rebind< victory_items_container* >

STATIC_SIZE_ASSERT(std_allocator< victory_items_container* >::rebind< victory_items_container* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
