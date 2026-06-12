////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< base_project::resolve_link_object >::rebind< base_project::resolve_link_object > {
}; // struct std_allocator< base_project::resolve_link_object >::rebind< base_project::resolve_link_object >

STATIC_SIZE_ASSERT(std_allocator< base_project::resolve_link_object >::rebind< base_project::resolve_link_object >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
