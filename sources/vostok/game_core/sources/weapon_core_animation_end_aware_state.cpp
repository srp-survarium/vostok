////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_animation_end_aware_state.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.set/remove_animation_callback, get_user
#include <vostok/game_core/base_player.h>		// get_user()->is_replaying_history()

namespace survarium {

// STATE[100%|DONE]
// void survarium::weapon_core_animation_end_aware_state::initialize()
void weapon_core_animation_end_aware_state::initialize( )
{
	// line 19 inlines boost::bind -> boost::function1 ctor -> set_animation_callback
	// -> the function temp's clear() (dtor): all byte-for-byte at 100%.
	m_animation_has_been_ended = false;
	m_weapon.set_animation_callback(
		animation::channel_id_on_animation_end,
		this,
		boost::bind( &weapon_core_animation_end_aware_state::on_animation_end, this, _1 )
	);
}

// STATE[100%|DONE]
// void survarium::weapon_core_animation_end_aware_state::finalize()
void weapon_core_animation_end_aware_state::finalize( )
{
	m_weapon.remove_animation_callback( animation::channel_id_on_animation_end, this );
}

// STATE[77.33%|PARTIAL]: LTCG inlines the trivial get_user() accessor (unsteerable)
// void survarium::weapon_core_animation_end_aware_state::set_animation_to_wait(vostok::resources::managed_resource_ptr const&) const
void weapon_core_animation_end_aware_state::set_animation_to_wait( resources::managed_resource_ptr const& animation ) const
{
	// claude@MATCH: the body is correct (the is_replaying_history virtual @vtbl+0x5c
	// and the operator= match byte-for-byte). The ONLY real divergence is LTCG
	// inline-vs-call of weapon_core::get_user(): the target keeps get_user standalone
	// (@0x9b330, emits `call get_user`), but our /GL LTCG inlined the trivial header
	// accessor `{ return m_user; }` whole-program (no standalone symbol in base) into
	// `mov eax,[m_weapon+44Ch]`, which also shifts the [ebp-XX] frame slots. Unsteerable
	// from this function's source (loop_performance.md trivial-accessor inline-vs-call).
	if ( !m_weapon.get_user()->is_replaying_history() )
		m_animation_to_wait_for = animation;

	// CALL SITE INFO
	// <0x6fd105> -> bool <unknown>() const  (base_player::is_replaying_history, vtbl+0x5c)
	// ******

	// FUNCTION BODY
	// <0x6fd0e9>|0x009|+0x025:'29'
	// <0x6fd10e>|0x02e|+0x012:'30'
	// ******

	// --view target divergence (right=target, base inlines get_user):
	//   target: mov eax,[this+128h]; mov eax,[eax+128h]; call weapon_core::get_user
	//   base:   ... mov edx,[ecx+128h]; mov eax,[edx+44Ch]   (m_user inlined)
}

// STATE[78.03%|PARTIAL]: LTCG-folded empty out-of-line call at line 38 (unreproducible)
// vostok::animation::callback_return_type_enum survarium::weapon_core_animation_end_aware_state::on_animation_end(vostok::animation::animation_callback_params&)
animation::callback_return_type_enum weapon_core_animation_end_aware_state::on_animation_end( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = false;
	if ( params.animated_object == &m_weapon )
	{
		// claude@MATCH: target emits the documented LTCG-folded empty out-of-line call
		// here (`mov byte[ebp-1],0; lea eax,[ebp-1]; call <empty/finalize_impl>`, source
		// line 38) - same artifact as weapon_core_show/hide_state_base, not reproducible
		// from this function's source (assembly_patterns.md). Every other statement matches.
		if ( m_animation_to_wait_for == params.animation )
		{
			m_animation_has_been_ended = true;
			on_animation_end_impl( params.interrupt_animation_player_tick );
		}
	}

	return animation::callback_return_type_call_me_again;

	// CALL SITE INFO
	// <0x6fd0b3> -> void <unknown>(bool&)  (on_animation_end_impl, this vtbl+0x20)
	// ******

	// FUNCTION BODY
	// <0x6fd059>|0x009|+0x007:'35'
	// <0>
	// <0x6fd060>|0x010|+0x010:'37'
	// <0x6fd070>|0x020|+0x00c:'38'	<- LTCG-folded empty call, see claude@MATCH
	// <0>
	// <0x6fd07c>|0x02c|+0x01b:'40'
	// <0x6fd097>|0x047|+0x00a:'41'
	// <0x6fd0a1>|0x051|+0x014:'42'
	// <0>
	// <1>
	// <2>
	// <0x6fd0b5>|0x065|+0x002:'46'
	// ******
}

} // namespace survarium
