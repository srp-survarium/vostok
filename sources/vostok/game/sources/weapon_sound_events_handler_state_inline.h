// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_INLINE_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_INLINE_H_INCLUDED

namespace survarium {

// The four sub-buffers are carved out of sounds_buffer by sounds_count (target
// 0x94230 reload instance): first emitters [buf, buf+count) ; third emitters
// [buf+count, buf+2*count) ; first instances at (proxy_ptr*)(buf)+2*count (== buf+count*8
// bytes) ; third instances at buf+count*0x88 bytes. The max_*_instances_count args are
// dropped by LTCG (MASTER_GOLD buffer_vector keeps no m_max_end), so they do not affect
// the bytes; new_state sizes the regions.
template < typename T >
inline weapon_sound_events_handler_state< T >::weapon_sound_events_handler_state(
	weapon&			weapon,
	const float		animation_time_scale,
	resources::managed_resource_ptr const* const	animations,
	const u8		animations_count,
	void* const		sounds_buffer,
	const u8		sounds_count,
	bool			stop_sounds_on_state_finalize,
	u8				simultaneous_sounds_queue_size
)	:	super( weapon, animation_time_scale, animations, animations_count ),
		m_sound_effect(
			weapon,
			( sound::sound_emitter_ptr* )sounds_buffer,
			sounds_count,
			( sound::sound_instance_proxy_ptr* )( ( sound::sound_emitter_ptr* )sounds_buffer + 2 * sounds_count ),
			simultaneous_sounds_queue_size,
			( sound::sound_emitter_ptr* )sounds_buffer + sounds_count,
			sounds_count,
			( sound::sound_instance_proxy_ptr* )( ( u8* )sounds_buffer + sounds_count * 0x88 ),
			simultaneous_sounds_queue_size,
			stop_sounds_on_state_finalize,
			simultaneous_sounds_queue_size
		),
		m_buffer_for_sounds( sounds_buffer )
{
}

template < typename T >
inline weapon_sound_events_handler_state< T >::weapon_sound_events_handler_state(
	weapon&			weapon,
	const float		animation_time_scale,
	resources::managed_resource_ptr const* const	animations,
	const u8		animations_count,
	void* const		sounds_buffer,
	const u8		sounds_count,
	bool			stop_sounds_on_state_finalize,
	u8				simultaneous_sounds_queue_size,
	bool&			shown
)	:	super( weapon, animation_time_scale, animations, animations_count, shown ),
		m_sound_effect(
			weapon,
			( sound::sound_emitter_ptr* )sounds_buffer,
			sounds_count,
			( sound::sound_instance_proxy_ptr* )( ( sound::sound_emitter_ptr* )sounds_buffer + 2 * sounds_count ),
			simultaneous_sounds_queue_size,
			( sound::sound_emitter_ptr* )sounds_buffer + sounds_count,
			sounds_count,
			( sound::sound_instance_proxy_ptr* )( ( u8* )sounds_buffer + sounds_count * 0x88 ),
			simultaneous_sounds_queue_size,
			stop_sounds_on_state_finalize,
			simultaneous_sounds_queue_size
		),
		m_buffer_for_sounds( sounds_buffer )
{
}

template < typename T >
void weapon_sound_events_handler_state< T >::initialize( )
{
	super::initialize( );
	m_sound_effect.initialize( );
	this->m_weapon.set_animation_callback( "sound_events", &m_sound_effect, boost::bind( &weapon_sound_effect::on_sound_event, &m_sound_effect, _1 ) );
}

template < typename T >
void weapon_sound_events_handler_state< T >::finalize( )
{
	this->m_weapon.remove_animation_callback( "sound_events", &m_sound_effect );

	m_sound_effect.finalize( );
	super::finalize( );
}

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_INLINE_H_INCLUDED
