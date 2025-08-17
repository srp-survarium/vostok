////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.08.2025
////////////////////////////////////////////////////////////////////////////

#include <boost/noncopyable.hpp>

#include "hit_type_parameters.h"

#ifndef FIND_HIT_PARAMETERS_BY_TYPE_PREDICATE_H_INCLUDED
#define FIND_HIT_PARAMETERS_BY_TYPE_PREDICATE_H_INCLUDED

namespace stalker2 {

/* survarium::find_hit_parameters_by_type_predicate */

struct find_hit_parameters_by_type_predicate : boost::noncopyable {
public:
	// STATE[UNVERIFIED]
	find_hit_parameters_by_type_predicate(pcstr hit_t) : hit_type(hit_t) {}

	// STATE[UNVERIFIED] sushi@TODO: hit_type_params most likely uses getter
	bool operator()(hit_type_parameters* hit_type_params) const { return strings::equal(hit_type, hit_type_params->m_type.c_str()); }

private:
	/* offset 0x0000 */ pcstr                               hit_type;
}; // struct find_hit_parameters_by_type_predicate
} // namespace stalker2

#endif // #ifndef FIND_HIT_PARAMETERS_BY_TYPE_PREDICATE_H_INCLUDED