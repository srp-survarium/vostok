////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< ladder_ptr >::rebind< resources::resource_ptr< ladder, resources::unmanaged_intrusive_base > > {
}; // struct std_allocator< ladder_ptr >::rebind< resources::resource_ptr< ladder, resources::unmanaged_intrusive_base > >

STATIC_SIZE_ASSERT(std_allocator< ladder_ptr >::rebind< resources::resource_ptr< ladder, resources::unmanaged_intrusive_base > >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
