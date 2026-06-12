////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< resources::creation_request >::rebind< resources::creation_request > {
}; // struct std_allocator< resources::creation_request >::rebind< resources::creation_request >

STATIC_SIZE_ASSERT(std_allocator< resources::creation_request >::rebind< resources::creation_request >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
