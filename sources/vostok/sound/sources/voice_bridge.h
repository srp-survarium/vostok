////////////////////////////////////////////////////////////////////////////
//	Created		: 28.02.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOICE_BRIDGE_H_INCLUDED
#define VOICE_BRIDGE_H_INCLUDED

#include "voice_format.h"

struct IXAudio2Voice;
struct IXAudio2SubmixVoice;

namespace vostok {
namespace sound {

class sound_buffer;
class sound_voice;

class voice_bridge :
	private IXAudio2VoiceCallback,
	private boost::noncopyable
{
public:
	inline	u8				get_channels_num				( ) const { return m_channels_num; }
	inline	u32				get_sample_rate					( ) const { return m_sample_rate; }

			void			start							( );
			void			stop							( );
			void			submit_source_buff				( sound_buffer* buffer );
			void			flush_source_buffers			( );

			u32				buffers_queued					( ) const;

			void			set_output_matrix				( float const* level_matrix );
			void			set_channel_volumes				( u8 channels_num, float const* level_matrix );
			void			set_low_pass_filter_params		( float coeff );
			void			set_output_voice				( IXAudio2SubmixVoice* output_voice  );
			void			set_sample_rate					( u32 new_sample_rate );
			void			set_frequency_ratio				( float ratio );
	inline	IXAudio2SourceVoice*	get_xaudio_voice			( ) { return m_source_voice; }
private:
	struct creation_parametrs
	{
		IXAudio2*			xaudio_engine;
		float				max_frequency_ratio;
		u8					channels_num;
		u8					master_channels_num;
		
	}; // struct creation_parametrs

							voice_bridge					( creation_parametrs& params );

			bool			has_handler						( ) const { return m_handler != 0; }
			void			set_handler						( sound_voice* handler );
			

	virtual void __stdcall	OnVoiceProcessingPassStart		( UINT32 BytesRequired );
    virtual void __stdcall	OnVoiceProcessingPassEnd		( );
    virtual void __stdcall	OnStreamEnd						( );
    virtual void __stdcall	OnBufferStart					( void* pBufferContext );
    virtual void __stdcall	OnBufferEnd						( void* pBufferContext );
    virtual void __stdcall	OnLoopEnd						( void* pBufferContext );
    virtual void __stdcall	OnVoiceError					( void* pBufferContext, HRESULT Error );

public:
	voice_bridge*			m_next;
	virtual					~voice_bridge					( );
private:
	sound_voice*			m_handler;
	IXAudio2SourceVoice*	m_source_voice;
	u8						m_master_channels_num;
	u32						m_sample_rate;
	u8						m_channels_num;

	friend class			voice_factory;
}; // class voice_bridge

typedef intrusive_list<voice_bridge, voice_bridge*, &voice_bridge::m_next, threading::single_threading_policy> voice_list_type;

} // namespace sound
} // namespace vostok

#endif // #ifndef VOICE_BRIDGE_H_INCLUDED
