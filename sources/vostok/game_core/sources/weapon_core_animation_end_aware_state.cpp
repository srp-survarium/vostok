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

	// STRUCTURE DIFF:
	// target: 0x6ed0e0            base: 0x44f9d0
	// ; void survarium::weapon_core_animation_end_aware_state::set_animation_to_wait(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&) const ; target 2 stmts / base 2 stmts
	// 0x009 <0x25> | 0x009 <0x2c> | if ( !m_weapon.get_user()->is_replaying_history() )   SIZE
	// .. same ..
	// ; aligned 1, size-diffs 1, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH - 2/2 stmts; lone SIZE diff on the if-guard is get_user() trivial-accessor inline (base) vs call (target), LTCG residual  trail: weapon_core_animation_end-set_animation_to_wait.md
}

// STATE[90.92%|PARTIAL]: empty_stub ASSERT recovered; residual is the inline-vs-call frame shape.
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

	// CALL SITE INFO
	// <0x6fd0b3> -> void < unknown >( bool& )		on_animation_end_impl( params.interrupt_animation_player_tick )
	// ******

	// STRUCTURE DIFF:
	// target: 0x6ed050            base: 0x44f850
	// ; vostok::animation::callback_return_type_enum survarium::weapon_core_animation_end_aware_state::on_animation_end(vostok::animation::animation_callback_params&) ; target 10 stmts / base 10 stmts
	// .. same ..
	// --          | 0x010 <0x10> | if ( params.animated_object == &m_weapon )   ONLY base
	// .. same ..
	// 0x010 <0x10> | --          | L37   ONLY target
	// .. same ..
	// --          | 0x02c <0x1c> | if ( m_animation_to_wait_for == params.animation )   ONLY base
	// .. same ..
	// 0x02c <0x1b> | --          | L40   ONLY target
	// .. same ..
	// ; aligned 8, size-diffs 0, quantity-diffs 4
	// VERDICT: STRUCTURE MATCH - both if-guards sit at the same offset/size; the ONLY-target L37/L40 vs ONLY-base if-text are line-marker alignment artifacts; 90.92% residual is operator== operand-eval order, not structure  trail: weapon_core_animation_end-on_animation_end.md
}

} // namespace survarium
