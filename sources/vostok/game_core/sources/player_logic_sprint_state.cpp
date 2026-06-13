////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_sprint_state.h"

#include <vostok/game_core/base_player.h>
#include <vostok/game_core/player_stamina.h>

namespace survarium {

// claude@NOTE: structure matches (single body stmt); walled at inline-vs-call. The
// target INLINES boost::function<void()>::operator=<bind_t<...>> as the partial-spec
// body `self_type(f).swap(*this)` (function-ctor + assign_to + swap + clear), whereas
// our base emits an out-of-line `call operator=<...>`. Per-call-site whole-program
// inline-vs-call decision (patterns/boost-function-assign-inline.md); the `= boost::bind`
// source is correct, so this stays at the inline-vs-call %.
player_logic_sprint_state::player_logic_sprint_state( weapon_user_animations_selector& owner ) :
	player_logic_base_state	( owner, type_sprint )
{
	m_stamina_subscriber.subscription_callback = boost::bind( &player_logic_sprint_state::on_stamina_depleted, this );
}

// STATE[STUB]: VOSTOK_UNREACHABLE_CODE() placeholder, NOT matched.
// claude@NOTE: walled like the jump_logic_state_* family - the body builds two
// animation::mixing::animation_interval objects (target ~animation_interval dtors at
// 0x161/0x361) via the only producer, the static
// animation_lexeme_parameters::create_animation_interval, which is DECLARED
// (mixing_animation_lexeme_parameters.h:113) but never DEFINED (the animation module
// implements only create_animation_intervals, plural). A real body LNK2001s unresolved.
// NEXT STEP: define create_animation_interval in the animation module (cross-module),
// then friend + land this body. Placeholder keeps the pair-returning override
// compilable (C4716/LNK1257) so the anchor can construct the state.
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> player_logic_sprint_state::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	VOSTOK_UNREACHABLE_CODE( );
}

void player_logic_sprint_state::set_callbacks( boost::function<void()> const& initialize_callback, boost::function<void()> const& finalize_callback )
{
	m_initialize_callback	= initialize_callback;
	m_finalize_callback		= finalize_callback;
}

// claude@NOTE: structure matches (single stmt); walled at inline-vs-call. The target
// does `call base_player::force_animation_selection` (a real out-of-line method), but
// base_player.h declares that method inline, so our base inlines it to
// `mov byte ptr [this+118h], 1`. Out-lining it is a base_player-TU change; cross-unit
// out-lining is off-limits here, so this stays at the inline-vs-call %.
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
