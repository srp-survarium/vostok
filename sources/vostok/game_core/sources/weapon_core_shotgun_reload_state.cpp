////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_state.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/ai/fsm.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/game_core/weapon_core_shotgun_reload_base_substate.h>
#include <vostok/game_core/weapon_core_shotgun_reload_finish_substate.h>

namespace survarium {

// STATE[100%|DONE]
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

// STATE[88.24%|PARTIAL]: STRUCTURE MATCH (5/5). Residual is LTCG inline-vs-call of the
// base dtor chain + VOSTOK_DELETE_IMPL(g_allocator, m_logic) template depth
// (delete_helper_impl<...,ai::fsm,...> vs target's <...,intrusive_list,...>). Same wall as
// jump_logic dtor.
weapon_core_shotgun_reload_state::~weapon_core_shotgun_reload_state( )
{
	if ( m_delete_substates_on_destruction )
	{
		while ( ai::fsm_state* state = m_logic->pop_state( ) )
		{
			VOSTOK_DELETE_IMPL( g_allocator, state );
		}
	}

	VOSTOK_DELETE_IMPL( g_allocator, m_logic );
}

// STATE[50.88%|PARTIAL]: LTCG inline-vs-call of fsm::states()/fsm_state_list::front().
// Every statement and member offset matches; the target keeps states()+front() OUT-OF-LINE
// (frame = 1 local: `call finalize_impl`(=states fold) + `call operator[]`(=front)), while our
// /GL LTCG inlines front() into the body (frame = 3 locals, reads the list members directly).
// Same unsteerable inline-vs-call class as reload_state_base::initialize (assembly_patterns.md);
// no source change steers it.
void weapon_core_shotgun_reload_state::initialize( )
{
	m_logic->set_initial_state( m_logic->states( ).front( ) );
	m_animation_has_been_ended = false;

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE +0x5 | 55 | m_logic->set_initial_state( m_logic->states( ).front( ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is states()/front() kept out-of-line in
	// target vs inlined in base (extra temp + direct list-member reads), non-steerable LTCG.
}

// STATE[55.50%|PARTIAL]: LTCG cross-module inline-vs-call of animation_playback_state::reset().
// Both statements + offsets match; the target emits `add eax,120h; call reset` (out-of-line),
// but reset() is a header-inline whose body our LTCG INLINES at this call site (the same
// uncontrollable inline-vs-call documented at animation/type_definitions.h:40 for
// weapon_core_base_state::finalize).
void weapon_core_shotgun_reload_state::finalize( )
{
	m_animation_playback_state.reset( );
	m_logic->set_initial_state( 0 );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE +0x17 | 74 | m_animation_playback_state.reset( );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is reset() out-of-line promoted call in
	// target vs inlined body in base, non-steerable LTCG.
}

// STATE[77.95%|PARTIAL]: residuals are the states()/front() out-of-line calls in the for-init
// (same wall as initialize) and append(u8) kept out-of-line in target vs inlined to the
// temp+append(pcvoid,u32) leaf in base, both non-steerable LTCG.
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

	// STRUCTURE DIFF: target 10 stmts / base 10 stmts
	// SIZE -0x1 | 89 | for ( ai::fsm_state const* i = m_logic->states( ).front( ); i; i = i->next, ++state_id )
	// SIZE +0x7 | 100 | packet.append( state_id );
	// VERDICT: STRUCTURE MATCH (shape ok) - the ++state_id merged into the for head matches the
	// target's .inc block (was an 11-vs-10 BASE_ONLY row); residual SIZEs are the
	// states()/front() and append(u8) inline-vs-call walls, non-steerable.
}

// STATE[65.17%|PARTIAL]: sole residual is packet_reader::r<bool> kept out-of-line in target
// (0xb call) vs inlined read+advance in base (0x20) - the documented r<T> cross-module
// inline wall - plus the same states()/front() wall in the for-init.
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

	// STRUCTURE DIFF: target 10 stmts / base 10 stmts
	// SIZE +0xb | 113 | u8						target_state_id	= reader.r< bool >( );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the r<bool> inline-vs-call wall,
	// non-steerable.
}

// STATE[100%|DONE]: scored 0/unpaired until the declaration moved to the private section
// (target mangles it @@EBE - private virtual); with the access letters agreeing it pairs
// and scores 100. The two-slot materialization (`state` [ebp-8], `current` [ebp-4],
// sub esp,0Ch) comes from a SEPARATE named local for the current_state() result, not the inlined cast.
animation::mixing::expression weapon_core_shotgun_reload_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	ai::fsm_state* state = m_logic->current_state( );
	weapon_core_shotgun_reload_base_substate* current = static_cast< weapon_core_shotgun_reload_base_substate* >( state );
	return current->weapon_and_hands_expression( buffer, is_third_view, user_state_id, weight_driving_animation );
}

