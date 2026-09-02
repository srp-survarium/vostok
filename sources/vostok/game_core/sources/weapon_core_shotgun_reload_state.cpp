// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_state.h>
#include <vostok/ai/fsm.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/game_core/weapon_core_shotgun_reload_base_substate.h>
#include <vostok/game_core/weapon_core_shotgun_reload_finish_substate.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

weapon_core_shotgun_reload_state::weapon_core_shotgun_reload_state(
	weapon_core&								weapon,
	weapon_core_shotgun_reload_base_substate*	reload_start,
	weapon_core_shotgun_reload_base_substate*	reload_one_round,
	weapon_core_shotgun_reload_base_substate*	reload_finish
) :
	weapon_core_base_state( weapon, true ),
	m_logic( 0 ),
	m_delete_substates_on_destruction( true )
{
	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;
	initialize_logic( reload_start, reload_one_round, reload_finish );
}

weapon_core_shotgun_reload_state::~weapon_core_shotgun_reload_state( )
{
	if ( m_delete_substates_on_destruction )
		while ( ai::fsm_state* state = m_logic->pop_state( ) )
		{
			VOSTOK_DELETE_IMPL( g_allocator, state );
		}

	VOSTOK_DELETE_IMPL( g_allocator, m_logic );
}

void weapon_core_shotgun_reload_state::initialize( )
{
	m_logic->set_initial_state( m_logic->states( ).front( ) );
	m_animation_has_been_ended = false;
}

void weapon_core_shotgun_reload_state::finalize( )
{
	m_animation_playback_state.reset( );
	m_logic->set_initial_state( 0 );
}

void weapon_core_shotgun_reload_state::serialize( network_core::udp_match_packet& packet ) const
{
	u8							state_id	= 0;
	bool						found		= false;
	ai::fsm_state const* const	current		= m_logic->current_state( );

	for ( ai::fsm_state const* i = m_logic->states( ).front( ); i; i = i->next, ++state_id )
	{
		if ( i == current )
		{
			found	= true;
			break;
		}
	}

	ASSERT( UNKNOWN_EXPRESSION_T( found ) );

	packet.append( state_id );
}

void weapon_core_shotgun_reload_state::deserialize( network_core::packet_reader& reader )
{
	u8 const				target_state_id	= reader.r< bool >( );
	u8						state_id		= 0;
	ai::fsm_state*			current			= NULL;

	for ( ai::fsm_state* i = m_logic->states( ).front( ); i; i = i->next, ++state_id )
	{
		if ( state_id == target_state_id )
		{
			current	= i;
			break;
		}
	}

	ASSERT( UNKNOWN_EXPRESSION_T( current ) );

	m_logic->set_initial_state( current );
}

animation::mixing::expression weapon_core_shotgun_reload_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	const bool								is_third_view,
	const weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// claude@MATCH: chained cast-forward matches the target's SOURCE STRUCTURE - 1 statement,
	// ZERO named locals (`--view structure`: target has no locals block). The byte-100% variant
	// `T* state = ...; U* current = ...; return current->...` is rejected: it fabricates two
	// locals (state/current) the target never records. Residual SIZE -0x6 (~90%) is the target
	// materializing 2 stack temps our build elides - non-steerable, and preferred over phantom locals.
	// sushi@TODO: recover the exact temp shape via vostok-versions; the structure verdict does not
	// yet compare locals (a follow-up PR adds locals name+count to the tool + worker).
	// See patterns/current-state-cast-forward-materialized-locals.md.
	return static_cast< weapon_core_shotgun_reload_base_substate* >( m_logic->current_state( ) )->weapon_and_hands_expression( buffer, is_third_view, user_state_id, weight_driving_animation );
}

// bool survarium::true_predicate()
static bool true_predicate( )
{
	return true;
}

void weapon_core_shotgun_reload_state::initialize_logic( weapon_core_shotgun_reload_base_substate* reload_start, weapon_core_shotgun_reload_base_substate* reload_one_round, weapon_core_shotgun_reload_base_substate* reload_finish )
{
	m_logic = VOSTOK_NEW_IMPL( g_allocator, ai::fsm );

	reload_start->set_animation_playback_state_ptr( &m_animation_playback_state );
	reload_one_round->set_animation_playback_state_ptr( &m_animation_playback_state );
	reload_finish->set_animation_playback_state_ptr( &m_animation_playback_state );

	m_logic->add_state( reload_start );
	m_logic->add_state( reload_one_round );
	m_logic->add_state( reload_finish );

	static_cast< weapon_core_shotgun_reload_finish_substate* >( reload_finish )->set_owner_ready_for_transition( &m_animation_has_been_ended );

	m_logic->add_transition( reload_start, reload_one_round, boost::bind< bool >( &true_predicate ) );
	m_logic->add_transition( reload_one_round, reload_finish, boost::bind( &weapon_core_shotgun_reload_state::finish_reload_predicate, this ) );
}

// claude@NOTE: structure matched (1 stmt @0x14d) and the boolean logic is byte-faithful.
// Two residuals, both rooted in another unit's header (weapon_core), so out of scope here:
//   (1) the target reads m_weapon.m_ammo_in_magazine (+0x47a) and m_weapon.m_is_round_chambered
//       (+0x48e) INLINE (direct member access => weapon_core befriends this state), but the
//       state is not a friend, so ammo_in_magazine()/round_is_chambered() are CALLed instead.
//   (2) the target keeps m_weapon.get_target() out-of-line (a `call`), but our inline getter
//       (return m_target) folds to a direct read here.
// Both need cross-unit weapon_core.h changes (friend decl + out-of-line get_target) that would
// touch weapon_core's own matches; left for the weapon_core owner.
bool weapon_core_shotgun_reload_state::finish_reload_predicate( ) const
{
	return m_weapon.ammo_in_magazine( ) == m_weapon.get_magazine_capacity( )
		|| m_weapon.ammunition( )->amount( ) == 0
		|| ( ( m_weapon.ammo_in_magazine( ) + ( m_weapon.round_is_chambered( ) != 0 ) ) != 0
			&& ( m_weapon.get_target( ) == weapon_target_fire || m_weapon.get_target( ) == weapon_target_aim_fire ) );
}

void weapon_core_shotgun_reload_state::execute( )
{
	m_logic->tick( );
}

} // namespace survarium
