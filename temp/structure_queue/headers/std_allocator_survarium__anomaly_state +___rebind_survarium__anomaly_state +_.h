////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< anomaly_state* >::rebind< anomaly_state* > {
}; // struct std_allocator< anomaly_state* >::rebind< anomaly_state* >

STATIC_SIZE_ASSERT(std_allocator< anomaly_state* >::rebind< anomaly_state* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
