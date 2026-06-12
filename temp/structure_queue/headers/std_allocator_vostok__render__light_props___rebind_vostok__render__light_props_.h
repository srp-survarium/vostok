////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< render::light_props >::rebind< render::light_props > {
}; // struct std_allocator< render::light_props >::rebind< render::light_props >

STATIC_SIZE_ASSERT(std_allocator< render::light_props >::rebind< render::light_props >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
