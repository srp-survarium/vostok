////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef REMOVE_LEFT_RECEIVERS_PREDICATE_H_INCLUDED
#define REMOVE_LEFT_RECEIVERS_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::buffer_vector<survarium::hit_receiver_info>;

/* FORWARD REFS */
class survarium::hit_receiver_info;

namespace survarium {

struct remove_left_receivers_predicate {
	inline	explicit	remove_left_receivers_predicate	( buffer_vector< hit_receiver_info > const& arg_0 ) { /* no source */ }

	inline	bool		operator()						( hit_receiver_info const& arg_0 ) const { /* no source */ }

public:
	/* 0x0000 */	buffer_vector< hit_receiver_info > const*	m_receivers;
}; // struct remove_left_receivers_predicate

STATIC_SIZE_ASSERT(remove_left_receivers_predicate, 0x4);

} // namespace survarium

#endif // #ifndef REMOVE_LEFT_RECEIVERS_PREDICATE_H_INCLUDED
