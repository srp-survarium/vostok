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

// claude@NOTE: PARKED - heavy 0x2ee-byte sound-event state machine (26 stmts,
// two near-identical first/third-view playback branches): boost::function bind of
// on_sound_finished, sound_emitter::emit_point_sound, intrusive_ptr refcount churn,
// buffer_vector erase, weapon world-user/receiver lookups at large fixed offsets
// (weapon+0xFC0/+0x40C/+0x148/+0x1B4). Needs the full weapon_sound_events_handler
// playback semantics + the weapon accessor map; out of scope for this cook-cluster
// pass. Next: model the duplicated branch as a play_sound helper, recover the weapon
// world_user()/receiver() accessors, then the boost::bind site.
// STATE[STUB]
animation::callback_return_type_enum weapon_sound_effect::on_sound_event( animation::animation_callback_params& params )
{
	return animation::callback_return_type_call_me_again;
}

} // namespace survarium
