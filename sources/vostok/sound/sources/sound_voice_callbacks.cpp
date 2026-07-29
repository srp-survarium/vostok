////////////////////////////////////////////////////////////////////////////
//	Created		: 20.06.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_voice.h"
#include <vostok/sound/world_user.h>
#include <vostok/sound/sound_propagator_emitter.h>
#include <vostok/sound/sound_rms.h>
#include "functor_command.h"
#include "sound_world.h"
#include "voice_bridge.h"
#include "sound_buffer.h"

namespace vostok {
namespace sound {

// Called just before this voice's processing pass begins.
void sound_voice::on_voice_processing_pass_start	( u32 bytes_required )
{
	VOSTOK_UNREFERENCED_PARAMETERS( bytes_required );

}

// Called just after this voice's processing pass ends.
void sound_voice::on_voice_processing_pass_end	( )
{

}

// Called when this voice has just finished playing a buffer stream
void sound_voice::on_stream_end			( )
{






}

// Called when this voice is about to start processing a new buffer.
void sound_voice::on_buffer_start		( pvoid buffer_context )
{
	if ( m_conv_state == conversion_requested )
	{
		LOG_DEBUG							( "on_buffer_start conv start" );
		threading::interlocked_exchange		( m_conv_state, conversion_buffer_preparing );

		m_voice->flush_source_buffers							( );

		m_proxy.get_world_user().get_sound_world()->add_xaudio_order(
				MT_NEW ( functor_order ) (
				boost::bind( &sound_voice::on_buffer_start_impl, this, buffer_context )
			));
	}
	else if ( m_conv_state == conversion_buffer_submited )
		threading::interlocked_exchange	( m_conv_state, conversion_not_requested );
}

// Called when this voice has just finished processing a buffer.
// The buffer can now be reused or destroyed.
void sound_voice::on_buffer_end			( pvoid buffer_context )
{
	if ( m_conv_state == conversion_buffer_ready && m_voice->buffers_queued( ) == 0 )
	{
		m_voice->set_sample_rate		( m_target_sound_quality->get_samples_per_sec( ));
		
		m_voice->submit_source_buff		( m_conversion_buffer );
		threading::interlocked_increment( m_buffers_queued );
		threading::interlocked_exchange	( m_conv_state, conversion_buffer_submited );

		m_current_sound_quality			= m_target_sound_quality;
		m_target_sound_quality			= 0;
	}

	m_world_user.get_sound_world()->add_xaudio_order(
				MT_NEW ( functor_order ) (
				boost::bind( &sound_voice::on_buffer_end_impl, this, buffer_context )
			)
			);
}
void sound_voice::on_loop_end			( pvoid buffer_context )
{
	VOSTOK_UNREFERENCED_PARAMETERS( buffer_context );
}

void sound_voice::on_voice_error		( pvoid buffer_context, HRESULT error )
{
	LOG_ERROR("OnVoiceError: %d, %x", error, buffer_context);
}

void sound_voice::on_stream_end_impl( )
{
}


void	sound_voice::on_buffer_start_impl( pvoid pcontext )
{
}

void sound_voice::on_buffer_end_impl( void* pcontext )
{
	threading::interlocked_decrement( m_buffers_queued );

	if ( pcontext == 0 )
		return;

	if ( m_is_playing && ( m_conv_state == conversion_not_requested || m_conv_state == conversion_buffer_submited ) )
		refill_buffers		( );



	sound_buffer* sb		= static_cast_checked<sound_buffer*>(pcontext);
	m_world_user.get_sound_world()->free_sound_buffer ( sb );

	return;
}

void sound_voice::on_buffer_error_impl( void* pcontext )
{
	VOSTOK_UNREFERENCED_PARAMETERS( pcontext );
}

} // namespace sound
} // namespace vostok
