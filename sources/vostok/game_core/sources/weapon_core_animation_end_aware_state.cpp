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

	// CALL SITE INFO
	// <0x6fd105> -> bool < unknown >() const		get_user()->is_replaying_history()
	// ******

	// FUNCTION BODY[0x6fd0e0]: 2
	// <0x6fd0e9>|0x009|+0x025:'29'	if ( !m_weapon.get_user()->is_replaying_history() )	target 0x25 / base 0x2c: get_user() inline-vs-call
	// <0x6fd10e>|0x02e|+0x012:'30'	m_animation_to_wait_for = animation;
	// ******
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

	// FUNCTION BODY[0x6fd050]: 12
	// <0x6fd059>|0x009|+0x007:'35'	params.interrupt_animation_player_tick = false;
	// <0>
	// <0x6fd060>|0x010|+0x010:'37'	if ( params.animated_object == &m_weapon ) {
	// <0x6fd070>|0x020|+0x00c:'38'	ASSERT( UNKNOWN_EXPRESSION );  (compiled-out, +0x0c)
	// <0>
	// <0x6fd07c>|0x02c|+0x01b:'40'	if ( m_animation_to_wait_for == params.animation ) {	target 0x1b / base 0x1c: == operand-order inline-vs-call
	// <0x6fd097>|0x047|+0x00a:'41'	m_animation_has_been_ended = true;
	// <0x6fd0a1>|0x051|+0x014:'42'	on_animation_end_impl( params.interrupt_animation_player_tick );
	// <0>
	// <1>
	// <2>
	// <0x6fd0b5>|0x065|+0x002:'46'	return animation::callback_return_type_call_me_again;
	// ******
}

} // namespace survarium
