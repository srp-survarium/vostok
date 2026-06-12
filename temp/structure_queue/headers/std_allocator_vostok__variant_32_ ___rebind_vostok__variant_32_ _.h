////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< variant< 32 > >::rebind< variant< 32 > > {
}; // struct std_allocator< variant< 32 > >::rebind< variant< 32 > >

STATIC_SIZE_ASSERT(std_allocator< variant< 32 > >::rebind< variant< 32 > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
