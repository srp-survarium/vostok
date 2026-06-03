////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_state.h>
#include <vostok/game_core/weapon_core_shotgun_reload_base_substate.h>
#include <vostok/ai/fsm.h>

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

	// FUNCTION BODY
	// <0>
	// <0x599737>|0x007|+0x022:'44'		m_logic->set_initial_state( m_logic->states().front() );
	// <0x599759>|0x029|+0x00a:'45'		m_animation_has_been_ended = false;
	// ******
	// TARGET @0x07: mov eax,[m_logic]; call finalize_impl(=states); call operator[](=front); push eax; set_initial_state
	// BASE: inlines front() -> extra [ebp-8] temp + reads list members in-line (sub esp,0Ch).
}

// STATE[78.57%|PARTIAL]: LTCG cross-module inline-vs-call of animation_playback_state::reset().
// Both statements + offsets match; the target emits `add eax,120h; call reset` (out-of-line),
// but reset() is a header-inline stub so our LTCG ELIDES it at this call site (the same
// uncontrollable inline-vs-call documented at animation/type_definitions.h:40 for
// weapon_core_base_state::finalize). The 3 missing reset instrs are the sole residual.
void weapon_core_shotgun_reload_state::finalize( )
{
	m_animation_playback_state.reset( );
	m_logic->set_initial_state( 0 );

	// FUNCTION BODY
	// <0x599777>|0x007|+0x00d:'50'		m_animation_playback_state.reset();
	// <0x599784>|0x014|+0x010:'51'		m_logic->set_initial_state( 0 );
	// ******
	// TARGET @0x07: mov eax,[ebp-4]; add eax,120h; call animation_playback_state::reset  (MISSING in base - LTCG elided)
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core_shotgun_reload_state::serialize(vostok::network_core::udp_match_packet&) const
void weapon_core_shotgun_reload_state::serialize( network_core::udp_match_packet& packet ) const
{
	// LOCALS
	// u8 							state_id
	// bool 						found
	// ai::fsm_state const* 		current
	// ai::fsm_state const* 		i<1>
	// ******

	// FUNCTION BODY
	// <0x599839>|0x009|+0x004:'56'
	// <0x59983d>|0x00d|+0x004:'57'
	// <0x599841>|0x011|+0x00f:'58'
	// <0x599850>|0x020|+0x02f|[1]:'59'
	// <0x59987f>|0x04f|+0x008:'60'
	// <0x599887>|0x057|+0x004:'61'
	// <0x59988b>|0x05b|+0x002:'62'
	// <0>
	// <0x59988d>|0x05d|+0x002:'64'
	// <0>
	// <0x59988f>|0x05f|+0x00c:'66'
	// <0x59989b>|0x06b|+0x00d:'67'
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_core_shotgun_reload_state::deserialize(vostok::network_core::packet_reader&)
void weapon_core_shotgun_reload_state::deserialize( network_core::packet_reader& reader )
{
	// LOCALS
	// u8 							target_state_id
	// u8 							state_id
	// ai::fsm_state* 				current
	// ai::fsm_state* 				i<1>
	// ******

	// FUNCTION BODY
	// <0x5997a9>|0x009|+0x00b:'72'
	// <0x5997b4>|0x014|+0x004:'73'
	// <0x5997b8>|0x018|+0x007:'74'
	// <0x5997bf>|0x01f|+0x02f|[1]:'75'
	// <0x5997ee>|0x04e|+0x00c:'76'
	// <0x5997fa>|0x05a|+0x006:'77'
	// <0x599800>|0x060|+0x002:'78'
	// <0>
	// <0x599802>|0x062|+0x002:'80'
	// <0>
	// <0x599804>|0x064|+0x00c:'82'
	// <0x599810>|0x070|+0x012:'83'
	// ******
}

// STATE[INPROGRESS]: UNBLOCKED (was a VOSTOK_UNREACHABLE_CODE placeholder referencing the no-return
// base_substate STUB -> LNK1257; real body restored now that the override is matched). objdiff lists
// no score yet (newly emitted, absent from the delink unit's function list). NOT byte-identical: the
// target (0x589db0) materializes the current_state() downcast into TWO slots before the call
// (`mov [ebp-8],edx; mov eax,[ebp-8]; mov [ebp-4],eax`, sub esp,0Ch) while our single-expression form
// emits ONE slot (`mov [ebp-4],edx`, sub esp,8). That extra slot is a NAMED `current` local in the
// target, NOT an /Od scratch temp - declare `weapon_core_shotgun_reload_base_substate* current =
// static_cast<...>( m_logic->current_state() ); return current->weapon_and_hands_expression(...);` and
// re-diff. See md.
animation::mixing::expression weapon_core_shotgun_reload_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	return static_cast< weapon_core_shotgun_reload_base_substate* >( m_logic->current_state( ) )
		->weapon_and_hands_expression( buffer, is_third_view, user_state_id, weight_driving_animation );

	// FUNCTION BODY
	// <0x589db0+0x00>|+0x009:'87'
	// <0x589db0+0x09>|+0x035:'89'	return static_cast<base_substate*>( current_state() )->weapon_and_hands_expression(...);
	// <0x589db0+0x3e>|+0x006:'90'
	// ******
	// TARGET @0x09: mov [ebp-8],edx; mov eax,[ebp-8]; mov [ebp-4],eax  (current local materialized to 2 slots, sub esp,0Ch)
	// BASE   @0x09: mov [ebp-4],edx                                    (single expression, sub esp,8) - MISSING the `current` local
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
