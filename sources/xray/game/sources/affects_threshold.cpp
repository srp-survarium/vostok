////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "affects_threshold.h"

namespace stalker2 {

// STATE[UNVERIFIED]
affects_threshold::affects_threshold(
	float                              value,
	u32                                affects_count,
	body_part_parameters*              bodypart) :
	next(NULL),
	m_value(value),
	m_affects_count(affects_count),
	m_bodypart(bodypart)
{
}

} // namespace stalker2 