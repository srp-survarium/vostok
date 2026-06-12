////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< variant< 32 > const* >::rebind< variant< 32 > const* > {
}; // struct std_allocator< variant< 32 > const* >::rebind< variant< 32 > const* >

STATIC_SIZE_ASSERT(std_allocator< variant< 32 > const* >::rebind< variant< 32 > const* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
