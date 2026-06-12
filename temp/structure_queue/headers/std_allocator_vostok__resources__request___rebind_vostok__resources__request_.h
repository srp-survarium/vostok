////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< resources::request >::rebind< resources::request > {
}; // struct std_allocator< resources::request >::rebind< resources::request >

STATIC_SIZE_ASSERT(std_allocator< resources::request >::rebind< resources::request >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
