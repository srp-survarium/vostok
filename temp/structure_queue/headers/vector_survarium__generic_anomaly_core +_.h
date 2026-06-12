////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::generic_anomaly_core *,survarium::std_allocator<survarium::generic_anomaly_core *> >
	stlp_std::vector<survarium::generic_anomaly_core * >;
class survarium::generic_anomaly_core *;

/* FORWARD REFS */
class survarium::generic_anomaly_core;
class survarium::generic_anomaly_core*;

namespace survarium {

class vector< generic_anomaly_core* > : public std::vector< generic_anomaly_core* > {
public:
	inline									vector<survarium::generic_anomaly_core *>( ) { /* no source */ }
	inline									vector<survarium::generic_anomaly_core *>( u32 arg_0, generic_anomaly_core* const& arg_1 ) { /* no source */ }
	inline	explicit						vector<survarium::generic_anomaly_core *>( u32 arg_0 ) { /* no source */ }

	inline	generic_anomaly_core*&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	generic_anomaly_core* const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32								size		( ) const { /* no source */ }

	inline									~vector<survarium::generic_anomaly_core *>( ) { /* no source */ }
}; // class vector< generic_anomaly_core* >

STATIC_SIZE_ASSERT(vector< generic_anomaly_core* >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
