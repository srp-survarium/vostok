////////////////////////////////////////////////////////////////////////////
//	Created		: 28.02.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "voice_bridge.h"
#include "sound_buffer.h"
#include "sound_voice.h"

namespace vostok {
namespace sound {

voice_bridge::voice_bridge			( creation_parametrs& params ) : 
	m_next				( 0 ),
	m_handler			( 0 ),
	m_source_voice		( 0 ),
	m_master_channels_num( params.master_channels_num )
{
	R_ASSERT_U						( ( params.channels_num == 1 ) || ( params.channels_num == 2 ) );
	R_ASSERT_U						( params.xaudio_engine );

	WAVEFORMATEX wfx_standard		= { 0 };
	wfx_standard.wFormatTag			= WAVE_FORMAT_PCM;
    wfx_standard.nSamplesPerSec		= 44100;
    wfx_standard.wBitsPerSample		= 16;
    wfx_standard.cbSize				= 0;
	wfx_standard.nChannels			= params.channels_num;
	wfx_standard.nBlockAlign		= wfx_standard.nChannels * ( wfx_standard.wBitsPerSample >> 3 ); 
	wfx_standard.nAvgBytesPerSec	= wfx_standard.nSamplesPerSec * wfx_standard.nBlockAlign;
	m_sample_rate					= 44100;
	m_channels_num					= params.channels_num;
	
	HRESULT hr						= params.xaudio_engine->CreateSourceVoice	( 
																&m_source_voice,
																&wfx_standard,
																XAUDIO2_VOICE_NOPITCH | XAUDIO2_VOICE_NOSRC,
																params.max_frequency_ratio,
																this,
																NULL,
																NULL );
	R_ASSERT_U				(!FAILED(hr));

}

voice_bridge::~voice_bridge( )
{
	m_source_voice->DestroyVoice( );
}

void voice_bridge::set_handler ( sound_voice* handler )
{
	m_handler = handler;
	if ( !m_handler )
		stop( );
}

void voice_bridge::set_sample_rate	( u32 new_sample_rate )
{
	R_ASSERT_U			( buffers_queued() == 0 );
	if (get_sample_rate() != new_sample_rate)
	{
		m_sample_rate	= new_sample_rate;
		HRESULT hr		= m_source_voice->SetSourceSampleRate( new_sample_rate );
		R_ASSERT_U		(!FAILED(hr));
	}
}

void voice_bridge::set_output_voice		( IXAudio2SubmixVoice* output_voice  )
{
	if ( output_voice )
	{
		XAUDIO2_SEND_DESCRIPTOR desc;
		desc.Flags			= 0;
		desc.pOutputVoice	= output_voice;

		XAUDIO2_VOICE_SENDS sends;
		sends.SendCount		= 1;
		sends.pSends		= &desc;

		m_source_voice->SetOutputVoices	( &sends );
	}
	else
		m_source_voice->SetOutputVoices	( 0 );
}

void voice_bridge::start ( )
{
	HRESULT const result	= m_source_voice->Start	(0);
	R_ASSERT_U				( !FAILED(result) );
}

void voice_bridge::stop ( )
{
	HRESULT const result	= m_source_voice->Stop	();
	R_ASSERT_U				( !FAILED(result) );
}





















void voice_bridge::submit_source_buff	( sound_buffer* buffer )
{
	XAUDIO2_BUFFER* xaudio_buffer	= buffer->get_xaudio_buffer( );

	R_ASSERT						( xaudio_buffer );
	HRESULT hr						= m_source_voice->SubmitSourceBuffer( xaudio_buffer );
	R_ASSERT_U						( !FAILED(hr) );
}

void voice_bridge::flush_source_buffers ( )
{
	HRESULT hr		= m_source_voice->FlushSourceBuffers( );
	R_ASSERT_U		(!FAILED(hr));

}
u32 voice_bridge::buffers_queued ( ) const
{
	XAUDIO2_VOICE_STATE			vstate;
	m_source_voice->GetState	( &vstate );
	return						vstate.BuffersQueued;
}


void voice_bridge::set_output_matrix ( float const* level_matrix )
{
	XAUDIO2_VOICE_DETAILS voice_details;
	m_source_voice->GetVoiceDetails( &voice_details );
	m_source_voice->SetOutputMatrix( NULL, voice_details.InputChannels, m_master_channels_num, level_matrix);
}

void voice_bridge::set_channel_volumes	( u8 channels_num, float const* level_matrix )
{
	m_source_voice->SetChannelVolumes( channels_num, level_matrix );
}

void voice_bridge::set_low_pass_filter_params		( float coeff )
{
	XAUDIO2_FILTER_PARAMETERS params;
	params.Type				= LowPassFilter;
	params.Frequency		= coeff;
	params.OneOverQ			= 1.0f;

}



void voice_bridge::set_frequency_ratio				( float ratio )
{
	R_ASSERT_U				( !( ratio < 0.0f ) );
	R_ASSERT_U				( m_source_voice );
	HRESULT hr				= m_source_voice->SetFrequencyRatio( ratio );
	R_ASSERT_U				(!FAILED(hr));
}





void voice_bridge::OnVoiceProcessingPassStart	( UINT32 BytesRequired )
{
}

void voice_bridge::OnVoiceProcessingPassEnd	( )
{
}

void voice_bridge::OnStreamEnd	( )
{
	R_ASSERT_U					(m_handler);
}


void voice_bridge::OnBufferStart( void* pBufferContext )
{
	R_ASSERT_U					(m_handler);
	m_handler->on_buffer_start	( pBufferContext );
}

void voice_bridge::OnBufferEnd	( void* pBufferContext )
{
	R_ASSERT_U					(m_handler);
	m_handler->on_buffer_end	( pBufferContext );
}

void voice_bridge::OnLoopEnd	( void* pBufferContext )
{
	R_ASSERT_U					(m_handler);
	m_handler->on_loop_end		( pBufferContext );
}

void voice_bridge::OnVoiceError	( void* pBufferContext, HRESULT Error )
{
	R_ASSERT_U					(m_handler);
	m_handler->on_voice_error	( pBufferContext, Error );
}

} // namespace sound
} // namespace vostok
