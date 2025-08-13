////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "affects_threshold.h"

namespace stalker2 {

affects_threshold::affects_threshold(float value, unsigned int affects_count, body_part_parameters *const bodypart):
	next(NULL),
	m_value(value),
	m_affects_count(affects_count),
	m_bodypart(bodypart)
{
}

} // namespace stalker2 