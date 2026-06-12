////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< collision_geometry* >::rebind< collision_geometry* > {
}; // struct std_allocator< collision_geometry* >::rebind< collision_geometry* >

STATIC_SIZE_ASSERT(std_allocator< collision_geometry* >::rebind< collision_geometry* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
