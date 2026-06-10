////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_animation_end_aware_state.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.set/remove_animation_callback, get_user
#include <vostok/game_core/base_player.h>		// get_user()->is_replaying_history()

namespace survarium {

// STATE[100%|DONE]
void weapon_core_animation_end_aware_state::initialize( )
{
	m_animation_has_been_ended = false;
	m_weapon.set_animation_callback(
		animation::channel_id_on_animation_end,
		this,
		boost::bind( &weapon_core_animation_end_aware_state::on_animation_end, this, _1 )
	);
}

// STATE[100%|DONE]
void weapon_core_animation_end_aware_state::finalize( )
{
	m_weapon.remove_animation_callback( animation::channel_id_on_animation_end, this );
}

// STATE[77.33%|PARTIAL]: residual is the inline-vs-call of the trivial weapon_core::get_user()
// accessor (target calls it out-of-line @0x9b330; base inlines it, also shifting frame slots).
void weapon_core_animation_end_aware_state::set_animation_to_wait( resources::managed_resource_ptr const& animation ) const
{
	if ( !m_weapon.get_user()->is_replaying_history() )
		m_animation_to_wait_for = animation;

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE +0x7 | 38 | if ( !m_weapon.get_user()->is_replaying_history() )
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is get_user() trivial-accessor inlined in base vs out-of-line call in target, non-steerable LTCG.
}

// STATE[90.92%|PARTIAL]: empty_stub ASSERT recovered; residual is the LTCG-promoted
// intrusive_ptr::operator== convention (target passes the arg in eax, base pushes it).
animation::callback_return_type_enum weapon_core_animation_end_aware_state::on_animation_end( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = false;
	if ( params.animated_object == &m_weapon )
	{
		ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call empty_stub` @ +0x20)
		if ( m_animation_to_wait_for == params.animation )
		{
			m_animation_has_been_ended = true;
			on_animation_end_impl( params.interrupt_animation_player_tick );
		}
	}

	return animation::callback_return_type_call_me_again;

	// STRUCTURE DIFF: target 7 stmts / base 7 stmts
	// BASE_ONLY | 54 | if ( params.animated_object == &m_weapon )
	// TRGT_ONLY | -- | (same stmt, 0x10 both sides - aligner mispair)
	// BASE_ONLY | 57 | if ( m_animation_to_wait_for == params.animation )
	// TRGT_ONLY | -- | (same stmt, base 0x1c vs target 0x1b)
	// VERDICT: STRUCTURE MATCH (shape ok) - 7/7 aligned at identical offsets; the +0x1 on the
	// inner if is the promoted operator== convention (arg in eax vs push), non-steerable.
}

} // namespace survarium
