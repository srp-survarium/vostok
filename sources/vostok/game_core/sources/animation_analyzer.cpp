////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

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
	// FUNCTION BODY
	// <0x6fcbd0>|0x000|+0x039:'51'	{
	// <0x6fcc09>|0x039|      :'52'	}
	// ******
}

animation_analyzer::~animation_analyzer( )
{
}

} // namespace survarium
