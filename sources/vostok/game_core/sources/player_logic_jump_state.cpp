// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "player_logic_jump_state.h"
#include <vostok/game_core/base_player.h>

namespace survarium {

player_logic_jump_state::player_logic_jump_state( weapon_user_animations_selector& owner ) :
	player_logic_base_state	( owner, type_jump ),
	m_logic					( owner )
{
}

void player_logic_jump_state::initialize( )
{
	m_logic.activate( );
	ASSERT( UNKNOWN_EXPRESSION );
}

void player_logic_jump_state::execute( )
{
	m_logic.tick( );
}

bool player_logic_jump_state::is_ready_for_transition( ) const
{
	return m_logic.is_jump_finished( )
		|| ( *m_user->damage_model( ) ).broken_legs_count( ) == 2
		|| !m_user->is_alive( );
}

void player_logic_jump_state::finalize( )
{
	m_logic.deactivate( );
}

std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> player_logic_jump_state::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, const bool is_third_view ) const
{
	return m_logic.selected_animations( buffer, weapon_parameters, is_third_view );
}

void player_logic_jump_state::set_user( base_player& user )
{
	player_logic_base_state::set_user( user );
	m_logic.set_user( user );
}

} // namespace survarium
