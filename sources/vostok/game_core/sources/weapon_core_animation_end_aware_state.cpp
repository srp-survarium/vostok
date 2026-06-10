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
// STRUCTURE DIFF[target 0x6ed0e0 | base 0x44fb10]: target 2 / base 2 stmts
// 0x009 <0x25> | 0x009 <0x2c> | if ( !m_weapon.get_user()->is_replaying_history() )   SIZE
// .. same ..
// ; aligned 1, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE on the if-guard is get_user() trivial-accessor inline (base) vs out-of-line call (target), non-steerable LTCG. trail: weapon_core_animation_end_aware_state.md
void weapon_core_animation_end_aware_state::set_animation_to_wait( resources::managed_resource_ptr const& animation ) const
{
	if ( !m_weapon.get_user()->is_replaying_history() )
		m_animation_to_wait_for = animation;
}

// STATE[90.92%|PARTIAL]: empty_stub ASSERT recovered; residual is operator== operand-eval order.
// STRUCTURE DIFF[target 0x6ed050 | base 0x44f990]: target 10 / base 10 stmts
// --          | 0x010 <0x10> | if ( params.animated_object == &m_weapon )   ONLY base
// 0x010 <0x10> | --          | L37   ONLY target
// --          | 0x02c <0x1c> | if ( m_animation_to_wait_for == params.animation )   ONLY base
// 0x02c <0x1b> | --          | L40   ONLY target
// .. same ..
// ; aligned 8, size-diffs 0, quantity-diffs 4
// VERDICT: STRUCTURE MATCH (shape ok) - both if-guards sit at identical offset/size; the ONLY-target L37/L40 vs ONLY-base if-text rows are line-marker alignment artifacts; residual is the resource_ptr operator== operand-evaluation order (target evals the arg before the object address), not source structure. trail: weapon_core_animation_end_aware_state.md
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
}

} // namespace survarium
