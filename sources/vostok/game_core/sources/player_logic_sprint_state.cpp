////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_sprint_state.h"

#include <vostok/game_core/base_player.h>
#include <vostok/game_core/player_stamina.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_user_animations_container.h>
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/animation/instant_interpolator.h>

namespace survarium {

static float s_aim_transition_time = 0.3f;

// claude@NOTE: single .rdata copy in the target exe (?sprint_animations_captions@survarium@@3QBQBDB
// @va 0x89d19c); 2 captions (move, look) sitting between crouch_animations_captions and
// jump_animations_captions. Mirrors m_sprint_animations[2][2].
pcstr const sprint_animations_captions[] = {
	"run_fwd",
	"run_fwd_look",
};

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

std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> player_logic_sprint_state::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, const bool is_third_view ) const
{
	VOSTOK_UNREFERENCED_PARAMETER( weapon_parameters );
	enum {
		main_sprint_animation_index,
		sprint_look_animation_index,
	};

	animation::linear_interpolator		l_interpolator( s_aim_transition_time );

	animation::mixing::animation_lexeme	movement_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			m_owner.animations( ).get_sprint_animation_caption( main_sprint_animation_index ),
			m_owner.animations( ).get_sprint_animation( main_sprint_animation_index, is_third_view ),
			0,
			0
		)
		.weight_synchronization_group_id	( 0 )
		.time_synchronization_group_id		( 0 )
		.weight_interpolator				( l_interpolator )
		.time_scale_interpolator			( l_interpolator )
		.time_scale							( m_user->get_movement_speed_factor( ) )
		.animated_object					( m_user )
		.user_data							( 1 )
	);

	animation::instant_interpolator		interpolator;

	pcstr const							look_animation_id	= m_owner.animations( ).get_sprint_animation_caption( sprint_look_animation_index );

	resources::managed_resource_ptr		look_animation		= m_owner.animations( ).get_sprint_animation( sprint_look_animation_index, is_third_view );

	animation::mixing::animation_lexeme_parameters	look_lexeme_parameters( buffer, look_animation_id, look_animation, 0, &movement_lexeme );

	float const							start_animation_interval_time	= look_lexeme_parameters.animation_intervals( )[ 0 ].length( ) * m_owner.look_time_factor( );

	look_lexeme_parameters
		.start_animation_interval_time	( start_animation_interval_time )
		.animated_object				( m_user )
		.additivity_priority			( 4 )
		.time_scale_interpolator		( interpolator )
		.time_calculator				( m_owner.look_time_calculator( ) );

	animation::mixing::animation_lexeme	look_lexeme( look_lexeme_parameters );

	return std::make_pair< animation::mixing::expression, animation::mixing::animation_lexeme >(
		movement_lexeme + look_lexeme,
		movement_lexeme
	);
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
