////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< relocate_item_descr >::rebind< relocate_item_descr > {
}; // struct std_allocator< relocate_item_descr >::rebind< relocate_item_descr >

STATIC_SIZE_ASSERT(std_allocator< relocate_item_descr >::rebind< relocate_item_descr >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
