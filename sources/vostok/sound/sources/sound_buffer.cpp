////////////////////////////////////////////////////////////////////////////
//	Created		: 21.02.2011
//	Author		: Dmitriy Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_buffer.h"

namespace vostok {
namespace sound {



lightweight_sound_buffer::lightweight_sound_buffer( encoded_sound_ptr const& encoded_sound, u32 offset ) :
	m_encoded_sound				( encoded_sound ),
	m_cached_offset				( offset )
{
	// empty
}



sound_buffer::sound_buffer ( ) : 
	m_users								( 0 ),
	m_cached_offset						( 0 ),
	m_cached_offset_after_decompress	( 0 ),
	m_reference_count					( 0 ),
	m_last_value_in_buffer				( 0 )
{
}

sound_buffer::~sound_buffer	( )
{
}

void sound_buffer::fill_buffer( encoded_sound_ptr const& encoded_sound, u32 pcm_offset, u32& next_pcm_offset )
{
	m_encoded_sound						= encoded_sound;
	m_cached_offset						= pcm_offset;
	u64 total_pcm						= m_encoded_sound->get_length_in_pcm( );
	u32 audio_bytes						= m_encoded_sound->decompress(m_buff, pcm_offset, next_pcm_offset, sound_buffer_size );
	m_last_value_in_buffer				= *(((s16*)m_buff) + ( next_pcm_offset - pcm_offset ) - 1);
	m_xaudio_buffer.PlayLength			= audio_bytes / ( m_encoded_sound->get_bytes_per_sample( ) * m_encoded_sound->get_channels_num( ) );
	m_xaudio_buffer.PlayBegin			= 0;
	m_cached_offset_after_decompress	= next_pcm_offset;
	m_xaudio_buffer.Flags				= total_pcm == next_pcm_offset ? XAUDIO2_END_OF_STREAM : 0;
    m_xaudio_buffer.AudioBytes			= audio_bytes;
    m_xaudio_buffer.pAudioData			= m_buff;
    m_xaudio_buffer.LoopBegin			= 0;
    m_xaudio_buffer.LoopLength			= 0;
    m_xaudio_buffer.LoopCount			= 0;
    m_xaudio_buffer.pContext			= this;
}

void sound_buffer::fill_mute_buffer		( encoded_sound_ptr const& encoded_sound )
{
	memory::zero( (pvoid)m_buff, sound_buffer_size * sizeof(u8) );
	memory::zero( (pvoid)&m_xaudio_buffer, sizeof(XAUDIO2_BUFFER) );
	m_xaudio_buffer.AudioBytes		= sound_buffer_size;
	m_xaudio_buffer.pAudioData		= m_buff;
	m_xaudio_buffer.pContext		= this;
	m_xaudio_buffer.PlayLength		= sound_buffer_size / ( encoded_sound->get_bytes_per_sample( ) * m_encoded_sound->get_channels_num( ) );
}

} // namespace sound
} // namespace vostok
