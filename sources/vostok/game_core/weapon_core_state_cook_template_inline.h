// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_INLINE_H_INCLUDED
#define WEAPON_CORE_STATE_COOK_TEMPLATE_INLINE_H_INCLUDED

namespace survarium {

template < typename T >
inline weapon_core_state_cook_template<T>::weapon_core_state_cook_template( ) :
	resources::unmanaged_cook( T::resource_class, reuse_false, use_current_thread_id, use_current_thread_id )
{
#line 16
	resources::register_cook( this );
}

template < typename T >
inline weapon_core_state_cook_template<T>::~weapon_core_state_cook_template( )
{
}

template < typename T >
inline mutable_buffer weapon_core_state_cook_template<T>::allocate_resource( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist )
{
#line 29
	return mutable_buffer( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( T ), "weapon_core_state" ), sizeof( T ) );
}

template < typename T >
inline void weapon_core_state_cook_template<T>::deallocate_resource( void* arg_0 )
{
	VOSTOK_FREE_IMPL( g_allocator, (resources::resource_base*&)arg_0 );
}


template < typename T >
#line 39
inline void weapon_core_state_cook_template<T>::create_resource( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer )
{
	weapon_state_creation_params const*	params	= static_cast< weapon_state_creation_params const* >( raw_file_data.c_ptr( ) );
	configs::binary_config_value		cfg;
	if ( !parent.user_data( )->try_get( cfg ) )
	{
#line 44
		DEBUG_BREAK		( );
		parent.finish_query( result_error );
		return;
	}

	typedef fixed_vector< resources::request, T::total_animations_count >	requests_fixed_type;

#line 50
	requests_fixed_type	requests;

#line 55
	for ( u32 i = 0; i != T::total_animations_count; ++i ) requests.push_back( resources::create_request( cfg["animations"][ i ], resources::animation_class ) );

#line 58
	cfg.value_exists( "user_animations" ); // retail MASTER_GOLD keeps the call, result unused

#line 68
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
	typedef fixed_vector< resources::managed_resource_ptr, T::total_animations_count >	skeleton_animations_fixed_type;

#line 85
	skeleton_animations_fixed_type	animations;
	for ( u32 i = 0; i != T::total_animations_count; ++i ) animations.push_back( static_cast_resource_ptr< resources::managed_resource_ptr >( data[ i ].get_managed_resource( ) ) );

#line 89
	T* object_to_cook = new_object( buffer, params, animations.begin( ), animations.size( ) );

#line 91
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
