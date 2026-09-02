// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.08.2025
////////////////////////////////////////////////////////////////////////////
#ifndef AFFECTS_THRESHOLD_H_INCLUDED
#define AFFECTS_THRESHOLD_H_INCLUDED

#include "hit_affects_type_enum.h"

namespace survarium {

class body_part_parameters;


class affects_threshold : private boost::noncopyable {
public:
									affects_threshold	( const float value, const u32 affects_count, body_part_parameters* const bodypart );

	float							value				( )	const { return m_value; }
	u32								get_affects_count	( ) const { return m_affects_count; }

	hit_affects_type_enum const*	get_affects			( )	const {
		return pointer_cast<hit_affects_type_enum const*>(
			pointer_cast<pcbyte>(this) + sizeof(*this)
		);
	}

	body_part_parameters* const	bodypart			( )	const { return m_bodypart; }

	affects_threshold*			next;

private:
	const float					m_value;
	const u32					m_affects_count;
	body_part_parameters* const	m_bodypart;
}; // class affects_threshold

STATIC_SIZE_ASSERT(affects_threshold, 0x10);


} // namespace survarium

#endif // #ifndef AFFECTS_THRESHOLD_H_INCLUDED
