////////////////////////////////////////////////////////////////////////////
//	Created		: 02.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_scene.h"
#include <vostok/collision/api.h>
#include <vostok/collision/ray_collision_utils.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/sound/sound_propagator_emitter.h>
#include <vostok/sound/sound_receiver.h>
#include <vostok/sound/sound_spl.h>
#include <vostok/sound/world_user.h>
#include <vostok/sound/single_sound.h>
#include <vostok/sound/sound_debug_stats.h>
#include <vostok/sound/sound_scene_creation_params.h>
#include <vostok/sound/search/search_service.h>
#include <vostok/core_entry_point.h>
#include "effect_cross_fader.h"
#include "sound_environment.h"
#include "sound_instance_proxy_order.h"
#include "sound_world.h"
#include "speakers.h"
#include "propagator_info.h"
#include "unique_propagator_info.h"
#include "compare_by_propagator.h"

namespace vostok {
namespace sound {

enum
{
	// this constant for playing sound earlier then needed (play quiet),
	// avoiding tick percission
	precalculation_time_for_propagators_in_msec			= 10,
};

receiver_collision::receiver_collision	( sound_receiver* receiver, atomic_half3* pos ) :
	m_receiver	( receiver ),
	m_next		( 0 ),
	m_position	( pos )
{
	float r					= 0.3f;
	m_collision				= &*collision::new_aabb_object
	( 
		g_allocator,
		1,
		m_position->get( ),
		float3( r, r, r ),
		this
	);
}

receiver_collision::~receiver_collision		( )
{
	collision::delete_object			( g_allocator, m_collision );
	m_collision							= 0;
}

void receiver_collision::delete_position	( sound_scene& scene )
{
	scene.delete_receiver_position		( m_position );
	m_position							= 0;
}

effect_cross_fader::effect_cross_fader	(
	sound_scene& scene,
	u32 fade_time_in_ms,
	IXAudio2SubmixVoice* first_submix,
	IXAudio2SubmixVoice* second_submix
) :
	m_scene								( scene ),
	m_fade_time							( fade_time_in_ms ),
	m_fade_in_value						( 1.0f ),
	m_fade_in_submix					( first_submix ),
	m_fade_out_submix					( second_submix )
{
	m_fade_in_environment				= m_scene.get_current_environment( );
	m_fade_out_environment				= m_scene.get_current_environment( );
}

void effect_cross_fader::tick	( u32 delta_time_in_ms, sound_environment* current_environment )
{
	if ( m_fade_in_environment != current_environment )
	{
		if ( math::is_similar( m_fade_in_value, 1.0f ) )
		{
			XAUDIO2FX_REVERB_I3DL2_PARAMETERS* params =
				m_scene.get_environment_params( m_fade_out_environment->env_params_id( ) );
			XAUDIO2FX_REVERB_PARAMETERS native;
			ReverbConvertI3DL2ToNative	( params, &native );
			m_fade_out_submix->SetEffectParameters( 0, &native, sizeof( native ) );

			m_fade_out_environment		= m_fade_in_environment;
			m_fade_in_environment		= current_environment;
			m_fade_in_value				= 0.0f;

			IXAudio2SubmixVoice* temp	= m_fade_in_submix;
			m_fade_in_submix			= m_fade_out_submix;
			m_fade_out_submix			= temp;

			m_fade_in_submix->SetVolume	( m_fade_in_value );
			m_fade_out_submix->SetVolume( 1.0f - m_fade_in_value );
		}
		else
		{
			m_fade_out_environment		= m_fade_in_environment;
			m_fade_in_environment		= current_environment;
			m_fade_in_value				= 1.0f - m_fade_in_value;

			IXAudio2SubmixVoice* temp	= m_fade_in_submix;
			m_fade_in_submix			= m_fade_out_submix;
			m_fade_out_submix			= temp;

			m_fade_in_submix->SetVolume	( m_fade_in_value );
			m_fade_out_submix->SetVolume( 1.0f - m_fade_in_value );
		}
	}
	else
	{
		float fade_out_value			= 0.0f;
		if ( m_fade_in_value < 1.0f )
		{
			m_fade_in_value				+= delta_time_in_ms * 1.0f / ( m_fade_time * 1.0f );
			m_fade_in_value				= math::min( m_fade_in_value, 1.0f );
			fade_out_value				= 1.0f - m_fade_in_value;
		}

		m_fade_in_submix->SetVolume		( m_fade_in_value );
		m_fade_out_submix->SetVolume	( 1.0f - m_fade_in_value );
	}
}

sound_scene::sound_scene	(
	sound_world& world,
	sound_scene_creation_params const& creation_params,
	IXAudio2SubmixVoice* submix_voice,
	u32 dbg_id,
	resources::query_result_for_cook& parent
) :
	m_next						( 0 ),
	m_environment_parameters	( g_allocator ),
	m_world						( world ),
	m_memory_arena_resources_ptr( 0 ),
	m_graph						( 0 ),
	m_proxies_count				( creation_params.proxies_count ),
	m_propagators_count			( creation_params.propagators_count ),
	m_receivers_count			( creation_params.receivers_count ),
	m_spatial_tree				( 0 ),
	m_environments_tree			( 0 ),
	m_submix_voice				( submix_voice ),
	m_fade_in_environment		( 0 ),
	m_fade_out_environment		( 0 ),
	m_is_active					( false ),
	m_is_listener_position_set	( false ),
	m_fade_state				( none ),
	m_fade_in_time				( 0 ),
	m_fade_out_time				( 0 ),
	m_volume					( 0.0f ),
	m_fade_vol_per_msec			( 0.0f ),
	m_is_paused					( false ),
	m_dbg_id					( dbg_id )
{
#ifndef MASTER_GOLD
	m_debug_snapshot					= 0;
	m_is_debug_stream_writing_enabled	= 0;
#endif // #ifndef MASTER_GOLD

	init_allocators						( parent );

	ASSERT								( !m_spatial_tree );
	m_spatial_tree						= &*collision::new_space_partitioning_tree( g_allocator, 0.0001f, 1024 );
	ASSERT								( m_spatial_tree );

	ASSERT								( !m_environments_tree );
	m_environments_tree					= &*collision::new_space_partitioning_tree( g_allocator, 0.0001f, 64 );
	ASSERT								( m_environments_tree );

	if ( m_world.is_audio_device_exist( ) )
	{
		m_fade_in_environment			= m_world.create_submix_voice( 1, 1 );
		m_fade_out_environment			= m_world.create_submix_voice( 1, 1 );

		IUnknown* pReverbEffect_1		= 0;
		HRESULT hr						= XAudio2CreateReverb( &pReverbEffect_1 );
		XAUDIO2_EFFECT_DESCRIPTOR effects_1[1] =
		{
			{ pReverbEffect_1, true, 1 }
		};
		XAUDIO2_EFFECT_CHAIN effectChain_1 =
		{
			1,
			effects_1
		};
		hr								= m_fade_in_environment->SetEffectChain( &effectChain_1 );

		IUnknown* pReverbEffect_2		= 0;
		hr								= XAudio2CreateReverb( &pReverbEffect_2 );
		XAUDIO2_EFFECT_DESCRIPTOR effects_2[1] =
		{
			{ pReverbEffect_2, true, 1 }
		};
		XAUDIO2_EFFECT_CHAIN effectChain_2 =
		{
			1,
			effects_2
		};
		hr								= m_fade_out_environment->SetEffectChain( &effectChain_2 );

		XAUDIO2FX_REVERB_I3DL2_PARAMETERS i3dl2_params =
		{
			100.0f,
			-10000,
			0,
			0.0f,
			1.0f,
			0.5f,
			-10000,
			0.02f,
			-10000,
			0.04f,
			100.0f,
			100.0f,
			5000.0f
		};
		XAUDIO2FX_REVERB_I3DL2_PARAMETERS* default_params =
			VOSTOK_NEW_IMPL( g_allocator, XAUDIO2FX_REVERB_I3DL2_PARAMETERS )( i3dl2_params );
		m_environment_parameters.push_back( std::make_pair( fixed_string< 64 >( "" ), default_params ) );

		u32 env_params_id				= 0;
		add_environment_params			( "", default_params, env_params_id );
		m_default_environment			= VOSTOK_NEW_IMPL( g_allocator, sound_environment )( env_params_id );

		XAUDIO2FX_REVERB_PARAMETERS native;
		ReverbConvertI3DL2ToNative		( &i3dl2_params, &native );
		hr								= m_fade_in_environment->SetEffectParameters( 0, &native, sizeof( native ) );
		hr								= m_fade_out_environment->SetEffectParameters( 0, &native, sizeof( native ) );

		XAUDIO2_SEND_DESCRIPTOR send_descriptor =
		{
			0,
			m_submix_voice
		};
		XAUDIO2_VOICE_SENDS sends =
		{
			1,
			&send_descriptor
		};
		hr								= m_fade_in_environment->SetOutputVoices( &sends );
		hr								= m_fade_out_environment->SetOutputVoices( &sends );

		m_environment_crossfader		=
			VOSTOK_NEW_IMPL( g_allocator, effect_cross_fader )
			( *this, 100, m_fade_in_environment, m_fade_out_environment );
		VOSTOK_UNREFERENCED_PARAMETER	( hr );
	}
}

void sound_scene::clear_resources	( )
{
	m_world.free_submix_voice		( m_fade_in_environment );
	m_world.free_submix_voice		( m_fade_out_environment );
	m_world.free_submix_voice		( m_submix_voice );
}

struct receiver_unconditional_erasing_predicate : private boost::noncopyable
{
	inline void operator()	( receiver_collision* receiver_container ) const
	{
		DELETE				( receiver_container );
	}	
};

sound_scene::~sound_scene	( )
{
	m_graph							= 0;
	delete_space_partitioning_tree	( m_spatial_tree );

	if ( !m_receivers.empty( ) )
	{
		m_receivers.for_each	( receiver_unconditional_erasing_predicate() );
		m_receivers.clear		( );
	}

	R_ASSERT					( m_receivers.empty( ) );

}

void sound_scene::stop		( )
{
	sound_instance_proxy_internal* proxy		= m_active_proxies.front( );
	while ( proxy )
	{
		stop_propagate_sound					( *proxy );
		proxy									= m_active_proxies.get_next_of_object( proxy );
	}
}

void sound_scene::tick		( sound_world& world, u32 time_delta )
{
	bool const device_exist		= world.is_audio_device_exist( );
	if ( device_exist )
		m_environment_crossfader->tick( time_delta, get_current_environment( ) );

	R_ASSERT					( m_is_active );
	if ( m_is_paused )
		return;

	process_fade				( world, time_delta );

	sound_instance_proxy_internal* proxy		= m_active_proxies.front( );
	while ( proxy )
	{
		sound_instance_proxy_internal* next_proxy	= m_active_proxies.get_next_of_object( proxy );
		proxy->tick									( time_delta );
		proxy										= next_proxy;
	}

	update_receivers_position	( );
	notify_receivers			( );
	if ( m_is_listener_position_set && device_exist )
		notify_listener			( world );
}

sound_instance_proxy_ptr sound_scene::create_sound_instance_proxy	
										(
											sound_emitter_ptr emitter,
											sound_propagator_emitter const& propagator_emitter,
											world_user& user
										)
{
	LOG_DEBUG							( "sound_scene::create_sound_instance_proxy" );
	if ( m_proxies_allocator->total_size( ) == m_proxies_allocator->allocated_size( ) )
	{
		LOG_ERROR						( "can't allocate sound_instance_proxy, memory is full" );
		return sound_instance_proxy_ptr	( 0 );
	}

	sound_instance_proxy* new_proxy		= VOSTOK_NEW_IMPL( m_proxies_allocator.c_ptr(), sound_instance_proxy_internal )( *this, emitter, propagator_emitter, user );
	LOG_INFO							( "new sound_instance_proxy allocated, id %d", new_proxy->get_id( ) );
	return sound_instance_proxy_ptr		( new_proxy );
}

sound_instance_proxy_ptr sound_scene::create_sound_instance_proxy	
										(
											sound_emitter_ptr emitter,
											sound_propagator_emitter const& propagator_emitter,
											world_user& user,
											sound_cone_type cone_type
										)
{
	LOG_DEBUG							( "sound_scene::create_sound_instance_proxy" );
	if ( m_proxies_allocator->total_size( ) == m_proxies_allocator->allocated_size( ) )
	{
		LOG_ERROR						( "can't allocate sound_instance_proxy, memory is full" );
		return sound_instance_proxy_ptr	( 0 );
	}

	sound_instance_proxy* new_proxy		= VOSTOK_NEW_IMPL( m_proxies_allocator.c_ptr(), sound_instance_proxy_internal )( *this, emitter, propagator_emitter, user, cone_type );
	LOG_INFO							( "new sound_instance_proxy allocated, id %d", new_proxy->get_id( ) );
	return sound_instance_proxy_ptr		( new_proxy );
}

sound_instance_proxy_ptr sound_scene::create_sound_instance_proxy	
											(
												sound_emitter_ptr emitter,
												sound_propagator_emitter const& propagator_emitter,
												world_user& user,
												collision::geometry_instance& geometry,
												float radius
											)
{
	LOG_DEBUG							( "sound_scene::create_sound_instance_proxy" );
	if ( m_proxies_allocator->total_size( ) == m_proxies_allocator->allocated_size( ) )
	{
		LOG_ERROR						( "can't allocate sound_instance_proxy, memory is full" );
		return sound_instance_proxy_ptr	( 0 );
	}

	sound_instance_proxy* new_proxy		= VOSTOK_NEW_IMPL( m_proxies_allocator.c_ptr(), sound_instance_proxy_internal )( *this, emitter, propagator_emitter, user, geometry, radius );
	LOG_INFO							( "new sound_instance_proxy allocated, id %d", new_proxy->get_id( ) );
	return sound_instance_proxy_ptr		( new_proxy );
}

void sound_scene::init_allocators	( resources::query_result_for_cook& parent )
{
	LOG_DEBUG						( "sound_scene::init_allocators" );
	u32	proxies_size				= m_proxies_count * sizeof( sound_instance_proxy_internal );
	u32	propagators_size			= m_propagators_count * sizeof( new_sound_propagator );
	u32 receiver_positions_size		= m_receivers_count * sizeof ( atomic_half3 );
	u32 receiver_collisions_size	= m_receivers_count * sizeof ( receiver_collision );
	u32 allocation_size				= proxies_size + propagators_size + receiver_positions_size + receiver_collisions_size;


	resources::creation_request request	
	(
		"unmanaged_sound_resources_allocation",
		allocation_size,
		resources::unmanaged_allocation_class
	);

	resources::query_create_resources_and_wait	
	(
		&request, 
		1,
		boost::bind( &sound_scene::on_unmanaged_resources_allocated, this, _1 ), 
		g_allocator,
		0,
		&parent,
		assert_on_fail_true
	);

	u32 proxies_offset				= 0;
	VOSTOK_CONSTRUCT_REFERENCE		( m_proxies_allocator, sound_proxies_allocator )
									( m_memory_arena_resources_ptr->buffer + proxies_offset, proxies_size );

	u32 propagators_offset			= proxies_offset + proxies_size;
	VOSTOK_CONSTRUCT_REFERENCE		( m_propagators_allocator, sound_propagators_allocator )
									( m_memory_arena_resources_ptr->buffer + propagators_offset, propagators_size );

	u32 receivers_positions_offset	= propagators_offset + propagators_size;
	VOSTOK_CONSTRUCT_REFERENCE		( m_receiver_positions_allocator, receiver_position_allocator )
									( m_memory_arena_resources_ptr->buffer + receivers_positions_offset, receiver_positions_size );

	u32 receivers_collisions_offset	= receivers_positions_offset + receiver_positions_size;
	VOSTOK_CONSTRUCT_REFERENCE		( m_receiver_collisions_allocator, receiver_collision_allocator )
									( m_memory_arena_resources_ptr->buffer + receivers_collisions_offset, receiver_collisions_size );
}

void sound_scene::on_unmanaged_resources_allocated	( resources::queries_result& queries )
{
	R_ASSERT					( queries[0].is_successful() );
	m_memory_arena_resources_ptr	= static_cast_resource_ptr<resources::unmanaged_allocation_resource_ptr>( queries[0].get_unmanaged_resource() );
}

void sound_scene::notify_receivers	( )
{
	sound_instance_proxy_internal* proxy		= m_active_proxies.front( );
	while ( proxy )
	{
		proxy->notify_receivers					( *m_spatial_tree );
		proxy									= m_active_proxies.get_next_of_object( proxy );
	}
}

void sound_scene::calculate_in_graph_position( float3 const& proxy_position )
{
	vectora< fixed_vector< u32, 32 > > paths( g_allocator );
	vector< search::vertex_id_type > path;
	search::search_service s( g_allocator );
	s.search( *g_allocator, m_graph, &path, proxy_position, m_list_position.get( ), 200.0f, paths );
}

void sound_scene::find_path(
	float3 const& destination_point,
	vectora< fixed_vector< u32, 32 > >& result_paths
) const
{
	vector< search::vertex_id_type > path;
	search::search_service s( g_allocator );
	s.search( *g_allocator, m_graph, &path, m_list_position.get( ), destination_point, 200.0f, result_paths );
}

unique_propagator_info::unique_propagator_info	( ) :
	voice_params	( g_allocator ),
	prop			( 0 )
{
}

bool compare_propagator_info_by_distance	( propagator_info const& lhs, propagator_info const& rhs )
{
	return lhs.distance_to_listener < rhs.distance_to_listener;
}

void sound_scene::notify_listener	( sound_world const& world )
{
	float3 const listener					= m_list_position.get( );





	vectora< propagator_info > props		( g_allocator );
	sound_instance_proxy_internal* proxy	= m_active_proxies.front( );
	while ( proxy )
	{





		vectora< std::pair< float, float3 > > results	( g_allocator );
		proxy->calculate_graph_position		( listener, results );

		R_ASSERT							( !proxy->get_propagators( ).empty( ) );

		new_sound_propagator* propagator	= proxy->get_propagators().front();
		while ( propagator )
		{
			u32 i = 0;
			while ( i < results.size( ) )
			{
				propagator_info info;
				info.prop						= propagator;
				info.in_graph_position			= results[i].second;
				info.distance_to_listener		= results[i].first;
				props.push_back					( info );
				++i;
			}

			propagator			= proxy->get_propagators().get_next_of_object( propagator );
		}

		proxy					= m_active_proxies.get_next_of_object( proxy );
	}

	std::sort								( props.begin( ), props.end( ), &compare_propagator_info_by_distance );

	vectora< unique_propagator_info > unique	( g_allocator );
	for ( u32 i = 0; i < props.size( ) && i < 20; ++i )
	{

		float distance_to_listener	= props[i].distance_to_listener;
		float distance				= math::max( distance_to_listener, 1.0f );
		distance					= math::min( distance, math::float_max );
		float attenuation			= props[i].prop->get_proxy( ).get_sound_propagator_emitter( ).get_sound_spl( )->get_loudness( distance );
		sound_voice_params params; calculate_channel_matrix	( world.get_panning_lut( ), props[i].prop->get_proxy( ), props[i].in_graph_position, distance, attenuation, params.channel_matrix, params.lp_filter_coeff );

		compare_by_propagator predicate	( props[i].prop );
		vectora< unique_propagator_info >::iterator it	= std::find_if( unique.begin( ), unique.end( ), predicate );
		if ( it != unique.end( ) )
			it->voice_params.push_back	( params );
		else
		{
			unique_propagator_info info;
			info.voice_params.push_back	( params );
			info.prop					= props[i].prop;
			unique.push_back			( info );
		}
	}

	vectora< unique_propagator_info >::iterator it	= unique.begin( );
	vectora< unique_propagator_info >::iterator end	= unique.end( );
	for ( ; it != end; ++it )
		it->prop->distribute_voices		( it->voice_params.size( ), it->voice_params );
}

static u32 cart_to_lut_position	(float re, float im)
{
    u32 pos = 0;
	float denom = math::abs(re) + math::abs(im);
    if(denom > 0.0f)
		pos = (u32)(panning_lut::quadrant_num*math::abs(im) / denom + 0.5);

    if(re < 0.0)
        pos = 2 * panning_lut::quadrant_num - pos;
    if(im < 0.0)
        pos = panning_lut::lut_num - pos;
    return pos%panning_lut::lut_num;
}

static float coeff_calc		( float g, float cw )
{
    float a		= 0.0f;

    /* Be careful with gains < 0.01, as that causes the coefficient
     * head towards 1, which will flatten the signal */
	g = math::max(	g, 0.01f );
    if ( g < 0.9999f ) /* 1-epsilon */
		a = (1 - g*cw - math::sqrt ( 2 * g * ( 1 - cw ) - g * g * ( 1 - cw * cw ))) / (1 - g);

    return a;
}

void sound_scene::calculate_3d_sound	( sound_voice& voice, panning_lut_ptr panning_lut )
{
	sound_instance_proxy_internal const& proxy	= voice.get_proxy( );
	float3 position								= proxy.get_sound_type( ) == volumetric ?
		proxy.get_volumetric_position( get_listenet_position( ) ) : proxy.get_position( );
	float distance								= math::length( position - get_listenet_position( ) );
	float attenuation							= voice.get_sound_spl( )->get_loudness( distance );
	float channel_matrix[2];
	float lp_filter_coeff;
	calculate_channel_matrix
	(
		panning_lut,
		proxy,
		position,
		distance,
		attenuation,
		channel_matrix,
		lp_filter_coeff
	);
	voice.set_output_matrix						( channel_matrix );
	voice.set_low_pass_filter_params				( lp_filter_coeff );
}

void sound_scene::calculate_channel_matrix
(
	panning_lut_ptr const& panning_lut,
	sound_instance_proxy_internal const& proxy,
	float3 const& graph_position,
	float distance,
	float attenuation,
	float* channels_result,
	float& lp_filter_result
) const
{
	if ( proxy.get_sound_type( ) == hud )
	{
		channels_result[0]						= 1.0f;
		channels_result[1]						= 1.0f;
		lp_filter_result							= 0.0f;
		return;
	}

	float dry_gain_hf		= 1.0f;
	u32 frequency				= 44100;
	float head_dampen			= 0.25f;
	u32 num_channels			= 2;
	//get source properties
    float source_volume			= 1.0f;
    float min_volume			= 0.0f;
    float max_volume			= 1.0f;
	float min_dist				= 1.0f;
	float max_dist				= math::float_max;

	float inner_angle			= 360.0f;
	float outer_angle			= 360.0f;
	float outer_gain			= 0.0f;

	sound_type type								= proxy.get_sound_type( );
	if ( type == cone )
	{
		switch( proxy.get_cone_type ( ) )
		{
		case human:
			{
				inner_angle			= 60.0f;
				outer_angle			= 90.0f;
				outer_gain			= 0.5f;
			} break;
		default:NODEFAULT			( );
		}
	}

	float outer_gain_hf			= 1.0f;
	float cone_volume			= 0.0f;
	float cone_hf				= 0.0f;
	bool dry_gain_hf_auto		= true;
	float air_absorption_factor	= 2.0f;

	float3 u, v, n;
	float4x4 matrix;

	n					= get_listenet_orient_front( );
	n.normalize			( );
	
	v					= get_listenet_orient_top( );
	v.normalize			( );

	u					= cross_product ( n, v );
	u.normalize			( );

	matrix.i[0] = u[0]; matrix.i[1] = v[0]; matrix.i[2] = -n[0]; matrix.i[3] = 0.0f;
	matrix.j[0] = u[1]; matrix.j[1] = v[1]; matrix.j[2] = -n[1]; matrix.j[3] = 0.0f;
	matrix.k[0] = u[2]; matrix.k[1] = v[2]; matrix.k[2] = -n[2]; matrix.k[3] = 0.0f;
	matrix.c[0] = 0.0f; matrix.c[1] = 0.0f; matrix.c[2] =  0.0f; matrix.c[3] = 1.0f;

	float3 position					= float3( 0.0f, 0.0f, 0.0f );
	float3 direction				= float3( 0.0f, 0.0f, 0.0f );


	switch( type )
	{
	case cone:			direction		= proxy.get_direction( );
	case point:			position		= graph_position; break;
	case volumetric:	position		= proxy.get_volumetric_position( get_listenet_position( ) ); break;
	default:			NODEFAULT		( );
	}

	// Translate position
	position							-= get_listenet_position( );

	// Transform source position and direction into listener space
	float4 temp_position				( position, 1.0f );
	float4 temp_direction				( direction, 0.0f );

	float4x4 mtx						= math::transpose( matrix );

	position							= ( temp_position * matrix ).xyz( );
	direction							= ( temp_direction * matrix ).xyz( );

	float3 source_to_listener			= -position;
	source_to_listener.normalize_safe	( source_to_listener );
	direction.normalize_safe			( direction );

	//2. Calculate distance attenuation

    float orig_dist						= distance;

	distance							= math::max ( distance, min_dist );
	distance							= math::min ( distance, max_dist );

	// Source Gain + Attenuation






    float dry_gain						= source_volume * attenuation;







	float effective_dist = 0.0f;
    if( min_dist > 0.0f && attenuation < 1.0f )
        effective_dist = min_dist/attenuation - min_dist;

   // Distance-based air absorption
    if( air_absorption_factor > 0.0f && effective_dist > 0.0f)
    {
        float absorb;

        // Absorption calculation is done in dB
		float const AIRABSORBGAINDBHF          = -0.05f;
        absorb = (air_absorption_factor*AIRABSORBGAINDBHF) * effective_dist;
        // Convert dB to linear gain before applying
		absorb = math::pow(10.0f, absorb/20.0f);

        dry_gain_hf *= absorb;
    }

	// apply directional soundcones
	float angle			= math::acos( direction.dot_product( source_to_listener )) * 180.0f / math::pi;
    if ( angle >= inner_angle && angle <= outer_angle )
    {
        float scale		= ( angle - inner_angle ) / ( outer_angle-inner_angle );
        cone_volume		= ( 1.0f + ( outer_gain - 1.0f ) * scale );
		cone_hf			= ( 1.0f + ( outer_gain_hf - 1.0f ) * scale );
    }
    else if(angle > outer_angle)
    {
        cone_volume		= ( 1.0f + ( outer_gain - 1.0f ) );
        cone_hf			= ( 1.0f + ( outer_gain_hf - 1.0f ) );
    }
    else
    {
        cone_volume		= 1.0f;
        cone_hf			= 1.0f;
    }

	// Apply some high-frequency attenuation for sources behind the listener
	// NOTE: This should be dot product({0,0,-1}, ListenerToSource), however
	// that is equivalent to dot product({0,0,1}, SourceToListener), which is
	// the same as SourceToListener[2]
	angle				= math::acos ( source_to_listener[2] ) * 180.0f / math::pi;

    // Sources within the minimum distance attenuate less
    if( orig_dist < min_dist)
        angle			*= orig_dist / min_dist;
    if( angle > 90.0f )
    {
        float scale		= ( angle - 90.0f) / ( 180.1f - 90.0f ); // .1 to account for fp errors
        cone_hf			*= 1.0f - ( head_dampen * scale );
    }

	dry_gain			*= cone_volume;
    if( dry_gain_hf_auto )
        dry_gain_hf		*= cone_hf;

    // Clamp to Min/Max Gain
	dry_gain			= math::min(dry_gain, max_volume );
    dry_gain			= math::max(dry_gain, min_volume );

    // Use energy-preserving panning algorithm for multi-speaker playback
	float length		= math::max( orig_dist, min_dist );
    if( length > 0.0f )
    {
		position.normalize_safe	( float3( 0.0f, 0.0f, 0.0f ));
    }

    u32 pos					= cart_to_lut_position( -position[2], position[0] );
	float* speaker_gain		= &((*panning_lut)[speakers_count * pos]);
	float dir_gain			= math::sqrt( position[0]*position[0] + position[2]*position[2] );

	// elevation adjustment for directional gain. this sucks, but
	// has low complexity
	float ambient_gain		= math::sqrt( 1.0f / num_channels );

	float ch_l				= 0.0f;
	float ch_r				= 0.0f;

	for( u32 s = 0; s < num_channels; ++s )
    {
		u32 chan		= s; // Speaker2Chan
		float gain		= ambient_gain + ( speaker_gain[chan] - ambient_gain ) * dir_gain;
		
		switch ( s )
		{
		case 0: ch_l		= dry_gain * gain; break;
		case 1: ch_r		= dry_gain * gain; break;
		default: NODEFAULT( );
		}
    }





	float channel_matrix[2];
	channel_matrix[0]				= ch_r;
	channel_matrix[1]				= ch_l;




	/* Update filter coefficients. */
	float const LOWPASSFREQCUTOFF   = 5000;
	float cw						= math::cos	( math::pi_x2 * LOWPASSFREQCUTOFF / frequency );

	float iir_filter_coeff			= coeff_calc	( math::sqrt( dry_gain_hf ), cw ) * 1;

	if ( type == volumetric )
	{
		float3 volumetric_listener_pos	= get_listenet_position( );
		float3 volumetric_source_pos	= proxy.get_volumetric_position( volumetric_listener_pos );
		float volumetric_distance		= ( volumetric_listener_pos - volumetric_source_pos ).length( );
		
		if ( volumetric_distance > proxy.get_volumetric_radius( ))
		{
			channels_result[0]					= channel_matrix[0];
			channels_result[1]					= channel_matrix[1];
			lp_filter_result					= 1.0f - iir_filter_coeff;
			return;
		}

		float coeff					= volumetric_distance / proxy.get_volumetric_radius( );
		channel_matrix[0]			*= coeff;
		channel_matrix[1]			*= coeff;
		
		float stereo_coeff			= 1.0f - coeff; 
		channel_matrix[0]			+= stereo_coeff;
		channel_matrix[1]			+= stereo_coeff;

		if ( channel_matrix[0] > 1.0f )
			channel_matrix[0]			= 1.0f;

		if ( channel_matrix[1] > 1.0f )
			channel_matrix[1]			= 1.0f;

		if ( math::is_similar( volumetric_listener_pos, volumetric_source_pos ) )
			iir_filter_coeff			= 0.0f;
	}

	channels_result[0]				= channel_matrix[0];
	channels_result[1]				= channel_matrix[1];
	lp_filter_result				= 1.0f - iir_filter_coeff;
}

void sound_scene::calculate_hdr_audio	( )
{
}

struct compare_receivers_predicate : private boost::noncopyable
{
	inline compare_receivers_predicate	( sound_receiver* const receiver )
		:	m_receiver					( receiver )
	{
	}

