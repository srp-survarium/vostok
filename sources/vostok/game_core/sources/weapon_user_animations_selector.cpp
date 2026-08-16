////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

// claude@VERIFY (structure, 2026-06-13): every body in this TU is a confirmed
// STRUCTURE MATCH (statement quantity + order). The low fuzzy% on several is a
// header-level, cross-unit byte wall, NOT a per-TU structure defect:
//   ctor / dtor / crouch_predicate ........ clean MATCH (no steerable residual)
//   sprint/jump/stand_predicate, activate,
//   deactivate ............................ SIZE-only on the ( *m_user->damage_model() )
//                                           deref: target ICF-folds intrusive_ptr::operator*
//                                           (mov eax,[eax]), base inlines the empty-stub'd
//                                           assert + extra indirection (intrusive_ptr_inline.h).
//                                           activate's BASE_ONLY/TRGT_ONLY rows are an
//                                           aligner artifact from SIZE drift; both sides 7 stmts.
//   is_weapon_firing/in_idle/toggling ..... SIZE +0x49 (identical) on the current_active_object()
//                                           temp: intrusive_ptr<inventory_item> copy-ctor
//                                           inline-vs-call + temp element type.
//   set_sprint_callbacks / look_time_factor SIZE-only: static_cast_checked instantiation /
//                                           float-op scheduling+regalloc (LTCG), not source-shape.
// None of the residuals is steerable from this TU; see the claude@NOTE blocks below.

#include "pch.h"
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/game_core/player_logic_base_state.h>		// current_state().serialize/deserialize virtuals
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/player_input.h>
#include <vostok/game_core/player_stamina.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/physics/api.h>
#include <vostok/physics/character_controller.h>

#include "player_logic_stand_state.h"
#include "player_logic_crouch_state.h"
#include "player_logic_sprint_state.h"
#include "player_logic_jump_state.h"

namespace survarium {

weapon_user_animations_selector::weapon_user_animations_selector( ) :
	m_forced_not_to_sprint		( false ),
	m_right_leg_is_supporting	( true )
{
	player_logic_base_state* const stand	= VOSTOK_NEW_IMPL( g_allocator, player_logic_stand_state )( *this );
	player_logic_base_state* const crouch	= VOSTOK_NEW_IMPL( g_allocator, player_logic_crouch_state )( *this );
	player_logic_base_state* const sprint	= VOSTOK_NEW_IMPL( g_allocator, player_logic_sprint_state )( *this );
	player_logic_base_state* const jumping	= VOSTOK_NEW_IMPL( g_allocator, player_logic_jump_state )( *this );

	m_logic.add_state( stand );
	m_logic.add_state( crouch );
	m_logic.add_state( sprint );
	m_logic.add_state( jumping );

	m_logic.add_transition( stand,   crouch,  boost::bind( &weapon_user_animations_selector::crouch_predicate, this ) );
	m_logic.add_transition( stand,   sprint,  boost::bind( &weapon_user_animations_selector::sprint_predicate, this ) );
	m_logic.add_transition( stand,   jumping, boost::bind( &weapon_user_animations_selector::jump_predicate,   this ) );

	m_logic.add_transition( crouch,  stand,   boost::bind( &weapon_user_animations_selector::stand_predicate,  this ) );
	m_logic.add_transition( crouch,  sprint,  boost::bind( &weapon_user_animations_selector::sprint_predicate, this ) );

	m_logic.add_transition( sprint,  crouch,  boost::bind( &weapon_user_animations_selector::crouch_predicate, this ) );
	m_logic.add_transition( sprint,  stand,   boost::bind( &weapon_user_animations_selector::stand_predicate,  this ) );
	m_logic.add_transition( sprint,  jumping, boost::bind( &weapon_user_animations_selector::jump_predicate,   this ) );

	m_logic.add_transition( jumping, crouch,  boost::bind( &weapon_user_animations_selector::broken_legs_predicate, this ) );
	m_logic.add_transition( jumping, stand,   boost::bind( &weapon_user_animations_selector::stand_predicate,  this ) );
	m_logic.add_transition( jumping, sprint,  boost::bind( &weapon_user_animations_selector::sprint_predicate, this ) );

	m_leg_damaged_subscriber.subscription_callback = boost::bind( &weapon_user_animations_selector::on_broken_limb_affect, this, _1, _2, _3 );

	m_player_logic_initial_state = stand;
}

weapon_user_animations_selector::~weapon_user_animations_selector( )
{
	while ( ai::fsm_state* state = m_logic.pop_state( ) )
		VOSTOK_DELETE_IMPL( g_allocator, state );
}

void weapon_user_animations_selector::tick( )
{
	if ( !m_user->input( ).is_trying_to_sprint( ) )
		m_forced_not_to_sprint = false;
	else if ( !( m_user->input( ).is_sprinting( ) && is_weapon_in_idle( ) ) && current_state( ).id( ) == type_sprint )
		m_forced_not_to_sprint = true;

	m_logic.tick( );
}

// claude@MATCH: the "deref idiom" is static_cast_checked - its non-DEBUG body is an
// inline cref-taking cast wrapper; the target's `call <op*>` IS the static_cast_checked
// instantiation kept out-of-line by LTCG (ICF-folded onto intrusive_ptr<...>::operator*,
// body `mov eax,[eax]; ret`), taking &(the cref-bound fsm_state* temp).
player_logic_base_state& weapon_user_animations_selector::current_state( ) const
{
	player_logic_base_state* const result = static_cast_checked< player_logic_base_state* >( m_logic.current_state( ) );
	ASSERT( UNKNOWN_EXPRESSION_T( result ) );
	return *result;
}

std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> weapon_user_animations_selector::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, const bool is_third_view ) const
{
	return current_state( ).selected_animations( buffer, weapon_parameters, is_third_view );
}

