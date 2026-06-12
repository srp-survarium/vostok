////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::anomaly_state *,survarium::std_allocator<survarium::anomaly_state *> >
	stlp_std::vector<survarium::anomaly_state * >;
class survarium::anomaly_state *;

/* FORWARD REFS */
class survarium::anomaly_state;
class survarium::anomaly_state*;

namespace survarium {

class vector< anomaly_state* > : public std::vector< anomaly_state* > {
public:
	inline								vector<survarium::anomaly_state *>	( ) { /* no source */ }
	inline								vector<survarium::anomaly_state *>	( u32 arg_0, anomaly_state* const& arg_1 ) { /* no source */ }
	inline	explicit					vector<survarium::anomaly_state *>	( u32 arg_0 ) { /* no source */ }

	inline	anomaly_state*&				operator[]							( u32 arg_0 ) { /* no source */ }
	inline	anomaly_state* const&		operator[]							( u32 arg_0 ) const { /* no source */ }

	inline	u32							size								( ) const { /* no source */ }

	inline								~vector<survarium::anomaly_state *>	( ) { /* no source */ }
}; // class vector< anomaly_state* >

STATIC_SIZE_ASSERT(vector< anomaly_state* >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
