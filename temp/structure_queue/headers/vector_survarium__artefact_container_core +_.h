////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::artefact_container_core *,survarium::std_allocator<survarium::artefact_container_core *> >
	stlp_std::vector<survarium::artefact_container_core * >;
class survarium::artefact_container_core *;

/* FORWARD REFS */
class survarium::artefact_container_core;
class survarium::artefact_container_core*;

namespace survarium {

class vector< artefact_container_core* > : public std::vector< artefact_container_core* > {
public:
	inline										vector<survarium::artefact_container_core *>( ) { /* no source */ }
	inline										vector<survarium::artefact_container_core *>( u32 arg_0, artefact_container_core* const& arg_1 ) { /* no source */ }
	inline	explicit							vector<survarium::artefact_container_core *>( u32 arg_0 ) { /* no source */ }

	inline	artefact_container_core*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	artefact_container_core* const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32									size		( ) const { /* no source */ }

	inline										~vector<survarium::artefact_container_core *>( ) { /* no source */ }
}; // class vector< artefact_container_core* >

STATIC_SIZE_ASSERT(vector< artefact_container_core* >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
