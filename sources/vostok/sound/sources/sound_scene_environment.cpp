// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "sound_scene.h"
#include "effect_cross_fader.h"
#include "sound_environment.h"
#include "sound_world.h"
#include <vostok/collision/space_partitioning_tree.h>

namespace vostok {
namespace sound {

IXAudio2SubmixVoice* sound_scene::create_environment_submix_voice	( sound_world const& world ) const
{
	IXAudio2SubmixVoice* voice			= world.create_submix_voice( 1, 1 );

	IUnknown* pReverbEffect				= 0;
	HRESULT hr							= XAudio2CreateReverb( &pReverbEffect );
	XAUDIO2_EFFECT_DESCRIPTOR effects[1] = { { pReverbEffect, true, 1 } };
	XAUDIO2_EFFECT_CHAIN effectChain		= { 1, effects };

	voice->SetEffectChain				( &effectChain );

	XAUDIO2FX_REVERB_I3DL2_PARAMETERS i3dl2_params = XAUDIO2FX_I3DL2_PRESET_DEFAULT;
	XAUDIO2FX_REVERB_PARAMETERS native;
	ReverbConvertI3DL2ToNative			( &i3dl2_params, &native );
	hr									= voice->SetEffectParameters( 0, &native, sizeof( native ) );

	return								voice;
}

void sound_scene::insert_environment	( sound_environment& environment, float4x4 const& transform )
{
	m_environments_tree->insert			( environment.collision( ), transform );
}

void sound_scene::add_environment_params	(
	pcstr name,
	XAUDIO2FX_REVERB_I3DL2_PARAMETERS* params,
	u32& id
)
{
	id									= m_environment_parameters.size( );
	m_environment_parameters.push_back	( std::make_pair( fixed_string< 64 >( name ), params ) );
}

u32 sound_scene::get_environment_params_id	( pcstr name )
{
	for ( u32 i = 0; i < m_environment_parameters.size( ); ++i )
	{
		if ( strings::equal( m_environment_parameters[i].first.c_str( ), name ) )
			return i;
	}

	return u32( -1 );
}

XAUDIO2FX_REVERB_I3DL2_PARAMETERS* sound_scene::get_environment_params	( pcstr name )
{
	XAUDIO2FX_REVERB_I3DL2_PARAMETERS* params = 0;
	for ( u32 i = 0; i < m_environment_parameters.size( ); ++i )
	{
		if ( strings::equal( m_environment_parameters[i].first.c_str( ), name ) )
		{
			params						= m_environment_parameters[i].second;
			break;
		}
	}

	return params;
}

XAUDIO2FX_REVERB_I3DL2_PARAMETERS* sound_scene::get_environment_params	( u32 id )
{
	R_ASSERT							( id < m_environment_parameters.size( ) );
	return m_environment_parameters[id].second;
}

sound_environment* sound_scene::get_current_environment	( )
{
	collision::objects_type query_result	( g_allocator );
	math::aabb aabb						=
		math::create_aabb_center_radius	( m_list_position.get( ), float3( 1.0f, 1.0f, 1.0f ) );
	m_environments_tree->aabb_query		( 1, aabb, query_result );

	if ( query_result.empty( ) )
		return m_default_environment;

	sound_environment* test				=
		static_cast< sound_environment* >( query_result[0]->user_data( ) );
	return test;
}

IXAudio2SubmixVoice* sound_scene::get_current_effect_submix	( )
{
	return m_environment_crossfader->submix( );
}

} // namespace sound
} // namespace vostok
