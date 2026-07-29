////////////////////////////////////////////////////////////////////////////
//	Created		: 24.02.2011
//	Author		: Dmitriy Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "voice_factory.h"
#include "sound_world.h"

namespace vostok {
namespace sound {

voice_factory::voice_factory( u8* buffer, u32 buffer_size, sound_world const& world, pool_parametrs const& params ) :
	m_pool_params				( params ),
	m_min_frequency_ratio		( XAUDIO2_MIN_FREQ_RATIO ),
	m_max_frequency_ratio		( XAUDIO2_DEFAULT_FREQ_RATIO ),
	m_voices_allocator			( buffer, buffer_size )
{

	voice_bridge::creation_parametrs	voice_params;
	voice_params.xaudio_engine			= world.xaudio_engine( );
	voice_params.master_channels_num	= world.master_channels_num( );
	voice_params.max_frequency_ratio	= m_max_frequency_ratio;
	voice_params.channels_num			= 1;

	for (u32 i = 0; i < params.mono_voices_count; ++i)
	{
		voice_bridge* new_voice	= VOSTOK_NEW_IMPL( m_voices_allocator, voice_bridge)( voice_params );
		m_voices_pool[voice_params.channels_num - 1].push_back( new_voice );
	}

	voice_params.channels_num			= 2;

	for (u32 i = 0; i < params.stereo_voices_count; ++i)
	{
		voice_bridge* new_voice	= VOSTOK_NEW_IMPL( m_voices_allocator, voice_bridge)( voice_params );
		m_voices_pool[voice_params.channels_num - 1].push_back( new_voice );
	}
}

voice_factory::~voice_factory			( )
{
	for (u32 i = 0; i < 2; ++i)
	{
		voice_bridge* current_voice	= m_voices_pool[i].front( );
		while( current_voice )
		{
			voice_bridge* object_to_be_deleted		= current_voice;
			
			current_voice							= voice_list_type::get_next_of_object( current_voice );
			VOSTOK_DELETE_IMPL						( m_voices_allocator, object_to_be_deleted );
		}
	}

}

voice_bridge* voice_factory::new_voice ( sound_voice* callback_handler, u8 const channels_num, u32 const sample_rate )
{
	R_ASSERT					( channels_num );
	voice_bridge* idle_voice	= m_voices_pool[channels_num - 1].front( );
	while ( idle_voice )
	{
		if (!idle_voice->has_handler( ))
		{
			idle_voice->set_handler		( callback_handler );
			idle_voice->set_sample_rate	( sample_rate );
			return idle_voice;
		}

		idle_voice			= voice_list_type::get_next_of_object( idle_voice );
	}

	UNREACHABLE_CODE		( return 0 );
}

void voice_factory::delete_voice( voice_bridge* voice_to_be_deleted)
{
	u8 const channels_num					= voice_to_be_deleted->get_channels_num( );
	R_ASSERT_U								( m_voices_pool[channels_num - 1].contains_object( voice_to_be_deleted ));
	voice_to_be_deleted->set_handler		( 0 );
	voice_to_be_deleted->set_output_voice	( 0 );
}

void voice_factory::set_frequency_ratio				( float ratio )
{
	R_ASSERT										( !( ratio < m_min_frequency_ratio ) );
	R_ASSERT										( !( ratio > m_max_frequency_ratio ) );

	for ( u32 i = 0; i < 2; ++i )
	{
		voice_bridge* current_voice	= m_voices_pool[i].front( );
		while( current_voice )
		{
			current_voice->set_frequency_ratio		( ratio );
			current_voice							= voice_list_type::get_next_of_object( current_voice );
		}
	}
}

} // namespace sound
} // namespace vostok
