////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::generic_anomaly_core;
class survarium::generic_anomaly_core*;

namespace survarium {

class std_allocator< generic_anomaly_core* > {
public:
	inline									std_allocator<survarium::generic_anomaly_core *>( ) { /* no source */ }

	inline	generic_anomaly_core**			address		( generic_anomaly_core*& arg_0 ) const { /* no source */ }
	inline	generic_anomaly_core* const*	address		( generic_anomaly_core* const& arg_0 ) const { /* no source */ }

	inline	generic_anomaly_core**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	generic_anomaly_core**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	generic_anomaly_core**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	generic_anomaly_core**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate	( generic_anomaly_core** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct	( generic_anomaly_core** arg_0, generic_anomaly_core* const& arg_1 ) { /* no source */ }

	inline	void							destroy		( generic_anomaly_core** arg_0 ) { /* no source */ }

	inline	u32								max_size	( ) const { /* no source */ }
}; // class std_allocator< generic_anomaly_core* >

STATIC_SIZE_ASSERT(std_allocator< generic_anomaly_core* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
