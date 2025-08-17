////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef AFFECTS_THRESHOLD_H_INCLUDED
#define AFFECTS_THRESHOLD_H_INCLUDED

#include <boost/noncopyable.hpp>

#include "hit_affects_type_enum.h"

namespace stalker2 {

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class body_part_parameters;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class affects_threshold : public boost::noncopyable {
public:
	affects_threshold(
		float                              value,
		u32                                affects_count,
		body_part_parameters*              bodypart);

	float							value()				const { return m_value; }
	u32								get_affects_count() const { return m_affects_count; }
	// hit_affects_type_enum const*	get_affects()		const { return NULL; } /* TODO */
	body_part_parameters*			bodypart()			const { return m_bodypart; }

public:
	affects_threshold*			next;
	const float					m_value;
	const u32					m_affects_count;
	body_part_parameters*		m_bodypart;
}; // class affects_threshold

namespace { 
	typedef char size_assert[
		sizeof(affects_threshold) == 0x10 ? 1 : -1
	];
}


} // namespace stalker2

#endif // #ifndef AFFECTS_THRESHOLD_H_INCLUDED