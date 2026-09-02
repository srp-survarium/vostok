// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/player_stealth.h>

namespace survarium {

player_stealth::player_stealth( )
{
}

player_stealth::player_stealth( player_stealth const& other )
{
	*this = other;
}

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

void player_stealth::load( configs::binary_config_value const& config )
{
	m_default_value				= (float)config["default_value"];
	m_default_sound_value		= (float)config["default_sound_value"];
	m_stand_factor				= (float)config["stand_factor"];
	m_crouch_factor				= (float)config["crouch_factor"];
	m_crouch_sound_factor		= (float)config["crouch_sound_factor"];
	m_walk_factor				= (float)config["walk_factor"];
	m_walk_sound_factor			= (float)config["walk_sound_factor"];
	m_sprint_factor				= (float)config["sprint_factor"];
	m_sprint_sound_factor		= (float)config["sprint_sound_factor"];
	m_detection_level			= (float)config["detection_level"];
	m_always_visible_distance	= (float)config["always_visible_distance"];
}

} // namespace survarium
