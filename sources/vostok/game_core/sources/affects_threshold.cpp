// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/affects_threshold.h>

namespace survarium {

affects_threshold::affects_threshold( const float value, const u32 affects_count, body_part_parameters* const bodypart ) :
	next			( NULL ),
	m_value			( value ),
	m_affects_count	( affects_count ),
	m_bodypart		( bodypart )
{
}

} // namespace survarium
