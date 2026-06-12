////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< zone_group::zone_wrapper >::rebind< zone_group::zone_wrapper > {
}; // struct std_allocator< zone_group::zone_wrapper >::rebind< zone_group::zone_wrapper >

STATIC_SIZE_ASSERT(std_allocator< zone_group::zone_wrapper >::rebind< zone_group::zone_wrapper >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
