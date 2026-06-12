////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::artefact_container_core;
class survarium::artefact_container_core*;

namespace survarium {

class std_allocator< artefact_container_core* > {
public:
	inline										std_allocator<survarium::artefact_container_core *>( ) { /* no source */ }

	inline	artefact_container_core**			address		( artefact_container_core*& arg_0 ) const { /* no source */ }
	inline	artefact_container_core* const*		address		( artefact_container_core* const& arg_0 ) const { /* no source */ }

	inline	artefact_container_core**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	artefact_container_core**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	artefact_container_core**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	artefact_container_core**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( artefact_container_core** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( artefact_container_core** arg_0, artefact_container_core* const& arg_1 ) { /* no source */ }

	inline	void								destroy		( artefact_container_core** arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< artefact_container_core* >

STATIC_SIZE_ASSERT(std_allocator< artefact_container_core* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
