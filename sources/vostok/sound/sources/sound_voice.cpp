////////////////////////////////////////////////////////////////////////////
//	Created		: 20.06.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_voice.h"
#include <vostok/sound/sound_instance_proxy.h>
#include <vostok/sound/encoded_sound_interface.h>
#include <vostok/sound/sound_propagator_emitter.h>
#include <vostok/sound/sound_rms.h>
#include <vostok/sound/world_user.h>
#include <vostok/sound/single_sound.h>
#include "sound_world.h"
#include "voice_bridge.h"
#include "sound_buffer.h"
#include "sound_voice.h"

namespace vostok {
namespace sound {

enum
{
	max_queued_buffers = 3,
};

static u32 time_in_msec_to_pcm	( encoded_sound_ptr const& encoded_sound, u32 time )
{
	return	( time * (encoded_sound->get_samples_per_sec( ))) / 1000;
}

static u32 sound_voice_id		= 0;

sound_voice::sound_voice	(	s32 playing_offset_in_msec,
								u32 before_playing_quiet,
								u32 after_playing_quiet,
								sound_instance_proxy_internal& proxy,
								sound_propagator_emitter const& emitter,
								sound_spl_ptr const& spl ) :
	m_position				( 0.0f, 0.0f, 0.0f ),
	m_proxy					( proxy ),
	m_emitter				( emitter ),
	m_sound_spl				( spl ),
	m_is_playing			( false ),
	m_is_quality_changing_requested	( false ),
	m_is_conversion_buffer_ready	( false ),
	m_stream_cursor_pcm		( 0 ),
	m_next_for_delete		( 0 ),
	m_next_for_active		( 0 ),
	m_playing_offset		( playing_offset_in_msec ),
	m_buffers_queued		( 0 ),
	m_mode					( once ),
	m_before_playing_quiet	( before_playing_quiet ),
	m_after_playing_quiet	( after_playing_quiet ),
	m_dbg_id				( sound_voice_id++ ),
	m_conversion_buffer		( 0 ),
	m_is_conversion_buffer_submited( false ),
	m_conv_state			( conversion_not_requested ),
	m_current_quality		( 2 ),
	m_first_portal_id		( u32(-1) ),
	m_second_portal_id		( u32(-1) ),
	m_distance_to_listener	( 0.0f ),
	m_target_sound_quality	( 0 ),
	m_world_user			( proxy.get_world_user( ) )
{
	m_current_sound_quality	= emitter.dbg_get_encoded_sound( 0 );

	m_channels_num			= m_current_sound_quality->get_channels_num	( );
	u32 samples_per_sec	= m_current_sound_quality->get_samples_per_sec( );

	m_voice				= m_world_user.get_sound_world()->get_voice( this, proxy.get_sound_scene().get_submix_voice(), m_channels_num, samples_per_sec );
}

sound_voice::~sound_voice	( )
{
	R_ASSERT						( m_buffers_queued == 0 );
	m_world_user.get_sound_world()->free_voice( m_voice );
}


void sound_voice::play		( playback_mode mode )
{
	m_mode					= mode;

	if ( m_mode == once )
	{
		if ( m_playing_offset > 10  )
		{
			m_stream_cursor_pcm				= find_nearest_adjective_pcm_offset ( );
		}
		else
		{
			m_stream_cursor_pcm				= 0;
		}
	}
	else if ( m_mode == looped )
	{
		if ( m_playing_offset < (s32)m_before_playing_quiet )
		{
			submit_muted_buffers		( m_before_playing_quiet - m_playing_offset );
		}
		else if ( m_playing_offset < m_before_playing_quiet + m_current_sound_quality->get_length_in_msec() )
		{
			m_playing_offset				-= m_before_playing_quiet;
			m_stream_cursor_pcm				= find_nearest_adjective_pcm_offset ( );
		}
		else
		{
			submit_muted_buffers		( m_before_playing_quiet * 2 + m_after_playing_quiet + (u32)m_current_sound_quality->get_length_in_msec() - m_playing_offset );
		}
	}

	m_is_playing								= true;
	refill_buffers								( );
	m_voice->start								( );
}

void sound_voice::stop		( )
{
	m_is_playing					= false;
	m_voice->stop					( );
	m_voice->flush_source_buffers	( );
}

void sound_voice::refill_buffers	( )
{
	u64 pcm_total					= m_current_sound_quality->get_length_in_pcm();

	while ( ( m_buffers_queued  < max_queued_buffers ) && ( m_stream_cursor_pcm < pcm_total ) )
	{
		sound_buffer* buffer = m_world_user.get_sound_world()->get_sound_buffer( m_current_sound_quality, m_stream_cursor_pcm, m_stream_cursor_pcm );
		m_voice->submit_source_buff		( buffer );
		if ( m_mode == looped && pcm_total == m_stream_cursor_pcm )
		{
			submit_muted_buffers		( m_before_playing_quiet + m_after_playing_quiet );
			m_stream_cursor_pcm			= 0;
		}

		threading::interlocked_increment( m_buffers_queued );
	}

}

void sound_voice::sumbmit_muted_sound_buffers_on_first_playing	( )
{
}

bool sound_voice::can_be_deleted	( ) const
{
	return m_buffers_queued == 0; 
}

void sound_voice::set_output_matrix	( float const* level_matrix )
{
	R_ASSERT						( m_voice != 0 );
	if ( m_channels_num == 1 )
		m_voice->set_output_matrix	( level_matrix );
	else
	if ( m_channels_num == 2 )
		m_voice->set_channel_volumes( m_channels_num, level_matrix );
}

u32 sound_voice::find_nearest_adjective_pcm_offset	( ) const
{
	u32 pcm_cursor					= time_in_msec_to_pcm( m_current_sound_quality, m_playing_offset );
	return ( pcm_cursor / ( sound_buffer_size / m_current_sound_quality->get_bytes_per_sample( ))) * ( sound_buffer_size / m_current_sound_quality->get_bytes_per_sample( ) );
}

void sound_voice::on_finish_playing	( )
{

}

sound_scene const& sound_voice::get_sound_scene	( ) const
{
	return m_proxy.get_sound_scene( );
}

void sound_voice::set_quality	( u32 quality )
{
	if ( quality == m_current_quality )
		return;

	if ( m_conv_state != conversion_not_requested )
		return;

	if ( m_voice->buffers_queued( ) < 2 )
		return;

	if ( m_emitter.dbg_get_encoded_sound( quality ).c_ptr() == 0 )
	{
		LOG_ERROR		( "qality not yet loaded!" );
		return;
	}

	m_target_sound_quality		= m_emitter.dbg_get_encoded_sound( quality );
	m_current_quality			= quality;
}

void sound_voice::submit_muted_buffers	( u32 msec )
{
	if ( msec == 0 )
		return;

	u32 const bytes_per_sample	= m_current_sound_quality->get_bytes_per_sample	( );
	u32 const samples_per_sec	= m_current_sound_quality->get_samples_per_sec( ) ;
	u32 const samples_in_buffer	= sound_buffer_size / bytes_per_sample;
	m_channels_num					= m_current_sound_quality->get_channels_num( );
	u32 buffer_length_in_msec	= (( samples_in_buffer * 1000 ) / samples_per_sec ) / m_channels_num;
	u32 mute_buffers_count		= msec / buffer_length_in_msec;

	
	u32 mute_buffer_playing_offset		= 0;
	if ( mute_buffers_count == 0 )
		mute_buffer_playing_offset		= buffer_length_in_msec - msec;
	else
		mute_buffer_playing_offset		= ( buffer_length_in_msec * mute_buffers_count ) % msec;

	if ( mute_buffer_playing_offset != 0 )
		++mute_buffers_count;

	for ( u32 i = 0; i < mute_buffers_count; ++i )
	{
		sound_buffer* mute_buff			= m_proxy.get_world_user().get_sound_world()->get_mute_sound_buffer( m_current_sound_quality );
		m_voice->submit_source_buff		( mute_buff );
		threading::interlocked_increment( m_buffers_queued );		
	}
}

void sound_voice::set_low_pass_filter_params( float coeff )
{
	m_voice->set_low_pass_filter_params	( coeff );
}

void sound_voice::set_path	( u32 first_portal_id, u32 second_portal_id )
{
	m_first_portal_id				= first_portal_id;
	m_second_portal_id				= second_portal_id;
}

void sound_voice::get_path	( u32& first_portal_id, u32& second_portal_id ) const
{
	first_portal_id					= m_first_portal_id;
	second_portal_id				= m_second_portal_id;
}

bool sound_voice::is_same_voice	( u32 first_portal_id, u32 second_portal_id ) const
{
	if ( first_portal_id == m_first_portal_id && second_portal_id == m_second_portal_id )
		return true;

	if ( first_portal_id == m_second_portal_id || second_portal_id == m_first_portal_id )
		return true;

	return false;
}

IXAudio2SourceVoice* sound_voice::get_xaudio_voice	( )
{
	return m_voice->get_xaudio_voice	( );
}

} // namespace sound
} // namespace vostok
