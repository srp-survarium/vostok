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

	// STRUCTURE DIFF[target 0x6ed0e0 | base 0x44f9d0]: target 3 / base 4 stmts
	// .. same ..
	// 0x009 <0x25> | 0x009 <0x2c> | if ( !m_weapon.get_user()->is_replaying_history() )   SIZE
	// 0x02e <0x12> | 0x035 <0x12> | m_animation_to_wait_for = animation;
	// --          | <0>         |    EMPTY only base
	// ; aligned 2, size-diffs 1, quantity-diffs 1
	// SIZE: get_user() trivial accessor inline (base) vs call @0x9b330 (target) - LTCG residual.
	// QUANTITY: EMPTY-only-base = a collapsed trailing source-line gap, whitespace artifact, not control flow.
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

	// STRUCTURE DIFF[target 0x6ed050 | base 0x44f850]: target 11 / base 12 stmts
	// .. same ..
	// --          | 0x010 <0x10> | if ( params.animated_object == &m_weapon )            ONLY base
	// .. same ..
	// 0x010 <0x10> | --          | L37                                                   ONLY target
	// .. same ..
	// --          | 0x02c <0x1c> | if ( m_animation_to_wait_for == params.animation )    ONLY base
	// .. same ..
	// 0x02c <0x1b> | --          | L40                                                   ONLY target
	// .. same ..
	// --          | <0>         |                                                        EMPTY only base
	// ; aligned 9, size-diffs 0, quantity-diffs 5
	//
	// STRUCTURE MATCH: the 5 "quantity-diffs" are an alignment artifact, NOT a real
	// divergence. The target side carries bare source-line markers (L37/L40) where the
	// base carries the resolved `if (...)` text; both `if` statements sit at the SAME
	// offset 0x010 / 0x02c with the SAME size (0x10; 0x1b vs 0x1c). Both are nested
	// brace-blocks whose `jne/je short` share the single `.1` return - count and shape
	// agree. The 90.92% residual is purely operand evaluation order inside the second
	// `if`'s intrusive_ptr::operator== (+0x2c..+0x38: this+0x138 computed before vs after
	// params.animation is pushed) - a SIZE/inline-vs-call artifact, not structure.
}

} // namespace survarium
