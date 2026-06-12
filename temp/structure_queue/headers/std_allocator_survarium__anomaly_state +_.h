////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::anomaly_state;
class survarium::anomaly_state*;

namespace survarium {

class std_allocator< anomaly_state* > {
public:
	inline								std_allocator<survarium::anomaly_state *>( ) { /* no source */ }

	inline	anomaly_state**				address		( anomaly_state*& arg_0 ) const { /* no source */ }
	inline	anomaly_state* const*		address		( anomaly_state* const& arg_0 ) const { /* no source */ }

	inline	anomaly_state**				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	anomaly_state**				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	anomaly_state**				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	anomaly_state**				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( anomaly_state** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( anomaly_state** arg_0, anomaly_state* const& arg_1 ) { /* no source */ }

	inline	void						destroy		( anomaly_state** arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< anomaly_state* >

STATIC_SIZE_ASSERT(std_allocator< anomaly_state* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