// STATE[100%|DONE]: body is `return true;` (3 bytes: mov al,1; ret). Will score once
// initialize_logic references it (currently compiled away as unreferenced, which is correct).
static bool true_predicate( )
{
	return true;
}

// STATE[92.99%|PARTIAL]: STRUCTURE MATCH (10/10). Sole residuals are LTCG inline-vs-call of
// set_animation_playback_state_ptr, set_owner_ready_for_transition, and the boost::bind<bool>
// (same fsm::states()/front() / boost::bind inline wall as jump_logic::initialize_logic).
void weapon_core_shotgun_reload_state::initialize_logic( weapon_core_shotgun_reload_base_substate* reload_start, weapon_core_shotgun_reload_base_substate* reload_one_round, weapon_core_shotgun_reload_base_substate* reload_finish )
{
	m_logic = VOSTOK_NEW_IMPL( g_allocator, ai::fsm );

	reload_start->set_animation_playback_state_ptr( &m_animation_playback_state );
	reload_one_round->set_animation_playback_state_ptr( &m_animation_playback_state );
	reload_finish->set_animation_playback_state_ptr( &m_animation_playback_state );

	m_logic->add_state( reload_start );
	m_logic->add_state( reload_one_round );
	m_logic->add_state( reload_finish );

	static_cast< weapon_core_shotgun_reload_finish_substate* >( reload_finish )->set_owner_ready_for_transition( &m_is_ready_to_be_deactivated );

	m_logic->add_transition( reload_start, reload_one_round, boost::bind< bool >( &true_predicate ) );
	m_logic->add_transition( reload_one_round, reload_finish, boost::bind( &weapon_core_shotgun_reload_state::finish_reload_predicate, this ) );

	// STRUCTURE DIFF: target 10 / base 10 stmts
	// SIZE -0x6 | 167 | reload_finish->set_animation_playback_state_ptr( &m_animation_playback_state );
	// SIZE -0x7 | 173 | static_cast< weapon_core_shotgun_reload_finish_substate* >( reload_finish )->set_owner_ready_for_transition( &m_is_ready_to_be_deactivated );
	// SIZE -0xc | 175 | m_logic->add_transition( reload_start, reload_one_round, boost::bind< bool >( &true_predicate ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - SIZE rows are LTCG inline-vs-call: target keeps set_animation_playback_state_ptr, set_owner_ready_for_transition, and boost::bind<bool> out-of-line vs base inlines them. Non-steerable.
}

// STATE[62.18%|PARTIAL]: STRUCTURE MATCH (1/1 stmt). Sole SIZE diff (0x64) is
// inline-vs-call: target out-of-lines ammo_in_magazine/get_magazine_capacity/get_target
// accessors while base inlines them; intrusive_ptr temporary copy pattern differs
// (target manual ref-count, base uses set()). Non-steerable LTCG.
bool weapon_core_shotgun_reload_state::finish_reload_predicate( ) const
{
	return	m_weapon.ammo_in_magazine( ) == m_weapon.get_magazine_capacity( )
		|| (	m_weapon.ammunition( )
			&&	m_weapon.ammunition( )->amount( ) > 0
			&&	( m_weapon.m_ammo_in_magazine + ( m_weapon.m_is_round_chambered != 0 ) ) > 0
			&&	(	m_weapon.get_target( ) == weapon_target_fire
				||	m_weapon.get_target( ) == weapon_target_aim_fire
				)
			);
}

// STATE[100%|DONE]
void weapon_core_shotgun_reload_state::execute( )
{
	m_logic->tick( );
}

} // namespace survarium
