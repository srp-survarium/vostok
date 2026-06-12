////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef CHECK_HEALTH_PREDICATE_H_INCLUDED
#define CHECK_HEALTH_PREDICATE_H_INCLUDED

/* INCLUDES */
class survarium::body_part_parameters* const;

namespace survarium {

struct check_health_predicate : public boost::noncopyable {
	inline	bool	operator()				( body_part_parameters* const arg_0 ) const { /* no source */ }

	inline			check_health_predicate	( ) { /* no source */ }
	inline			~check_health_predicate	( ) { /* no source */ }
}; // struct check_health_predicate

STATIC_SIZE_ASSERT(check_health_predicate, 0x1);

} // namespace survarium

#endif // #ifndef CHECK_HEALTH_PREDICATE_H_INCLUDED
