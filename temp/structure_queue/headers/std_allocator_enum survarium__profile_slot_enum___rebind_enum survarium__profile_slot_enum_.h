////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< enum profile_slot_enum >::rebind< enum profile_slot_enum > {
}; // struct std_allocator< enum profile_slot_enum >::rebind< enum profile_slot_enum >

STATIC_SIZE_ASSERT(std_allocator< enum profile_slot_enum >::rebind< enum profile_slot_enum >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