void weapon_user_animations_selector::activate( base_player& user, boost::function<void()> const& sprint_start_callback, boost::function<void()> const& sprint_end_callback )
{
	m_user = &user;
	for ( ai::fsm_state* i = m_logic.states( ).front( ); i; i = i->next )
		static_cast_checked< player_logic_base_state* >( i )->set_user( user );
	m_logic.set_initial_state( m_player_logic_initial_state );

	m_user->subscribe_animation_player(
		animation::channel_id_on_animation_interval_end,
		boost::bind( &weapon_user_animations_selector::on_interval_ended, this, _1 ),
		this,
		resources::managed_resource_ptr( ),
		m_user
	);

	( *m_user->damage_model( ) ).subscribe_on_affect( affects_type_leg_damage, &m_leg_damaged_subscriber );
	set_sprint_callbacks( sprint_start_callback, sprint_end_callback );
}

void weapon_user_animations_selector::deactivate( )
{
	m_logic.set_initial_state( NULL );
	m_user->unsubscribe_animation_player( animation::channel_id_on_animation_interval_end, this );
	( *m_user->damage_model( ) ).unsubscribe_from_affect( affects_type_leg_damage, &m_leg_damaged_subscriber );
}

bool weapon_user_animations_selector::is_ready_to_be_deactivated( ) const
{
	return current_state( ).is_ready_to_be_deactivated( );
}

bool weapon_user_animations_selector::is_sprinting( ) const
{
	return current_state( ).id( ) == type_sprint;
}

void weapon_user_animations_selector::serialize( network_core::udp_match_packet& packet ) const
{
	u8							state_id	= 0;
	bool						found		= false;
	ai::fsm_state const* const	current		= m_logic.current_state( );

	for ( ai::fsm_state const* i = m_logic.states( ).front( ); i; i = i->next, ++state_id )
	{
		if ( i == current )
		{
			found	= true;
			break;
		}
	}

	ASSERT( UNKNOWN_EXPRESSION_T( found ) );

	packet.append( state_id );
	static_cast_checked< player_logic_base_state const* >( current )->serialize( packet );
}

void weapon_user_animations_selector::deserialize( network_core::packet_reader& reader )
{
	u8 const					target_state_id	= reader.r< bool >( );
	u8							state_id		= 0;
	ai::fsm_state*				current			= NULL;

	for ( ai::fsm_state* i = m_logic.states( ).front( ); i; i = i->next, ++state_id ) // sushi@TODO: same comma-in-for-tail doubt as serialize above
	{
		if ( state_id == target_state_id )
		{
			current	= i;
			break;
		}
	}

	ASSERT( UNKNOWN_EXPRESSION_T( current ) );

	m_logic.set_initial_state( current );
	static_cast_checked< player_logic_base_state* >( current )->deserialize( reader );
}

weapon_user_state_enum weapon_user_animations_selector::get_current_state_id( ) const
{
	return static_cast_checked< player_logic_base_state* >( m_logic.current_state( ) )->id( );
}

bool weapon_user_animations_selector::sprint_predicate( ) const
{
	return !is_weapon_firing( ) && !is_weapon_toggling( )
		&& ( *m_user->damage_model( ) ).broken_legs_count( ) == 0
		&& !m_forced_not_to_sprint
		&& m_user->input( ).is_sprinting( )
		&& m_user->stamina( ).can_be_spent( );
}

bool weapon_user_animations_selector::stand_predicate( ) const
{
	switch ( current_state( ).id( ) )
	{
		default:
			return true;
		case type_crouch:
			return ( *m_user->damage_model( ) ).broken_legs_count( ) != 2 && !m_user->input( ).is_crouching( ) && m_user->physics_controller( ).can_stand( );
		case type_sprint:
			return !sprint_predicate( );
	}
}

void weapon_user_animations_selector::set_animation_callback( pcstr channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, resources::managed_resource_ptr( ), 0xff, NULL );
}

void weapon_user_animations_selector::remove_animation_callback( pcstr channel_id, pcvoid callback_uid )
{
	m_user->unsubscribe_animation_player( channel_id, callback_uid );
}

void weapon_user_animations_selector::set_animation_callback( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid, boost::function<enum animation::callback_return_type_enum(animation::animation_callback_params &)> const& animation_callback )
{
	m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, resources::managed_resource_ptr( ), NULL );
}

