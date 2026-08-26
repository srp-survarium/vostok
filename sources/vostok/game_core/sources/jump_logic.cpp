////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/jump_logic.h>

#include "jump_logic_base_state.h"
#include "jump_logic_state_inactive.h"
#include "jump_logic_state_start.h"
#include "jump_logic_state_landing.h"
#include <vostok/game_core/player_input.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/ai/fsm.h>

namespace survarium {

// claude@MATCH: the external survarium::true_predicate is defined in
// breath_vibration_calculator.cpp (target 0xbb5c0); initialize_logic references it
// via boost::bind<bool>(&true_predicate) - forward-declare and reuse, do not redefine.
bool true_predicate( );

// claude@MATCH: file STATIC - the target obj records this function under the PLAIN
// name `survarium::get_move_direction` (no mangling), the file-static signature
// (see patterns/static-plain-name-pairing.md); an extern definition never pairs.
static move_direction_enum get_move_direction( player_input const& input )
{
	bool move_bwd_pressed;
	bool move_right_pressed;
	bool move_fwd_pressed;
	bool move_left_pressed;

	move_fwd_pressed	= ( input.actions_mask & 0x1 ) != 0;
	move_bwd_pressed	= ( input.actions_mask & 0x2 ) != 0;
	move_left_pressed	= ( input.actions_mask & 0x4 ) != 0;
	move_right_pressed	= ( input.actions_mask & 0x8 ) != 0;

	if ( move_fwd_pressed && move_bwd_pressed )
	{
		move_fwd_pressed = false;
		move_bwd_pressed = false;
	}

	if ( move_left_pressed && move_right_pressed )
	{
		move_left_pressed = false;
		move_right_pressed = false;
	}

	if ( move_fwd_pressed )
	{
		if ( move_left_pressed )
			return move_direction_fwd_left;
		else if ( move_right_pressed )
			return move_direction_fwd_right;
		else
			return move_direction_fwd;
	}
	else if ( move_bwd_pressed )
	{
		if ( move_left_pressed )
			return move_direction_back_left;
		else if ( move_right_pressed )
			return move_direction_back_right;
		else
			return move_direction_back;
	}
	else if ( move_left_pressed )
		return move_direction_left;
	else if ( move_right_pressed )
		return move_direction_right;
	else
		return move_direction_on_site;
}

jump_logic::jump_logic( weapon_user_animations_selector& owner )
	:	m_owner						( owner ),
		m_user						( 0 ),
		m_logic						( 0 ),
		m_animated_object			( 0 ),
		m_jumping_direction			( move_direction_on_site ),
		m_is_jump_from_right_leg	( true )
{
	initialize_logic( );
}

jump_logic::~jump_logic( )
{
	m_logic->clear_transitions( );

	while ( ai::fsm_state* state = m_logic->pop_state( ) )
		VOSTOK_DELETE_IMPL( g_allocator, state );

	VOSTOK_DELETE_IMPL( g_allocator, m_logic );
}

void jump_logic::initialize_logic( )
{
	m_logic = VOSTOK_NEW_IMPL( g_allocator, ai::fsm );

	jump_logic_base_state* inactive	= VOSTOK_NEW_IMPL( g_allocator, jump_logic_state_inactive )( *this );
	jump_logic_base_state* start	= VOSTOK_NEW_IMPL( g_allocator, jump_logic_state_start )( *this );
	jump_logic_base_state* landing	= VOSTOK_NEW_IMPL( g_allocator, jump_logic_state_landing )( *this );

	m_logic->add_state( inactive );
	m_logic->add_state( start );
	m_logic->add_state( landing );

	m_logic->add_transition( inactive, start, boost::bind< bool >( &true_predicate ) );
	m_logic->add_transition( start, landing, boost::bind( &jump_logic::landing_predicate, this ) );

	m_logic->set_initial_state( m_logic->states( ).front( ) );
}

void jump_logic::tick( )
{
	m_logic->tick( );
}

u32 get_jump_animation_index(
	const move_direction_enum		move_direction,
	const bool						jump_from_right_leg,
	const jump_animation_parts		animation_part
)
{
	enum {
		animations_per_simple_jump_direction		= 4,
		animations_per_two_legged_jump_direction	= 6,

		on_site_jump				= 0,

		forward_from_right_leg		= on_site_jump + animations_per_simple_jump_direction,
		forward_from_left_leg		= forward_from_right_leg + animations_per_two_legged_jump_direction,

		forward_right_from_right_leg	= forward_from_left_leg + animations_per_two_legged_jump_direction,
		forward_right_from_left_leg		= forward_right_from_right_leg + animations_per_two_legged_jump_direction,

		right_from_right_leg		= forward_right_from_left_leg + animations_per_two_legged_jump_direction,
		right_from_left_leg			= right_from_right_leg + animations_per_two_legged_jump_direction,

		backward_right_from_right_leg	= right_from_left_leg + animations_per_two_legged_jump_direction,
		backward_right_from_left_leg	= backward_right_from_right_leg + animations_per_two_legged_jump_direction,

		backward_from_right_leg		= backward_right_from_left_leg + animations_per_two_legged_jump_direction,
		backward_from_left_leg		= backward_from_right_leg + animations_per_two_legged_jump_direction,

		backward_left_from_right_leg	= backward_from_left_leg + animations_per_two_legged_jump_direction,
		backward_left_from_left_leg		= backward_left_from_right_leg + animations_per_two_legged_jump_direction,

		left_from_right_leg			= backward_left_from_left_leg + animations_per_two_legged_jump_direction,
		left_from_left_leg			= left_from_right_leg + animations_per_two_legged_jump_direction,

		left_forward_from_right_leg		= left_from_left_leg + animations_per_two_legged_jump_direction,
		left_forward_from_left_leg		= left_forward_from_right_leg + animations_per_two_legged_jump_direction,
	};

	switch ( move_direction )
	{
	case move_direction_on_site:
		return on_site_jump + animation_part;
	case move_direction_fwd:
		return ( jump_from_right_leg ? forward_from_right_leg : forward_from_left_leg ) + animation_part;
	case move_direction_fwd_right:
		return ( jump_from_right_leg ? forward_right_from_right_leg : forward_right_from_left_leg ) + animation_part;
	case move_direction_right:
		return ( jump_from_right_leg ? right_from_right_leg : right_from_left_leg ) + animation_part;
	case move_direction_back_right:
		return ( jump_from_right_leg ? backward_right_from_right_leg : backward_right_from_left_leg ) + animation_part;
	case move_direction_back:
		return ( jump_from_right_leg ? backward_from_right_leg : backward_from_left_leg ) + animation_part;
	case move_direction_back_left:
		return ( jump_from_right_leg ? backward_left_from_right_leg : backward_left_from_left_leg ) + animation_part;
	case move_direction_left:
		return ( jump_from_right_leg ? left_from_right_leg : left_from_left_leg ) + animation_part;
	case move_direction_fwd_left:
		return ( jump_from_right_leg ? left_forward_from_right_leg : left_forward_from_left_leg ) + animation_part;
	default:
		NODEFAULT( );
	}
}

resources::managed_resource_ptr jump_logic::get_animation( const jump_animation_parts anim_part, const bool is_third_view ) const
{
	return m_owner.animations().get_jump_animation(
		get_jump_animation_index( get_jump_direction( ), is_jump_from_right_leg( ), anim_part ),
		is_third_view
	);
}

pcstr jump_logic::get_animation_caption( const jump_animation_parts anim_part ) const
{
	return m_owner.animations().get_jump_animation_caption(
		get_jump_animation_index( get_jump_direction( ), is_jump_from_right_leg( ), anim_part )
	);
}

resources::managed_resource_ptr jump_logic::get_move_animation( const bool is_third_view ) const
{
	return m_owner.animations().get_stand_animation( false, m_jumping_direction * 3, is_third_view );
}

resources::managed_resource_ptr jump_logic::get_move_look_animation( const bool is_third_view ) const
{
	return m_owner.animations().get_stand_animation( false, m_jumping_direction * 3 + 2, is_third_view );
}

pcstr jump_logic::get_move_look_caption( ) const
{
	return m_owner.animations().get_stand_animation_caption( false, m_jumping_direction * 3 + 2 );
}

void jump_logic::activate( )
{
	m_jumping_direction			= get_move_direction( m_user->input( ) );
	m_is_jump_from_right_leg	= !m_owner.is_right_leg_supporting( );
}

void jump_logic::deactivate( )
{
	m_logic->set_initial_state( m_logic->states( ).front( ) );
}

void jump_logic::set_user( base_player& user )
{
	m_user = &user;

	for ( ai::fsm_state* i = m_logic->states( ).front( ); i; i = i->next )
		static_cast_checked< jump_logic_base_state* >( i )->set_user( user );
}

bool jump_logic::landing_predicate( ) const
{
	return true;
}

std::pair< animation::mixing::expression, animation::mixing::animation_lexeme > jump_logic::selected_animations(
	mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, const bool is_third_view
) const
{
	return static_cast< jump_logic_base_state* >( m_logic->current_state( ) )->selected_animations(
		buffer, is_third_view, m_owner.look_time_calculator( ), weapon_parameters
	);
}

float jump_logic::look_time_factor( ) const
{
	return m_owner.look_time_factor( );
}

bool jump_logic::does_need_land_and_run( ) const
{
	move_direction_enum landing_direction = get_move_direction( m_user->input( ) );

	switch ( m_jumping_direction )
	{
	case move_direction_on_site:
		return false;
	case move_direction_fwd:
		return landing_direction == move_direction_fwd || landing_direction == move_direction_fwd_left || landing_direction == move_direction_fwd_right;
	case move_direction_fwd_right:
		return landing_direction == move_direction_fwd_right || landing_direction == move_direction_fwd || landing_direction == move_direction_right;
	case move_direction_right:
		return landing_direction == move_direction_right || landing_direction == move_direction_fwd_right || landing_direction == move_direction_back_right;
	case move_direction_back_right:
		return landing_direction == move_direction_back_right || landing_direction == move_direction_right || landing_direction == move_direction_back;
	case move_direction_back:
		return landing_direction == move_direction_back || landing_direction == move_direction_back_right || landing_direction == move_direction_back_left;
	case move_direction_back_left:
		return landing_direction == move_direction_back_left || landing_direction == move_direction_back || landing_direction == move_direction_left;
	case move_direction_left:
		return landing_direction == move_direction_left || landing_direction == move_direction_back_left || landing_direction == move_direction_fwd_left;
	case move_direction_fwd_left:
		return landing_direction == move_direction_fwd_left || landing_direction == move_direction_left || landing_direction == move_direction_fwd;
	default:
		NODEFAULT( );
	}
}

bool jump_logic::is_jump_finished( ) const
{
	return static_cast< jump_logic_base_state* >( m_logic->current_state( ) )->is_jump_finished( );
}

// claude@MATCH: rows 80/81 and 86/87 repeat the land/land_look captions - the left
// (and right-to-left) jump directions have no land_run animations in the original
// data (string pooling folds the repeats onto one literal in the exe).
pcstr const jump_animations_captions[] = {
	"on_site_jump",
	"on_site_jump_look",
	"on_site_jump_land",
	"on_site_jump_land_look",

	"(r)fwd_jump",
	"(r)fwd_jump_look",
	"(r)fwd_jump_land",
	"(r)fwd_jump_land_look",
	"(r)fwd_jump_land_run",
	"(r)fwd_jump_land_run_look",

	"(l)fwd_jump",
	"(l)fwd_jump_look",
	"(l)fwd_jump_land",
	"(l)fwd_jump_land_look",
	"(l)fwd_jump_land_run",
	"(l)fwd_jump_land_run_look",

	"(r)fwd_right_jump",
	"(r)fwd_right_jump_look",
	"(r)fwd_right_jump_land",
	"(r)fwd_right_jump_land_look",
	"(r)fwd_right_jump_land_run",
	"(r)fwd_right_jump_land_run_look",

	"(l)fwd_right_jump",
	"(l)fwd_right_jump_look",
	"(l)fwd_right_jump_land",
	"(l)fwd_right_jump_land_look",
	"(l)fwd_right_jump_land_run",
	"(l)fwd_right_jump_land_run_look",

	"(r)right_jump",
	"(r)right_jump_look",
	"(r)right_jump_land",
	"(r)right_jump_land_look",
	"(r)right_jump_land_run",
	"(r)right_jump_land_run_look",

	"(l)right_jump",
	"(l)right_jump_look",
	"(l)right_jump_land",
	"(l)right_jump_land_look",
	"(l)right_jump_land_run",
	"(l)right_jump_land_run_look",

	"(r)back_right_jump",
	"(r)back_right_jump_look",
	"(r)back_right_jump_land",
	"(r)back_right_jump_land_look",
	"(r)back_right_jump_land_run",
	"(r)back_right_jump_land_run_look",

	"(l)back_right_jump",
	"(l)back_right_jump_look",
	"(l)back_right_jump_land",
	"(l)back_right_jump_land_look",
	"(l)back_right_jump_land_run",
	"(l)back_right_jump_land_run_look",

	"(r)back_jump",
	"(r)back_jump_look",
	"(r)back_jump_land",
	"(r)back_jump_land_look",
	"(r)back_jump_land_run",
	"(r)back_jump_land_run_look",

	"(l)back_jump",
	"(l)back_jump_look",
	"(l)back_jump_land",
	"(l)back_jump_land_look",
	"(l)back_jump_land_run",
	"(l)back_jump_land_run_look",

	"(r)back_left_jump",
	"(r)back_left_jump_look",
	"(r)back_left_jump_land",
	"(r)back_left_jump_land_look",
	"(r)back_left_jump_land_run",
	"(r)back_left_jump_land_run_look",

	"(l)back_left_jump",
	"(l)back_left_jump_look",
	"(l)back_left_jump_land",
	"(l)back_left_jump_land_look",
	"(l)back_left_jump_land_run",
	"(l)back_left_jump_land_run_look",

	"(r)left_jump",
	"(r)left_jump_look",
	"(r)left_jump_land",
	"(r)left_jump_land_look",
	"(r)left_jump_land",
	"(r)left_jump_land_look",

	"(l)left_jump",
	"(l)left_jump_look",
	"(l)left_jump_land",
	"(l)left_jump_land_look",
	"(l)left_jump_land",
	"(l)left_jump_land_look",

	"(r)fwd_left_jump",
	"(r)fwd_left_jump_look",
	"(r)fwd_left_jump_land",
	"(r)fwd_left_jump_land_look",
	"(r)fwd_left_jump_land_run",
	"(r)fwd_left_jump_land_run_look",

	"(l)fwd_left_jump",
	"(l)fwd_left_jump_look",
	"(l)fwd_left_jump_land",
	"(l)fwd_left_jump_land_look",
	"(l)fwd_left_jump_land_run",
	"(l)fwd_left_jump_land_run_look",
};

} // namespace survarium
