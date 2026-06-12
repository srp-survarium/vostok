////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< fixed_string< 16 > >::rebind< fixed_string< 16 > > {
}; // struct std_allocator< fixed_string< 16 > >::rebind< fixed_string< 16 > >

STATIC_SIZE_ASSERT(std_allocator< fixed_string< 16 > >::rebind< fixed_string< 16 > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