	inline bool	operator ( )			( receiver_collision* const container ) const
	{
		return container->get_sound_receiver( ) == m_receiver;
	}
	
	sound_receiver* const				m_receiver;
}; // struct receivers_erase_predicate


void sound_scene::register_receiver		( sound_receiver* receiver, atomic_half3* position )
{
	LOG_DEBUG							( "sound_scene::register_receiver" );
	compare_receivers_predicate pred	( receiver );
	receiver_collision* new_receiver	= m_receivers.find_if( pred );

	R_ASSERT							( new_receiver == 0 );

	if ( m_receiver_collisions_allocator->total_size( ) == m_receiver_collisions_allocator->allocated_size( ) )
	{
		LOG_ERROR					( "can't allocate receiver_collision, memory is full" );
		return;
	}

	new_receiver			= VOSTOK_NEW_IMPL( m_receiver_collisions_allocator.c_ptr(), receiver_collision )( receiver, position );

	m_receivers.push_back	( new_receiver );

	float4x4 scale			= math::create_scale( new_receiver->get_collision_object( )->get_aabb().extents( ));
	float4x4 translation	= math::create_translation( new_receiver->get_position( ) );
	float4x4 local_to_world	= scale * translation;
	m_spatial_tree->insert	
	(
		new_receiver->get_collision_object( ),
		local_to_world
	);

}

void sound_scene::unregister_receiver	( world_user& user, sound_receiver* receiver )
{
	LOG_DEBUG							( "unregister_receiver_impl" );
	compare_receivers_predicate pred	( receiver );
	receiver_collision* collision		= m_receivers.find_if( pred );

	if ( collision )
	{
		m_receivers.erase				( collision );
		m_spatial_tree->erase			( collision->get_collision_object() );
		collision->delete_position		( *this );
		VOSTOK_DELETE_IMPL				( m_receiver_collisions_allocator.c_ptr( ), collision );
	}
	else
		LOG_ERROR						( "attempt to delete not unregistered receiver" );

	user.on_receiver_deleted			( (u64)receiver );

	if ( !m_receivers.empty() )
		LOG_INFO	( "sound receiver unregistered, but list of registered receivers isn't empty" );
	else
		LOG_INFO	( "list of registered receivers is empty" );
}

void sound_scene::stop_produce_sound		( sound_instance_proxy_internal& proxy ) const
{
	new_sound_propagator* prop			= proxy.get_propagators( ).front( );
	while ( prop )
	{
		prop->stop_produce				( );
		prop							= proxy.get_propagators( ).get_next_of_object( prop );
	}
}

void sound_scene::stop_propagate_sound	( sound_instance_proxy_internal& proxy )
{
	LOG_DEBUG						( "sound_scene::stop_propagate_sound" );
	new_sound_propagator* prop		= proxy.get_propagators( ).front( );
	while ( prop )
	{
		prop->stop_propagation		( );
		new_sound_propagator* next	= proxy.get_propagators( ).get_next_of_object( prop );
		R_ASSERT					( prop->can_be_deleted( ) );

		delete_sound_propagator		( proxy, prop );
	
		prop						= next;
	}
}

void sound_scene::pause_produce_sound	( sound_instance_proxy_internal& proxy ) const
{
	LOG_DEBUG						( "sound_scene::pause_produce_sound" );
	new_sound_propagator* prop		= proxy.get_propagators( ).front( );
	while ( prop )
	{
		prop->stop_produce			( );
		prop						= proxy.get_propagators( ).get_next_of_object( prop );
	}
}

void sound_scene::pause_propagate_sound	( sound_instance_proxy_internal& proxy ) const
{
	LOG_DEBUG						( "sound_scene::pause_propagate_sound" );
	new_sound_propagator* prop		= proxy.get_propagators( ).front( );
	while ( prop )
	{
		prop->pause_propagation		( );
		prop						= proxy.get_propagators( ).get_next_of_object( prop );
	}
}

void sound_scene::resume_produce_sound	( sound_instance_proxy_internal& proxy )
{
}

void sound_scene::resume_propagate_sound	( sound_instance_proxy_internal& proxy ) const
{
	LOG_DEBUG						( "sound_scene::resume_propagate_sound" );
	new_sound_propagator* prop		= proxy.get_propagators( ).front( );
	while ( prop )
	{
		prop->resume_propagation		( );
		prop						= proxy.get_propagators( ).get_next_of_object( prop );
	}

}

void sound_scene::set_listener_properties( float4x4 const& inv_view_matrix )
{
	set_listener_properties			( inv_view_matrix.c.xyz(), inv_view_matrix.k.xyz(), inv_view_matrix.j.xyz() );
}

void sound_scene::set_listener_properties	(	
												float3 const& position,
												float3 const& orient_front,
												float3 const& orient_top
											)
{
	R_ASSERT						( position.valid( ) && orient_front.valid( ) && orient_top.valid( ) );
	m_is_listener_position_set = true;
	m_list_position.set		( position );
	m_list_orient_front.set	( orient_front );
	m_list_orient_top.set	( orient_top );
}

void sound_scene::fade_in	( sound_world& world, u32 time_in_msec )
{
	LOG_DEBUG						( "sound_scene::fade_in" );
	if ( !m_is_active )
	{
		world.add_scene_to_active	( *this );
		m_is_active					= true;
	}

	m_fade_state			= fade_in_state;
	m_fade_in_time			= time_in_msec;

	m_fade_vol_per_msec		= 1.0f;
	if ( time_in_msec != 0 )
		m_fade_vol_per_msec		= 1.0f / time_in_msec;

	resume_propagate_all_sounds			( );
}

void sound_scene::fade_out	( u32 time_in_msec )
{
	R_ASSERT				( m_is_active );
	m_fade_state			= fade_out_state;
	m_fade_out_time			= time_in_msec;

	m_fade_vol_per_msec		= 1.0f;
	if ( time_in_msec != 0 )
		m_fade_vol_per_msec		= 1.0f / time_in_msec;
}

void sound_scene::process_fade	( sound_world& world, u64 time_delta )
{
	R_ASSERT					( m_is_active );

	if ( m_fade_state == fade_in_state )
	{
		m_volume		+= m_fade_vol_per_msec * time_delta;
		if ( m_volume >= 1.0f )
		{
			m_volume		= 1.0f;
			m_fade_state	= none;
		}
		if ( m_submix_voice )
			m_submix_voice->SetVolume( m_volume );
	}
	else if ( m_fade_state == fade_out_state )
	{
		m_volume		-= m_fade_vol_per_msec * time_delta;
		
		if ( m_volume < 0.0f || math::is_zero( m_volume ) )
		{
			m_volume								= 0.0f;
			m_fade_state							= none;
			pause_propagate_all_sounds				( );

			world.remove_scene_from_active			( *this );
			m_is_active								= false;
		}
		LOG_DEBUG								( "volume: %f", m_volume );
		if ( m_submix_voice )
			m_submix_voice->SetVolume( m_volume );
	}
}

atomic_half3* sound_scene::create_receiver_position	( )
{
	if ( m_receiver_positions_allocator->total_size( ) == m_receiver_positions_allocator->allocated_size( ) )
	{
		LOG_ERROR					( "can't allocate receiver_position, memory is full" );
		return						0;
	}

	atomic_half3* pos	= VOSTOK_NEW_IMPL( m_receiver_positions_allocator.c_ptr(), atomic_half3 )( );
	return pos;
}

void sound_scene::delete_receiver_position	( atomic_half3* pos )
{
	R_ASSERT			( pos );
	VOSTOK_DELETE_IMPL	( m_receiver_positions_allocator.c_ptr(), pos );
	pos					= 0;
}

void sound_scene::update_receivers_position	( )
{
	receiver_collision* rc	= m_receivers.front( );
	while ( rc )
	{
		m_spatial_tree->move			
		(
			rc->get_collision_object( ),
			math::create_scale( rc->get_collision_object( )->get_aabb().extents( ) ) *
			math::create_translation( rc->get_position( ) )
		);

		rc					= m_receivers.get_next_of_object( rc );
	}
}

void sound_scene::add_active_voice		( sound_voice& voice )
{
	R_ASSERT							( !m_active_voices.contains_object( &voice ) );
	m_active_voices.push_back			( &voice );
}

void sound_scene::remove_active_voice	( sound_voice& voice )
{
	R_ASSERT							( m_active_voices.contains_object( &voice ) );
	m_active_voices.erase				( &voice );
}

void sound_scene::update_stats			( sound_debug_stats& stats ) const
{
	stats.update_statistic				( );
}

#ifndef MASTER_GOLD

void sound_scene::enable_debug_stream_writing	( )
{
	R_ASSERT			( m_is_debug_stream_writing_enabled == false );
	R_ASSERT			( m_debug_snapshot == 0 );

	XAPO_REGISTRATION_PROPERTIES props;
	props.MajorVersion					= VOSTOK_ENGINE_VERSION;
	props.MinorVersion					= VOSTOK_ENGINE_SUBVERSION;
	props.Flags							= XAPOBASE_DEFAULT_FLAG;
	props.MinInputBufferCount			= 1;
	props.MaxInputBufferCount			= 1;
	props.MinOutputBufferCount			= 1;
	props.MaxOutputBufferCount			= 1;

	m_debug_snapshot					= VOSTOK_NEW_IMPL( g_allocator, debug_snapshot )( props );
	m_debug_snapshot->AddRef			( );

	XAUDIO2_EFFECT_DESCRIPTOR descriptor;
	descriptor.InitialState				= true;
	descriptor.OutputChannels			= 2;
	descriptor.pEffect					= m_debug_snapshot;

	XAUDIO2_EFFECT_CHAIN chain;
	chain.EffectCount = 1;
	chain.pEffectDescriptors = &descriptor;

	R_ASSERT							( m_submix_voice );
	m_submix_voice->SetEffectChain		( &chain );

	threading::interlocked_exchange		( m_is_debug_stream_writing_enabled, 1 );
}

void sound_scene::disable_debug_stream_writing	( )
{

	R_ASSERT							( m_submix_voice );
	m_submix_voice->SetEffectChain		( 0 );

	R_ASSERT							( m_is_debug_stream_writing_enabled );
	R_ASSERT							( m_debug_snapshot != 0 );
	VOSTOK_DELETE_IMPL					( g_allocator, m_debug_snapshot );


	m_debug_snapshot					= 0;
	threading::interlocked_exchange		( m_is_debug_stream_writing_enabled, 0 );
}


static void fill_path_name			( fs::path_string& dest, pcstr extention )
{
	SYSTEMTIME					date_time;
	GetLocalTime				( &date_time );
	dest.appendf				(	"%s/sound_dumps/scene_buffers_dump_%02d_%02d_%04d_%02d_%02d_%02d.%s",
									fs_new::portable_path_string::convert( core::user_data_directory() ).c_str(),
									date_time.wDay,
									date_time.wMonth,
									date_time.wYear,
									date_time.wHour, 
									date_time.wMinute,
									date_time.wSecond,
									extention
								 );
}


void sound_scene::dump_debug_stream_writing	( ) const
{
	R_ASSERT							( m_is_debug_stream_writing_enabled );
	R_ASSERT							( m_debug_snapshot );
	memory::writer writer				( &debug::g_mt_allocator );
	m_debug_snapshot->dump				( writer );
	fs::path_string path;
	fill_path_name						( path, "raw" );
	writer.save_to						( path.c_str( ) );
	writer.clear						( );

	path.clear							( );
	fill_path_name						( path, "txt" );
	
	configs::lua_config_ptr cfg			= configs::create_lua_config( path.c_str( ) );
	configs::lua_config_value val		= cfg->get_root( )["scene"];

	val["listener"]["position"]			= m_list_position.get( );
	val["listener"]["orient_front"]		= m_list_orient_front.get( );
	val["listener"]["orient_top"]		= m_list_orient_top.get( );

	val["active_proxies_count"]			= m_active_proxies.size( );

	sound_instance_proxy_internal* proxy= m_active_proxies.front( );
	u32 i								= 0;
	while( proxy )
	{
		configs::lua_config_value prx_val	= val["proxies"][i++];
		proxy->dump_debug_snapshot			( prx_val );
		proxy								= m_active_proxies.get_next_of_object( proxy );
	}
	cfg->save							( configs::target_sources );


}
#endif // #ifndef MASTER_GOLD

sound_scene_statistic* sound_scene::create_statistic	( ) const
{
	sound_scene_statistic* statistic			= VOSTOK_NEW_IMPL( g_allocator, sound_scene_statistic )( );
	statistic->values.m_registered_receivers_count	= m_receivers.size		( );
	statistic->values.m_active_proxies_count		= m_active_proxies.size	( );

	sound_voice* voice = m_active_voices.front( );
	while ( voice )
	{
		++statistic->values.m_active_voices_count[voice->get_channels_num( ) - 1];
		voice = m_active_voices.get_next_of_object( voice );
	}

	sound_instance_proxy_internal* proxy = m_active_proxies.front( );
	while ( proxy )
	{
		++statistic->values.m_sound_types[proxy->get_sound_type( )];
		statistic->values.m_propagators_count += proxy->get_propagators( ).size( );
		proxy = m_active_proxies.get_next_of_object( proxy );
	}
	return statistic;
}

void sound_scene::delete_statistic			( sound_scene_statistic* statistic ) const
{
	if ( statistic )
	{
		proxy_statistic* prx_stats			= statistic->m_proxies_statistic.front( );
		while( prx_stats )
		{
			proxy_statistic* prx_stats_next		= statistic->m_proxies_statistic.get_next_of_object( prx_stats );
			propagator_statistic* prop_stats	= prx_stats->m_propagator_statistics.front( );
			while ( prop_stats )
			{
				propagator_statistic* prop_stats_next	= prx_stats->m_propagator_statistics.get_next_of_object( prop_stats );
				prx_stats->m_propagator_statistics.erase( prop_stats );
				VOSTOK_DELETE_IMPL						( g_allocator, prop_stats );
				prop_stats								= prop_stats_next;
			}
			statistic->m_proxies_statistic.erase( prx_stats );
			VOSTOK_DELETE_IMPL					( g_allocator, prx_stats );
			prx_stats							= prx_stats_next;
		}
		VOSTOK_DELETE_IMPL					( g_allocator, statistic );
		statistic							= 0;
	}
}
void sound_scene::pause							( )
{
	R_ASSERT								( !m_is_paused );
	pause_propagate_all_sounds				( );
	m_is_paused			= true;
}

void sound_scene::resume						( )
{
	R_ASSERT								( m_is_paused );
	resume_propagate_all_sounds				( );
	m_is_paused			= false;
}

bool sound_scene::is_paused						( ) const
{
	return m_is_paused;
}

void sound_scene::pause_propagate_all_sounds	( ) const
{
	sound_instance_proxy_internal* proxy		= m_active_proxies.front( );
	while ( proxy )
	{
		pause_propagate_sound					( *proxy );
		proxy									= m_active_proxies.get_next_of_object( proxy );
	}
}

void sound_scene::resume_propagate_all_sounds	( ) const
{
	sound_instance_proxy_internal* proxy		= m_active_proxies.front( );
	while ( proxy )
	{
		resume_propagate_sound					( *proxy );
		proxy									= m_active_proxies.get_next_of_object( proxy );
	}
}

void fill_x3daudio_vector	(
	_D3DVECTOR&		vec,
	float			x,
	float			y,
	float			z
)
{
	vec.x							= x;
	vec.y							= y;
	vec.z							= z;
}

void fill_x3daudio_vector	( _D3DVECTOR& dest_vec, float3 const& vec )
{
	dest_vec.x						= vec.x;
	dest_vec.y						= vec.y;
	dest_vec.z						= vec.z;
}

void sound_scene::x3daudio_calculate	( sound_world const&, sound_voice& )
{
	X3DAUDIO_LISTENER listener;
	fill_x3daudio_vector				( listener.Velocity, 0.0f, 0.0f, 0.0f );
	fill_x3daudio_vector				( listener.Position, m_list_position.get( ) );
	fill_x3daudio_vector				( listener.OrientFront, m_list_orient_front.get( ) );
	fill_x3daudio_vector				( listener.OrientTop, m_list_orient_top.get( ) );
	listener.pCone					= 0;
}

void sound_scene::set_graph	( render::culling::portal_sector_structure_ptr& graph )
{
	m_graph							= graph;
}

bool sound_scene::graph_exist	( ) const
{
	return								m_graph.c_ptr( ) != 0;
}

float3 sound_scene::get_portal_center	( u32 portal_id ) const
{
	return								m_graph->get_portals( )[portal_id].get_points( )[0];
}

struct closets_point_predicate
{
	closets_point_predicate	( float min_value, float3 const& point ) :
		closest_point		( point ),
		min_val				( min_value )
	{}

