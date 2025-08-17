////////////////////////////////////////////////////////////////////////////
//	Created 	: 14.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PROTECT_AFFECT_PREDICATE_H_INCLUDED
#define PROTECT_AFFECT_PREDICATE_H_INCLUDED

#include <boost/noncopyable.hpp>

#include "hit_affects_type_enum.h"

namespace stalker2 {

class damage_protector;

/* survarium::protect_affect_predicate */

struct protect_affect_predicate : boost::noncopyable {
public:
	// STATE[UNVERIFIED]
	protect_affect_predicate(
		pcstr								body_type_name,
		hit_affects_type_enum				affect_type): 
		m_body_type_name					(body_type_name),
		m_affect_type						(affect_type) {}

	void operator()(
		damage_protector*                  protector);

private:
	// STATE_M[UNVERIFIED]
	/* offset 0x0000 */ pcstr                               m_body_type_name;
	/* offset 0x0004 */ hit_affects_type_enum               m_affect_type;
	/* offset 0x0008 */ bool                                m_result;
}; // struct protect_affect_predicate
} // namespace stalker2

#endif // #ifndef PROTECT_AFFECT_PREDICATE_H_INCLUDED
