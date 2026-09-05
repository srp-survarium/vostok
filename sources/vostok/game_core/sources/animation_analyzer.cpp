// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/animation_analyzer.h>
#include <vostok/game_core/leg_info.h>

namespace survarium {

animation_analyzer::animation_analyzer( leg_info const* legs_info, const u32 legs_count, animation::skeleton const& skeleton ) :
	m_legs_info		( legs_info ),
	m_legs_count	( legs_count ),
	m_skeleton		( skeleton ),
	m_ground_height	( 0.0f )
{
}

animation_analyzer::~animation_analyzer( )
{
}

} // namespace survarium