	void operator( )( std::pair< float, float3 > const& value )
	{
		if ( min_val > value.first )
		{
			min_val			= value.first;
			closest_point	= value.second;
		}
	}

	float3	closest_point;
	float	min_val;
};

float3 closest_point_on_segment	(
	float3 const&		point,
	float3 const&		segment_origin,
	float3 const&		segment_displacement
)
{
	float domen_value					= ( point - segment_origin ).dot_product( segment_displacement ) / segment_displacement.squared_length( );
	domen_value							= math::clamp_r( domen_value, 0.f, 1.f );
	return								segment_origin + segment_displacement * domen_value;
}

float3 sound_scene::get_portal_nearest_point	( u32 portal_id, float3 segment_start, float3 segment_end ) const
{
	render::culling::portal const& portal	= m_graph->get_portals( )[portal_id];

	float3 p1							= segment_start;
	float3 p3							= segment_end;
	float3 p2							= portal.get_points( )[0];
	float3 p4							= portal.get_points( )[1];

	float3 d1							= p3 - p1;
	float3 d2							= p4 - p2;

	vectora< std::pair< float, float3 > > portal_segments( g_allocator );

	closets_point_predicate p( ( p1 - sound::closest_point_on_segment( p1, p2, d2 ) ).squared_length( ), sound::closest_point_on_segment( p1, p2, d2 ) );
	portal_segments.push_back( std::make_pair( ( p1 - sound::closest_point_on_segment( p1, p2, d2 ) ).squared_length( ), sound::closest_point_on_segment( p1, p2, d2 ) ) );
	portal_segments.push_back( std::make_pair( ( p3 - sound::closest_point_on_segment( p3, p2, d2 ) ).squared_length( ), sound::closest_point_on_segment( p3, p2, d2 ) ) );
	portal_segments.push_back( std::make_pair( ( p2 - sound::closest_point_on_segment( p2, p1, d1 ) ).squared_length( ), sound::closest_point_on_segment( p2, p1, d1 ) ) );
	portal_segments.push_back( std::make_pair( ( p4 - sound::closest_point_on_segment( p4, p1, d1 ) ).squared_length( ), sound::closest_point_on_segment( p4, p1, d1 ) ) );

	p2									= portal.get_points( )[1];
	p4									= portal.get_points( )[2];

	d1									= p3 - p1;
	d2									= p4 - p2;

	portal_segments.push_back( std::make_pair( ( p1 - sound::closest_point_on_segment( p1, p2, d2 ) ).squared_length( ), sound::closest_point_on_segment( p1, p2, d2 ) ) );
	portal_segments.push_back( std::make_pair( ( p3 - sound::closest_point_on_segment( p3, p2, d2 ) ).squared_length( ), sound::closest_point_on_segment( p3, p2, d2 ) ) );
	portal_segments.push_back( std::make_pair( ( p2 - sound::closest_point_on_segment( p2, p1, d1 ) ).squared_length( ), sound::closest_point_on_segment( p2, p1, d1 ) ) );
	portal_segments.push_back( std::make_pair( ( p4 - sound::closest_point_on_segment( p4, p1, d1 ) ).squared_length( ), sound::closest_point_on_segment( p4, p1, d1 ) ) );

	p2									= portal.get_points( )[2];
	p4									= portal.get_points( )[3];

	d1									= p3 - p1;
	d2									= p4 - p2;

	portal_segments.push_back( std::make_pair( ( p1 - sound::closest_point_on_segment( p1, p2, d2 ) ).squared_length( ), sound::closest_point_on_segment( p1, p2, d2 ) ) );
	portal_segments.push_back( std::make_pair( ( p3 - sound::closest_point_on_segment( p3, p2, d2 ) ).squared_length( ), sound::closest_point_on_segment( p3, p2, d2 ) ) );
	portal_segments.push_back( std::make_pair( ( p2 - sound::closest_point_on_segment( p2, p1, d1 ) ).squared_length( ), sound::closest_point_on_segment( p2, p1, d1 ) ) );
	portal_segments.push_back( std::make_pair( ( p4 - sound::closest_point_on_segment( p4, p1, d1 ) ).squared_length( ), sound::closest_point_on_segment( p4, p1, d1 ) ) );

	p2									= portal.get_points( )[3];
	p4									= portal.get_points( )[0];

	d1									= p3 - p1;
	d2									= p4 - p2;

	portal_segments.push_back( std::make_pair( ( p1 - sound::closest_point_on_segment( p1, p2, d2 ) ).squared_length( ), sound::closest_point_on_segment( p1, p2, d2 ) ) );
	portal_segments.push_back( std::make_pair( ( p3 - sound::closest_point_on_segment( p3, p2, d2 ) ).squared_length( ), sound::closest_point_on_segment( p3, p2, d2 ) ) );
	portal_segments.push_back( std::make_pair( ( p2 - sound::closest_point_on_segment( p2, p1, d1 ) ).squared_length( ), sound::closest_point_on_segment( p2, p1, d1 ) ) );
	portal_segments.push_back( std::make_pair( ( p4 - sound::closest_point_on_segment( p4, p1, d1 ) ).squared_length( ), sound::closest_point_on_segment( p4, p1, d1 ) ) );

	std::for_each( portal_segments.begin( ), portal_segments.end( ), p );
	return p.closest_point;
}

bool sound_scene::is_segment_pass_portal	( u32 portal_id, float3 segment_start, float3 segment_end ) const
{
	render::culling::portal const& portal	= m_graph->get_portals( )[portal_id];
	float result_a, result_b				= 0.0f;

	return collision::test_triangle( portal.get_points( )[0], portal.get_points( )[1], portal.get_points( )[2], segment_end, ( segment_start - segment_end ).normalize( ), ( segment_end - segment_start ).length( ), result_a ) || collision::test_triangle( portal.get_points( )[0], portal.get_points( )[2], portal.get_points( )[3], segment_end, ( segment_start - segment_end ).normalize( ), ( segment_end - segment_start ).length( ), result_b );
}


} // namespace sound
} // namespace vostok
