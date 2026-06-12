////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< enum game_action_id >::rebind< enum game_action_id > {
}; // struct std_allocator< enum game_action_id >::rebind< enum game_action_id >

STATIC_SIZE_ASSERT(std_allocator< enum game_action_id >::rebind< enum game_action_id >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
