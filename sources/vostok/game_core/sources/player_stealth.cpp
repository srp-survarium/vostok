////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_stealth.h>

namespace survarium {

// STATE[STUB]
// survarium::player_stealth::player_stealth()
player_stealth::player_stealth( )
{
	// FUNCTION BODY
	// ******
}

// STATE[100%|DONE]
player_stealth::player_stealth( player_stealth const& other )
{
	*this = other;
}

// STATE[100%|DONE]
player_stealth& player_stealth::operator=( player_stealth const& other )
{
	if ( this != &other )
	{
		m_default_value				= other.m_default_value;
		m_default_sound_value		= other.m_default_sound_value;
		m_stand_factor				= other.m_stand_factor;
		m_crouch_factor				= other.m_crouch_factor;
		m_crouch_sound_factor		= other.m_crouch_sound_factor;
		m_walk_factor				= other.m_walk_factor;
		m_walk_sound_factor			= other.m_walk_sound_factor;
		m_sprint_factor				= other.m_sprint_factor;
		m_sprint_sound_factor		= other.m_sprint_sound_factor;
		m_detection_level			= other.m_detection_level;
		m_always_visible_distance	= other.m_always_visible_distance;
	}
	return *this;
}

// STATE[STUB]
// void survarium::player_stealth::load(vostok::configs::binary_config_value const&)
void player_stealth::load( configs::binary_config_value const& config )
{
	// FUNCTION BODY
	// <0x5aaae9>|0x009|+0x019:'45'
	// <0x5aab02>|0x022|+0x01a:'46'
	// <0x5aab1c>|0x03c|+0x01a:'47'
	// <0x5aab36>|0x056|+0x01a:'48'
	// <0x5aab50>|0x070|+0x01a:'49'
	// <0x5aab6a>|0x08a|+0x01a:'50'
	// <0x5aab84>|0x0a4|+0x01a:'51'
	// <0x5aab9e>|0x0be|+0x01a:'52'
	// <0x5aabb8>|0x0d8|+0x01a:'53'
	// <0x5aabd2>|0x0f2|+0x01a:'54'
	// <0x5aabec>|0x10c|+0x01a:'55'
	// ******
}

} // namespace survarium
