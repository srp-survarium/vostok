////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_sound_effect.h"

#include <algorithm>		// std::find over the sound-instance list

namespace survarium {

weapon_sound_effect::weapon_sound_effect(
	weapon&								weapon,
	sound::sound_emitter_ptr* const		first_view_sounds_emitters,
	const u8							first_view_sounds_emitters_count,
	sound::sound_instance_proxy_ptr* const	first_view_sounds_instances,
	const u8							max_first_view_sounds_instances_count,
	sound::sound_emitter_ptr* const		third_view_sounds_emitters,
	const u8							third_view_sounds_emitters_count,
	sound::sound_instance_proxy_ptr* const	third_view_sounds_instances,
	const u8							max_third_view_sounds_instances_count,
	bool								stop_sounds_on_state_finalize,
	u8									simultaneous_sounds_queue_size
)
	:	m_first_view_sounds				( first_view_sounds_emitters, first_view_sounds_emitters_count, first_view_sounds_instances, max_first_view_sounds_instances_count ),
		m_third_view_sounds				( third_view_sounds_emitters, third_view_sounds_emitters_count, third_view_sounds_instances, max_third_view_sounds_instances_count ),
		m_weapon						( weapon ),
		m_stop_sounds_on_state_finalize	( stop_sounds_on_state_finalize ),
		m_sounds_counter				( 0 ),
		m_simultaneous_sounds_queue_size( simultaneous_sounds_queue_size )
{
}

void weapon_sound_effect::initialize( )
{
	m_sounds_counter = 0xff;
}

// claude@NOTE: structure faithful (guard + 2 clear_instances, both inlining to
// buffer_vector::destroy). Capped: target folds the `if` condition into the prologue
// block (no line-table row), while our base emits `push esi` + a separate line for the
// `if` -> 1 extra statement / 1-byte SIZE. Codegen line-attribution residual, not
// source-steerable.
void weapon_sound_effect::finalize( )
{
	if ( m_stop_sounds_on_state_finalize )
	{
		m_first_view_sounds.clear_instances( );
		m_third_view_sounds.clear_instances( );
	}
}

void on_sound_finished(
	buffer_vector< sound::sound_instance_proxy_ptr >&	instances,
	sound::sound_instance_proxy const&		instance
)
{
	sound::sound_instance_proxy_ptr* const found = std::find( instances.begin( ), instances.end( ), &instance );
	instances.erase( found );
}

// claude@NOTE: PARKED (cross-module-blocked) - heavy 0x2ee-byte sound-event state
// machine (26 stmts): u8 domain_data guard (params+0x14) selecting the
// m_sounds_counter modulo update, then two near-identical first/third-view playback
// branches (emit_hud_sound vs emit_point_sound, intrusive_ptr::operator= refcount
// churn, boost::bind(on_sound_finished, ref(instances), ref(instance)) into the
// instance's boost::function at +0x190, buffer_vector::erase, then a virtual call at
// vtbl+0x1C). Wall: the branch dereferences UNRECOVERED structures - m_weapon (+0x20)
// -> base_game_scene (m_game_scene at weapon+0xFC0) -> sound world/listener at
// scene+0xA8/+0x3B8, plus weapon_core members at weapon+0x40C/+0x148/+0x1B4 that are
// inside weapon_core's not-yet-recovered region (weapon.h members start at 0x0498).
// Needs the base_game_scene sound-listener accessors + the weapon_core accessor map
// before the playback branch can be spelled faithfully. Structure @0x5bd340.
// STATE[STUB]
animation::callback_return_type_enum weapon_sound_effect::on_sound_event( animation::animation_callback_params& params )
{
	return animation::callback_return_type_call_me_again;
}

} // namespace survarium
