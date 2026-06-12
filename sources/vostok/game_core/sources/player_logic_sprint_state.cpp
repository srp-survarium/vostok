////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_sprint_state.h"

#include <vostok/game_core/base_player.h>
#include <vostok/game_core/player_stamina.h>

namespace survarium {

player_logic_sprint_state::player_logic_sprint_state( weapon_user_animations_selector& owner ) :
	player_logic_base_state	( owner, type_sprint )
{
	m_stamina_subscriber.subscription_callback = boost::bind( &player_logic_sprint_state::on_stamina_depleted, this );
}

// STATE[STUB]: body is a VOSTOK_UNREACHABLE_CODE() placeholder, NOT matched -
// the lexeme/expression mixing infrastructure is not yet reversed. report.json DOES
// pair and score it (3.32%); the real body is the large routine in the carcass below.
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> player_logic_sprint_state::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// claude@TODO: lexeme/expression wall (mixing infrastructure not yet matched).
	// Placeholder keeps the pair-returning override compilable (C4716/LNK1257) so
	// the anchor can construct the state and the other members can match.
	VOSTOK_UNREACHABLE_CODE( );

	// LOCALS
	// pcstr 						look_animation_id
	// animation::instant_interpolator interpolator
	// animation::mixing::animation_lexeme_parameters look_lexeme_parameters
	// float 						start_animation_interval_time
	// animation::mixing::animation_lexeme look_lexeme
	// resources::managed_resource_ptr look_animation
	// animation::mixing::animation_lexeme movement_lexeme
	// animation::linear_interpolator l_interpolator
	// ******

	// CONSTANTS
	// const player_logic_sprint_state::selected_animations::__l2::<unnamed-tag> main_sprint_animation_index = 0;
	// const player_logic_sprint_state::selected_animations::__l2::<unnamed-tag> sprint_look_animation_index = 1;
	// ******

	// FUNCTION BODY
	// <0>
	// <0x6f9681>|0x011|+0x010:'41'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <0x6f9691>|0x021|+0x145:'56'
	// <0x6f97d6>|0x166|+0x00b:'57'
	// <0x6f97e1>|0x171|+0x049:'58'
	// <0x6f982a>|0x1ba|+0x05c:'59'
	// <0>
	// <0x6f9886>|0x216|+0x01f:'61'
	// <0x6f98a5>|0x235|+0x039:'62'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6f98de>|0x26e|+0x08a:'68'
	// <0>
	// <0x6f9968>|0x2f8|+0x011:'70'
	// <0>
	// <0x6f9979>|0x309|+0x075:'72'
	// ******
}

void player_logic_sprint_state::set_callbacks( boost::function<void()> const& initialize_callback, boost::function<void()> const& finalize_callback )
{
	m_initialize_callback	= initialize_callback;
	m_finalize_callback		= finalize_callback;
}

void player_logic_sprint_state::on_stamina_depleted( )
{
	m_user->force_animation_selection( );
}

void player_logic_sprint_state::initialize( )
{
	m_user->stamina( ).subscribe_on_depletion( &m_stamina_subscriber );
	m_initialize_callback( );
}

void player_logic_sprint_state::finalize( )
{
	m_user->stamina( ).unsubscribe_from_depletion( &m_stamina_subscriber );
	m_finalize_callback( );
}

} // namespace survarium
