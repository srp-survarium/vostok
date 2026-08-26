////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_INLINE_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_INLINE_H_INCLUDED

namespace survarium {

template < typename T >
inline weapon_sound_events_handler_state_cook< T >::weapon_sound_events_handler_state_cook( ) :
	super( weapon_sound_events_handler_state_cook_class< T >( ), reuse_false, use_current_thread_id, use_current_thread_id )
{
	resources::register_cook( this );
}

template < typename T >
mutable_buffer weapon_sound_events_handler_state_cook< T >::allocate_resource(
	resources::query_result_for_cook&		in_query,
	const_buffer							raw_file_data,
	const bool								file_exist
)
{
	return mutable_buffer( VOSTOK_MALLOC_IMPL( *g_allocator, sizeof( T ), "weapon_sound_events_handler_state" ), sizeof( T ) );
}

template < typename T >
void weapon_sound_events_handler_state_cook< T >::deallocate_resource( void* buffer )
{
	g_allocator->free_impl( buffer );
}

template < typename T >
void weapon_sound_events_handler_state_cook< T >::create_resource(
	resources::query_result_for_cook&		parent,
	const_buffer							raw_file_data,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
	typedef weapon_sound_events_handler_state_cook_traits< T > traits_type;
	typedef buffer_vector< resources::request > requests_buffer_type;

	weapon_state_creation_params const* const params = static_cast< weapon_state_creation_params const* >( raw_file_data.c_ptr( ) );
	configs::binary_config_value cfg = configs::binary_config_value( );
	if ( !parent.user_data( )->try_get( cfg ) )
	{
		parent.finish_query( result_error );
		return;
	}

	config_params config_parameters;
	u32 const max_requests_count = traits_type::animations_count +
		( cfg.value_exists( "sounds" ) && cfg[ "sounds" ].value_exists( "first_view" ) ?
			cfg[ "sounds" ][ "first_view" ].size( ) * 2 : 0 );
	requests_buffer_type requests(
		ALLOCA( sizeof( resources::request ) * max_requests_count ),
		max_requests_count
	);

	configs::binary_config_value const& animations_cfg = cfg[ "animations" ];
	for ( u32 i = 0; i != traits_type::weapon_animations_count; ++i )
	{
		resources::request request = { animations_cfg[ i ], resources::animation_class }; requests.push_back( request );
	}

	if ( cfg.value_exists( "user_animations" ) )
	{
		configs::binary_config_value const& user_animations_cfg = cfg[ "user_animations" ];
		for ( u32 i = 0; i != traits_type::user_animations_count; ++i )
		{
			resources::request request = { user_animations_cfg[ i ], resources::animation_class }; requests.push_back( request );
		}
	}

	if ( cfg.value_exists( "sounds" ) )
	{
		configs::binary_config_value const& sounds_cfg = cfg[ "sounds" ];

		configs::binary_config_value const& first_view_sounds_cfg = sounds_cfg[ "first_view" ];
		u32 const first_view_sounds_count = first_view_sounds_cfg.size( );
		if ( first_view_sounds_count )
		{
			for ( u32 i = 0; i != first_view_sounds_count; ++i )
			{
				resources::request request = { first_view_sounds_cfg[ i ][ 1 ], (resources::class_id_enum)(u32)first_view_sounds_cfg[ i ][ 0 ] }; requests.push_back( request );
			}
		}

		configs::binary_config_value const& third_view_sounds_cfg = sounds_cfg[ "third_view" ];
		u32 const third_view_sounds_count = third_view_sounds_cfg.size( );
		if ( third_view_sounds_count )
		{
			for ( u32 i = 0; i != third_view_sounds_count; ++i )
			{
				resources::request request = { third_view_sounds_cfg[ i ][ 1 ], (resources::class_id_enum)(u32)third_view_sounds_cfg[ i ][ 0 ] }; requests.push_back( request );
			}
		}

		config_parameters.stop_sounds_on_state_finalize = sounds_cfg[ "stop_sounds_on_state_finalize" ];
		config_parameters.simultaneous_sounds_queue_size = sounds_cfg[ "simultaneous_sounds_queue_size" ];
	}

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &weapon_sound_events_handler_state_cook< T >::on_subresources_ready, this, _1, in_out_unmanaged_resource_buffer, params, config_parameters ),
		g_allocator,
		NULL,
		&parent
	);
	parent.finish_query( result_postponed );
}

template < typename T >
inline void weapon_sound_events_handler_state_cook< T >::on_subresources_ready(
	resources::queries_result&		data,
	mutable_buffer					buffer,
	weapon_state_creation_params const* const	params,
	config_params					config_parameters
)
{
	typedef weapon_sound_events_handler_state_cook_traits< T > traits_type;
	typedef buffer_vector< resources::managed_resource_ptr > skeleton_animations_buffer_type;

	skeleton_animations_buffer_type animations(
		ALLOCA( sizeof( resources::managed_resource_ptr ) * traits_type::animations_count ),
		traits_type::animations_count
	);
	for ( u32 i = 0; i != traits_type::animations_count; ++i )
		animations.push_back( data[ i ].get_managed_resource( ) );

	u32 const total_sounds_count = data.size( ) - traits_type::animations_count;
	sound::sound_emitter_ptr* const buffer_for_sounds = total_sounds_count ?
		(sound::sound_emitter_ptr*)VOSTOK_MALLOC_IMPL(
			*g_allocator,
			total_sounds_count * ( sizeof( sound::sound_emitter_ptr ) + 32 * sizeof( sound::sound_instance_proxy_ptr ) ),
			"weapon_sound_events_handler_state_sounds"
		) : NULL;

	if ( total_sounds_count )
	{
		for ( u32 i = 0; i != total_sounds_count; ++i ) new ( buffer_for_sounds + i ) sound::sound_emitter_ptr( static_cast_resource_ptr< sound::sound_emitter_ptr >( data[ i + traits_type::animations_count ].get_unmanaged_resource( ) ) );
	}

	T* const object_to_cook = new_state(
		buffer,
		params,
		animations.begin( ),
		animations.size( ),
		buffer_for_sounds,
		total_sounds_count / 2,
		config_parameters
	);

	data.get_parent_query( )->set_unmanaged_resource(
		object_to_cook,
		resources::memory_usage_type( resources::nocache_memory, sizeof( T ) )
	);
	data.get_parent_query( )->finish_query( result_success );
}

template < typename T >
void weapon_sound_events_handler_state_cook< T >::destroy_resource( resources::unmanaged_resource* const resource )
{
	static_cast< T* >( resource )->~T( );
	g_allocator->free_impl( (pvoid)static_cast< T* >( resource )->m_buffer_for_sounds );
}

template < typename T >
inline T* weapon_sound_events_handler_state_cook< T >::new_state(
	mutable_buffer		buffer,
	weapon_state_creation_params const* const	params,
	resources::managed_resource_ptr const* const	animations,
	const u8			animations_count,
	void* const			sounds,
	const u8			sounds_count,
	config_params const&	config_parameters
)
{
	return new ( buffer.c_ptr( ) ) T(
			static_cast< weapon& >( params->weapon ),
			weapon_sound_events_handler_state_cook_traits< T >::animation_time_scale_calculator( animations[ 0 ], *params ),
			animations,
			animations_count,
			sounds,
			sounds_count,
			config_parameters.stop_sounds_on_state_finalize,
			config_parameters.simultaneous_sounds_queue_size
		);
}

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_INLINE_H_INCLUDED
