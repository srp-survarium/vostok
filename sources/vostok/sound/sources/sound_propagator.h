////////////////////////////////////////////////////////////////////////////
//	Created		: 14.06.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_PROPAGATOR_H_INCLUDED
#define SOUND_PROPAGATOR_H_INCLUDED

#include <vostok/sound/sound.h>
#include <vostok/fixed_vector.h>
#include <vostok/sound/sound_propagator_emitter.h>

struct IXAudio2SubmixVoice;

namespace vostok {

namespace memory {
class writer;
} // namespace memory

namespace sound {

class sound_instance_proxy_internal;
class sound_voice;
struct sound_voice_params;

class new_sound_propagator : private noncopyable
{
public:
	enum propagation_state
	{
		propagating,
		propagating_paused,
		propagating_finished
	}; // enum propagation_state

public:
											new_sound_propagator			(
												float3 const&						start_position,
												float3 const&						listener_position,
												playback_mode						mode,
												u32									playback_id,
												u32									playing_offset,
												u32									before_playing_offset,
												u32									after_playing_offset,
												sound_instance_proxy_internal&		proxy,
												sound_propagator_emitter const&		emitter
											);
											~new_sound_propagator			( );

			void							tick							( u32 time_delta_in_msec );

	inline	bool							can_be_deleted					( ) const { return m_propagation_state == propagating_finished; }

			void							distribute_voices				( u32 count, vectora< sound_voice_params > const& voices_params );

	inline	u32								get_length_with_offsets			( ) const { return m_sound_length_with_offsets; }

	inline	void							set_quality						( u32 quality );
	inline	void							set_as_callback_executer		( bool value ) { m_is_callback_executer = value; }

	inline	bool							is_sound_producing				( ) const { return m_voice != 0; }

			void							pause_propagation				( );
			void							resume_propagation				( );
			void							stop_propagation				( );
			u32								stop_produce					( );

	inline	float							get_percived_loudness			( ) const { return m_perceived_loudness; }
	inline	void							set_percived_loudness			( float value ) { m_perceived_loudness = value; }
	inline	float							get_attenuated_loudness			( ) const { return m_attenuated_loudness; }
	inline	void							set_attenuated_loudness			( float value ) { m_attenuated_loudness = value; }

	inline	float							get_sound_spl					( float distance ) const;
	inline	float							get_sound_rms_value				( ) const;
	inline	resources::managed_resource_ptr const&	get_sound_rms					( ) const;

	inline	void							serialize						( memory::writer& writer );

	inline	sound_instance_proxy_internal&	get_proxy						( ) { return m_proxy; }

private:
			u32								sound_playing_time				( ) const;
			u32								sound_playing_time_with_offsets	( ) const;

			sound_voice*					attach_voice					( u32 offset );
			void							attach_voices					( u32 count, vectora< sound_voice_params > const& voices_params );
			void							detach_voices					( u32 count );
			void							detach_voice					( sound_voice* voice );

			void							set_voice_channel_matrix		( sound_voice* voice, float const* channel_matrix, float lp_coeff );

			void							execute_callback				( );

public:
	new_sound_propagator*					m_next_for_proxies;
private:
	sound_voice*							m_voice;
	fixed_vector< IXAudio2SubmixVoice*, 4 >	m_submix_voices;
	float3									m_start_position;
	sound_instance_proxy_internal&			m_proxy;
	sound_propagator_emitter const&			m_emitter;
	playback_mode							m_mode;
	bool									m_is_callback_executer;
	s32										m_playing_offset;
	u32										m_playback_id;
	u32										m_end_propagation_time;
	u32										m_time_to_listener;
	u32										m_propagation_time;
	u32										m_sound_length;
	u32										m_sound_length_with_offsets;
	u32										m_before_playing_offsets;
	u32										m_after_playing_offsets;
	propagation_state						m_propagation_state;
	float									m_perceived_loudness;
	float									m_attenuated_loudness;
}; // class new_sound_propagator

STATIC_SIZE_ASSERT( new_sound_propagator, 0x6C );

typedef intrusive_list	<	new_sound_propagator,
							new_sound_propagator*,
							&new_sound_propagator::m_next_for_proxies,
							threading::single_threading_policy
						>	new_sound_propagator_list;

inline void new_sound_propagator::serialize	( memory::writer& )
{
}

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_PROPAGATOR_H_INCLUDED
