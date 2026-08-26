////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_INLINE_H_INCLUDED
#define WEAPON_CORE_STATE_COOK_TEMPLATE_INLINE_H_INCLUDED

namespace survarium {

template < typename T >
inline weapon_core_state_cook_template<T>::weapon_core_state_cook_template( ) :
	resources::unmanaged_cook( T::resource_class, reuse_false, use_current_thread_id, use_current_thread_id )
{
	resources::register_cook( this );
}

template < typename T >
inline weapon_core_state_cook_template<T>::~weapon_core_state_cook_template( )
{
}

template < typename T >
inline mutable_buffer weapon_core_state_cook_template<T>::allocate_resource( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist )
{
	return mutable_buffer( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( T ), "weapon_core_state" ), sizeof( T ) );
}

template < typename T >
inline void weapon_core_state_cook_template<T>::deallocate_resource( void* arg_0 )
{
	VOSTOK_FREE_IMPL( g_allocator, (resources::resource_base*&)arg_0 );
}


template < typename T >
inline void weapon_core_state_cook_template<T>::create_resource( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer )
{
	weapon_state_creation_params const*	params	= static_cast< weapon_state_creation_params const* >( raw_file_data.c_ptr( ) );
	configs::binary_config_value		cfg;
	if ( !parent.user_data( )->try_get( cfg ) )
	{
		DEBUG_BREAK		( );
		parent.finish_query( result_error );
		return;
	}

	typedef fixed_vector< resources::request, 4 >	requests_fixed_type;

	requests_fixed_type	requests;

	for ( u32 i = 0; i != 4; ++i ) requests.push_back( resources::create_request( cfg["animations"][ i ], resources::animation_class ) );

	ASSERT( UNKNOWN_EXPRESSION_T( cfg.value_exists( "user_animations" ) ) );

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &weapon_core_state_cook_template< T >::on_subresources_ready, this, _1, in_out_unmanaged_resource_buffer, params ),
		g_allocator,
		NULL,
		&parent
	);
	parent.finish_query( result_postponed );
}

template < typename T >
inline void weapon_core_state_cook_template<T>::on_subresources_ready( resources::queries_result& data, mutable_buffer buffer, weapon_state_creation_params const* params )
{
	typedef fixed_vector< resources::managed_resource_ptr, 4 >	skeleton_animations_fixed_type;

	skeleton_animations_fixed_type	animations;
	for ( u32 i = 0; i != 4; ++i ) animations.push_back( static_cast_resource_ptr< resources::managed_resource_ptr >( data[ i ].get_managed_resource( ) ) );

	T* object_to_cook = new_object( buffer, params, animations.begin( ), animations.size( ) );

	data.get_parent_query( )->set_unmanaged_resource( object_to_cook, resources::memory_usage_type( resources::nocache_memory, sizeof( T ) ) );
	data.get_parent_query( )->finish_query( result_success );
}

template < typename T >
inline void weapon_core_state_cook_template<T>::destroy_resource( resources::unmanaged_resource* resource )
{
	T* state = static_cast< T* >( resource );
	state->~T( );
}

} // namespace survarium

#endif // #ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_INLINE_H_INCLUDED
