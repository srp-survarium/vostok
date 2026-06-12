////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< generic_anomaly_core* >::rebind< generic_anomaly_core* > {
}; // struct std_allocator< generic_anomaly_core* >::rebind< generic_anomaly_core* >

STATIC_SIZE_ASSERT(std_allocator< generic_anomaly_core* >::rebind< generic_anomaly_core* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
