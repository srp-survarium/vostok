////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< game_object_* >::rebind< game_object_* > {
}; // struct std_allocator< game_object_* >::rebind< game_object_* >

STATIC_SIZE_ASSERT(std_allocator< game_object_* >::rebind< game_object_* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
