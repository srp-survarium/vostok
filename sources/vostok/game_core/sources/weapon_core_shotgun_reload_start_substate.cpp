////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_start_substate.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/animation/animation_callback.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_shotgun_reload_start_substate::weapon_core_shotgun_reload_start_substate(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_shotgun_reload_base_substate(
		weapon, animation_time_scale,
		animations,
		animations_count,
		animation::mixing::play_once_and_freeze_at_end,
		3,
		"shotgun-start_reload",
		"reload_start(stand)",
		"reload_start(crouch)",
		"reload_start(jump)"
	)
{
}

// STATE[97.45%|PARTIAL]: only residual is the documented inline-vs-call of the trivial
// weapon_core::round_is_chambered() accessor (header STUB inlines `m_is_round_chambered`,
// target keeps it out-of-line) - same wall as weapon_core_reload_state_base::initialize (92%);
// weapon_core.h is out of this unit's scope. The folded boost::bind/assign_to/function::clear
// symbol-name diffs are byte-identical ICF folds. See md.
void weapon_core_shotgun_reload_start_substate::initialize( )
{
	m_animation_ended = false;
	m_weapon.set_animation_callback(
		animation::channel_id_on_animation_end,
		this,
		boost::bind( &weapon_core_shotgun_reload_start_substate::on_animation_end, this, _1 )
	);

	if ( !m_weapon.deserializing( ) &&
		 m_weapon.chamber_a_round_on_reload( ) &&
		 m_weapon.round_is_chambered( ) )
		m_weapon.unload_chambered_round( );

	// FUNCTION BODY
	// <0>
	// <0x59e4ea>|0x00a|+0x00a:'26'		m_animation_ended = false;
	// <0x59e4f4>|0x014|+0x0c6:'27'		set_animation_callback(...);
	// <0>
	// <0x59e5ba>|0x0da|+0x01a:'29'		if ( !deserializing() &&
	// <0>
	// <0x59e5d4>|0x0f4|+0x02f:'31'		chamber_a_round_on_reload() && round_is_chambered() )
	// <0x59e603>|0x123|+0x00e:'32'		unload_chambered_round();
	// <0>
	// ******
}

// STATE[83.42%|PARTIAL]: residual is the inline/elide-vs-call of animation_playback_state::reset()
// - reset() has a REAL 2-store body (zeroes m_animation_playback_state @ this+0x148) kept out-of-line
// in target (rva 0x087f60) but absent/elided in our base (LTCG), NOT a no-op header stub - plus the
// benign dummy::nonnull/finalize_impl ICF fold for the compiled-out ASSERT. Both unsteerable, outside
// this unit's scope (same class as weapon_core_shotgun_reload_state::finalize 78%). See md.
void weapon_core_shotgun_reload_start_substate::finalize( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_animation_playback_state->reset( );
	m_weapon.remove_animation_callback( animation::channel_id_on_animation_end, this );

	// FUNCTION BODY
	// <0x59e4a9>|0x009|+0x01a:'38'	ASSERT + m_animation_playback_state->reset();
	// <0x59e4c3>|0x023|+0x014:'39'	remove_animation_callback(...);
	// ******
}

// STATE[100%|DONE]
bool weapon_core_shotgun_reload_start_substate::is_ready_for_transition( ) const
{
	return m_animation_ended;
}

// STATE[83%|PARTIAL]: residuals are the benign dummy::nonnull/finalize_impl ICF fold (compiled-out
// ASSERT, byte-identical at link) and the LTCG operand scheduling of intrusive_ptr::operator== (target
// computes the LHS m_animation_to_wait_for address before loading the params.animation arg; ours pushes
// the arg first) - a call-boundary temp/arg residual. Structure matches statement-for-statement. See md.
animation::callback_return_type_enum weapon_core_shotgun_reload_start_substate::on_animation_end( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = false;
	if ( params.animated_object == &m_weapon )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		if ( m_animation_to_wait_for == params.animation )
		{
			m_animation_ended = true;
			params.interrupt_animation_player_tick = true;
		}
	}

	return animation::callback_return_type_call_me_again;
}

} // namespace survarium
