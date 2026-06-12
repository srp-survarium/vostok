////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< victory_items_container* >::rebind< void* > {
}; // struct std_allocator< victory_items_container* >::rebind< void* >

STATIC_SIZE_ASSERT(std_allocator< victory_items_container* >::rebind< void* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
