////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_state.h>
#include <vostok/ai/fsm.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/game_core/weapon_core_shotgun_reload_base_substate.h>

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

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::~weapon_core_shotgun_reload_state()
weapon_core_shotgun_reload_state::~weapon_core_shotgun_reload_state( )
{
	// LOCALS
	// ai::fsm_state* 				state<1>
	// ******

	// FUNCTION BODY
	// <0x5998ec>|0x01c|+0x00e:'33'
	// <0x5998fa>|0x02a|+0x017|[1]:'34'
	// <0x599911>|0x041|+0x026:'35'
	// <0x599937>|0x067|+0x002:'36'
	// <0>
	// <0x599939>|0x069|+0x02c:'38'
	// ******
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
	// SIZE +0x15 | 113 | u8						target_state_id	= reader.r< bool >( );
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

// STATE[INPROGRESS]: body is `return true;` (asm @0x589720: mov al,1). Only referenced by initialize_logic
// (still STUB), so the compiler removes it as unreferenced (C4505); it scores once initialize_logic is matched.
// bool survarium::true_predicate()
static bool true_predicate( )
{
	return true;

	// FUNCTION BODY
	// <0x599723>|0x003|+0x002:'94'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::initialize_logic(survarium::weapon_core_shotgun_reload_base_substate*, survarium::weapon_core_shotgun_reload_base_substate*, survarium::weapon_core_shotgun_reload_base_substate*)
void weapon_core_shotgun_reload_state::initialize_logic( weapon_core_shotgun_reload_base_substate* reload_start, weapon_core_shotgun_reload_base_substate* reload_one_round, weapon_core_shotgun_reload_base_substate* reload_finish )
{
	// FUNCTION BODY
	// <0x599ae0>|0x010|+0x05d:'99'
	// <0x599b3d>|0x06d|+0x014:'100'
	// <0x599b51>|0x081|+0x015:'101'
	// <0x599b66>|0x096|+0x01b:'102'
	// <0x599b81>|0x0b1|+0x015:'103'
	// <0x599b96>|0x0c6|+0x015:'104'
	// <0x599bab>|0x0db|+0x015:'105'
	// <0x599bc0>|0x0f0|+0x01b:'106'
	// <0x599bdb>|0x10b|+0x065:'107'
	// <0x599c40>|0x170|+0x0ea:'108'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core_shotgun_reload_state::finish_reload_predicate() const
bool weapon_core_shotgun_reload_state::finish_reload_predicate( ) const
{
	return false;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0x599991>|0x011|+0x137:'116'
	// ******
}

// STATE[100%|DONE]
void weapon_core_shotgun_reload_state::execute( )
{
	m_logic->tick( );
}

} // namespace survarium