void weapon_user_animations_selector::remove_animation_callback( animation::reserved_channel_ids_enum channel_id, pcvoid callback_uid )
{
	m_user->unsubscribe_animation_player( channel_id, callback_uid );
}

bool weapon_user_animations_selector::crouch_predicate( ) const
{
	return broken_legs_predicate( ) || ( m_user->input( ).is_crouching( ) && m_user->physics_controller( ).can_crouch( ) );
}

// claude@NOTE: the `( *m_user->damage_model( ) )` deref wall, shared by deactivate /
// sprint_predicate / jump_predicate / stand_predicate / activate: target folds
// intrusive_ptr::operator* onto a no-assert representative (mov eax,[eax]); base emits the
// inline ASSERT( m_object ) (call to the empty stub) + an extra indirection. Header-level
// ICF/assert-fold artifact (intrusive_ptr_inline.h), not per-TU fixable; structure matches.
bool weapon_user_animations_selector::broken_legs_predicate( ) const
{
	return ( *m_user->damage_model( ) ).broken_legs_count( ) == 2;
}

bool weapon_user_animations_selector::jump_predicate( ) const
{
	bool const result =
		!is_weapon_firing( ) && !is_weapon_toggling( )
		&& ( *m_user->damage_model( ) ).broken_legs_count( ) == 0
		&& m_user->input( ).is_jumping( )
		&& m_user->physics_controller( ).can_jump( )
		&& m_user->stamina( ).current_value( ) >= m_user->stamina( ).amount_to_jump( );

	return result;
}

// claude@NOTE: is_weapon_in_idle / is_weapon_firing / is_weapon_toggling share a wall: the
// target builds the current_active_object() temp via an out-of-line `intrusive_ptr<inventory_item>`
// copy-ctor CALL, then reads the weapon_core flag; base INLINES the intrusive_ptr copy
// (refcount dec/interlocked_increment) and over a different temp element type
// (interactive_object/weapon_ammunition). Copy-ctor inline-vs-call + temp-type artifact, not
// per-TU fixable; the weapon_core::is_idle/is_toggling bodies (recovered from this asm) are correct.
bool weapon_user_animations_selector::is_weapon_in_idle( ) const
{
	return static_cast< weapon_core const& >( *m_user->current_active_object( ) ).is_idle( );
}

bool weapon_user_animations_selector::is_weapon_firing( ) const
{
	return static_cast< weapon_core const& >( *m_user->current_active_object( ) ).is_firing( );
}

bool weapon_user_animations_selector::is_weapon_toggling( ) const
{
	return static_cast< weapon_core const& >( *m_user->current_active_object( ) ).is_toggling( );
}

float weapon_user_animations_selector::look_time_factor( ) const
{
	return math::min( 1.f - math::epsilon_5, m_user->get_look_pitch( ) / 2.f + .5f );
}

float weapon_user_animations_selector::look_time_factor_calculator(
	const float		animation_length,
	const float		animation_time_before_time_scale_starts,
	const u32			time_scale_start_time_in_ms,
	const u32			current_time_in_ms,
	const u32			target_time_in_ms,
	const float		time_scale
) const
{
	VOSTOK_UNREFERENCED_PARAMETERS(
		animation_time_before_time_scale_starts,
		time_scale_start_time_in_ms,
		current_time_in_ms,
		target_time_in_ms,
		time_scale
	);
	return look_time_factor( ) * animation_length;
}

fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> weapon_user_animations_selector::look_time_calculator( ) const
{
	return look_time_functor( this, &weapon_user_animations_selector::look_time_factor_calculator );
}

animation::callback_return_type_enum weapon_user_animations_selector::on_interval_ended( animation::animation_callback_params& params )
{
	enum { c_right_leg_interval_id };

	ASSERT( UNKNOWN_EXPRESSION_T( &params ) );
	if ( params.animation_user_data == 1 )
		m_right_leg_is_supporting = params.animation_interval_id != c_right_leg_interval_id;

	return animation::callback_return_type_call_me_again;
}

void weapon_user_animations_selector::on_broken_limb_affect( pcstr bodypart, const hit_affects_type_enum affect, const affect_event_type_enum type )
{
	ASSERT_T_U( bodypart, type );
	ASSERT_CMP_U( affect, ==, 4 );
	m_user->force_animation_selection( );
}

bool weapon_user_animations_selector::is_in_jump( ) const
{
	return current_state( ).id( ) == type_jump;
}

void weapon_user_animations_selector::set_sprint_callbacks( boost::function<void()> const& start_callback, boost::function<void()> const& end_callback )
{
	player_logic_base_state* state = static_cast_checked< player_logic_base_state* >( m_logic.states( ).front( ) );
	while ( state )
	{
		if ( state->id( ) == type_sprint )
			static_cast_checked< player_logic_sprint_state* >( state )->set_callbacks( start_callback, end_callback );

		state = static_cast_checked< player_logic_base_state* >( state->next );
	}
}

} // namespace survarium
