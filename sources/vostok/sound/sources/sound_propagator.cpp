////////////////////////////////////////////////////////////////////////////
//	Created		: 15.06.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_propagator.h"
#include "sound_voice_params.h"
#include <vostok/sound/sound_propagator_emitter.h>
#include <vostok/sound/sound_rms.h>
#include <vostok/sound/sound_spl.h>
#include <vostok/sound/world_user.h>
#include "sound_world.h"


namespace vostok {
namespace sound {

static inline sound_world* get_world	( sound_instance_proxy_internal const& proxy )
{
	return proxy.get_world_user().get_sound_world();
}

static u32 prop_id					= 0;

new_sound_propagator::new_sound_propagator
(
	float3 const&						start_position,
	float3 const&						listener_position,
	playback_mode						mode,
	u32									playback_id,
	u32									playing_offset,
	u32									before_playing_offset,
	u32									after_playing_offset,
	sound_instance_proxy_internal&		proxy,
	sound_propagator_emitter const&		emitter
) :
	m_voice						( 0 ),
	m_start_position			( start_position ),
	m_proxy						( proxy ),
	m_emitter					( emitter ),
	m_mode						( mode ),
	m_is_callback_executer		( false ),
	m_playing_offset			( playing_offset ),
	m_playback_id				( playback_id ),
	m_propagation_time			( 0 ),
	m_before_playing_offsets	( before_playing_offset ),
	m_after_playing_offsets		( after_playing_offset ),
	m_propagation_state			( propagating ),
	m_perceived_loudness		( 0.0f ),
	m_attenuated_loudness		( 0.0f )
{
	m_sound_length				= (u32)emitter.get_quality_for_resource( )->get_length_in_msec( );
	m_sound_length_with_offsets	= m_before_playing_offsets + m_sound_length + m_after_playing_offsets;

	if ( m_proxy.get_sound_type( ) != hud )
		m_time_to_listener		= (u32)( ( listener_position - start_position ).length( ) / get_world( proxy )->get_speed_of_sound( ) );
	else
		m_time_to_listener		= 0;

	if ( mode == once )
		m_end_propagation_time	= m_time_to_listener + m_sound_length_with_offsets - playing_offset;
	else
		m_end_propagation_time	= u32(-1);
}

new_sound_propagator::~new_sound_propagator	( )
{
	if ( m_voice )
		detach_voice				( m_voice );
}

void new_sound_propagator::tick	( u32 time_delta_in_msec )
{
	if ( m_propagation_state == propagating )
	{
		m_propagation_time			+= time_delta_in_msec;
		if ( m_propagation_time > m_end_propagation_time )
		{
			stop_propagation			( );
			if ( m_is_callback_executer )
				execute_callback		( );
		}
	}
}

void new_sound_propagator::pause_propagation	( )
{
	if ( m_propagation_state == propagating )
	{
		m_propagation_state = propagating_paused;
		detach_voices			( 4 );
	}
}

void new_sound_propagator::resume_propagation	( )
{
	if ( m_propagation_state == propagating_paused )
		m_propagation_state = propagating;
}

void new_sound_propagator::stop_propagation	( )
{
	m_propagation_state		= propagating_finished;
	detach_voices			( 4 );
}

u32 new_sound_propagator::stop_produce	( )
{
	u32 producing_time			= m_propagation_time + m_playing_offset;
	m_end_propagation_time		= m_propagation_time + m_time_to_listener;
	return						producing_time;
}

void new_sound_propagator::distribute_voices	( u32 count, vectora< sound_voice_params > const& voices_params )
{
	R_ASSERT					( count == voices_params.size( ) );
	attach_voices				( count, voices_params );
}

void new_sound_propagator::attach_voices	( u32 count, vectora< sound_voice_params > const& voices_params )
{
	VOSTOK_UNREFERENCED_PARAMETERS	( count, voices_params );
	if ( !m_voice )
	{
		u32 offset				= sound_playing_time( );
		if ( offset == u32(-1) )
			return;

		m_voice					= attach_voice( offset );
		m_voice->set_output_matrix	( voices_params[0].channel_matrix );
		m_voice->play			( m_mode );
		return;
	}

	m_voice->set_output_matrix	( voices_params[0].channel_matrix );
}

void new_sound_propagator::detach_voices	( u32 )
{
}

sound_voice* new_sound_propagator::attach_voice	( u32 offset )
{
	u32 const tick_percission	= 10;
	if ( offset <= tick_percission )
		offset					= 0;

	return get_world( m_proxy )->create_sound_voice
	(
		m_proxy.get_sound_scene( ),
		offset,
		0,
		0,
		&m_proxy,
		m_emitter,
		m_emitter.get_sound_spl( )
	);
}

void new_sound_propagator::detach_voice	( sound_voice* voice )
{
	R_ASSERT					( voice );
	voice->stop					( );
	get_world( m_proxy )->delete_sound_voice
	(
		m_proxy.get_sound_scene( ),
		voice
	);
	voice						= 0;
}

void new_sound_propagator::set_voice_channel_matrix	( sound_voice* voice, float const* channel_matrix, float lp_coeff )
{
	voice->set_output_matrix			( channel_matrix );
	voice->set_low_pass_filter_params	( lp_coeff );
}

u32 new_sound_propagator::sound_playing_time	( ) const
{
	u32 offset					= sound_playing_time_with_offsets( );
	if ( offset == u32(-1) )
		return					u32(-1);

	R_ASSERT					( offset >= m_before_playing_offsets && offset <= m_before_playing_offsets + m_sound_length );
	if ( offset < m_before_playing_offsets || offset > m_before_playing_offsets + m_sound_length )
		return					u32(-1);

	return						offset - m_before_playing_offsets;
}

u32 new_sound_propagator::sound_playing_time_with_offsets	( ) const
{
	if ( m_propagation_time < m_time_to_listener )
		return					u32(-1);

	u32 offset					= m_propagation_time - m_time_to_listener;
	if ( m_mode == once )
		return					offset + m_playing_offset;
	else
		return					( offset + m_playing_offset ) % m_sound_length_with_offsets;
}

void new_sound_propagator::execute_callback	( )
{
	m_proxy.set_callback_pending		( true );
	functor_response* response			= VOSTOK_NEW_IMPL
	(
		m_proxy.get_world_user( ).get_channel( ).responses.owner_allocator( ),
		functor_response
	)
	(
		boost::bind
		(
			static_cast<void (sound_instance_proxy_internal::*)( u32 )>
			( &sound_instance_proxy_internal::execute_callback ),
			boost::ref( m_proxy ),
			m_playback_id
		)
	);
	m_proxy.get_world_user( ).add_response	( response );
}

#ifndef MASTER_GOLD
void sound_propagator::dump_debug_snapshot	( configs::lua_config_value& val ) const
{
	encoded_sound_child_ptr const& res	= get_propagator_emitter( ).get_quality_for_resource( );
	val["filename"]						= res->reusable_request_name( ).c_str( );
	val["length"]						= get_sound_length_original( );
	val["current_playing_time"]			= ( get_playing_offset( ) % get_sound_length_original( ) );
}

void sound_propagator::fill_statistic		( propagator_statistic& statistic ) const
{
	encoded_sound_child_ptr const& res	= get_propagator_emitter( ).get_quality_for_resource( );
	statistic.m_filename				= res->reusable_request_name( ).c_str( );
	statistic.m_length					= get_sound_length_original( );
	statistic.m_current_playing_time	= ( get_playing_offset( ) % get_sound_length_original( ) );
	statistic.m_rms						= get_sound_rms( );
	statistic.m_playback_mode			= m_mode;
}
#endif // #ifndef MASTER_GOLD

} // namespace sound
} // namespace vostok
