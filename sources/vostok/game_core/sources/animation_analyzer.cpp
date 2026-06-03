////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/animation_analyzer.h>

namespace survarium {

// STATE[100%|DONE]
animation_analyzer::animation_analyzer( leg_info const* legs_info, u32 legs_count, animation::skeleton const& skeleton ) :
	m_legs_info		( legs_info ),
	m_legs_count	( legs_count ),
	m_skeleton		( skeleton ),
	m_ground_height	( 0.0f )
{
	// FUNCTION BODY
	// ******
}

// STATE[100%|DONE]
animation_analyzer::~animation_analyzer( )
{
}

} // namespace survarium
