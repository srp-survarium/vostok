// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/weapon_user_animations_container_cook.h>

#include <vostok/game_core/weapon_user_animations_container.h>
#include <vostok/configs_binary_config.h>

namespace survarium {

weapon_user_animations_container_cook::weapon_user_animations_container_cook( )
	: resources::translate_query_cook( resources::animation_container_class, reuse_true, use_current_thread_id )
{
}

// claude@NOTE: 0-local target (matches this source). Base differs only in how the two delete_helper
// args are marshalled (target pushes &resource then strip_pointer(g_allocator); base defers the
// pointer into edi, push edi/pop edi, add esp,4 vs 8). strip_pointer/delete_helper arg-eval-order
// + register ceiling - same VOSTOK_DELETE_IMPL source, not steerable.
void weapon_user_animations_container_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );
}

void weapon_user_animations_container_cook::translate_query( resources::query_result_for_cook& parent )
{
	fs_new::virtual_path_string config_name;
	config_name.assignf( "resources/%s", parent.get_requested_path( ) );

	resources::query_resource(
		config_name.c_str( ),
		resources::binary_config_class_impl,
		boost::bind( &weapon_user_animations_container_cook::on_config_loaded, this, _1 ),
		g_allocator,
		NULL,
		&parent
	);
}

void create_requests_for_animations(
	configs::binary_config_value const&		cfg,
	const u32								requests_count,
	buffer_vector< resources::request >&	requests
)
{
	ASSERT( UNKNOWN_EXPRESSION_T( requests_count ) );
	for ( u32 i = 0; i < requests_count; ++i )
		requests.push_back( resources::create_request( cfg[i], resources::animation_class ) );
}

void weapon_user_animations_container_cook::on_config_loaded( resources::queries_result& data )
{
	if ( !data.is_successful( ) )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		data.get_parent_query( )->finish_query( result_error );
		return;
	}

	buffer_vector< resources::request > requests( ALLOCA( 468 * sizeof( resources::request ) ), 468 );

	configs::binary_config_ptr 			config = static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource( ) );
	configs::binary_config_value const&	root = config->get_root( );

	create_requests_for_animations( root["stand_hud"], 27, requests );
	create_requests_for_animations( root["stand"], 27, requests );
	create_requests_for_animations( root["stand_hands_only_hud"], 6, requests );
	create_requests_for_animations( root["stand_hands_only"], 6, requests );
	create_requests_for_animations( root["aimed_stand_hud"], 27, requests );
	create_requests_for_animations( root["aimed_stand"], 27, requests );
	create_requests_for_animations( root["aimed_stand_hands_only_hud"], 6, requests );
	create_requests_for_animations( root["aimed_stand_hands_only"], 6, requests );
	create_requests_for_animations( root["crouch_hud"], 27, requests );
	create_requests_for_animations( root["crouch"], 27, requests );
	create_requests_for_animations( root["crouch_hands_only_hud"], 6, requests );
	create_requests_for_animations( root["crouch_hands_only"], 6, requests );
	create_requests_for_animations( root["aimed_crouch_hud"], 27, requests );
	create_requests_for_animations( root["aimed_crouch"], 27, requests );
	create_requests_for_animations( root["aimed_crouch_hands_only_hud"], 6, requests );
	create_requests_for_animations( root["aimed_crouch_hands_only"], 6, requests );
	create_requests_for_animations( root["sprint_hud"], 2, requests );
	create_requests_for_animations( root["sprint"], 2, requests );
	create_requests_for_animations( root["jump_hud"], 100, requests );
	create_requests_for_animations( root["jump"], 100, requests );

	ASSERT( UNKNOWN_EXPRESSION );

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &weapon_user_animations_container_cook::on_animations_loaded, this, _1 ),
		g_allocator,
		NULL,
		data.get_parent_query( )
	);
}
template < u32 count >
void get_animations_from_request_results(
	resources::queries_result const&	data,
	const u32							animations_count,
	u32&								resource_index,
	resources::managed_resource_ptr		(&result)[count]
)
{
	ASSERT( UNKNOWN_EXPRESSION );
	for ( u32 i = 0; i != animations_count; ++i )
		result[i] = static_cast_resource_ptr< resources::managed_resource_ptr >( data[resource_index++].get_managed_resource( ) );
}
void weapon_user_animations_container_cook::on_animations_loaded( resources::queries_result& data )
{
	if ( !data.is_successful( ) )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		data.get_parent_query( )->finish_query( result_error );
		return;
	}

	weapon_user_animations_container* container = VOSTOK_NEW_IMPL( g_allocator, weapon_user_animations_container );
	u32 resource_index = 0;

	get_animations_from_request_results( data, 27, resource_index, container->m_stand_animations[0] );
	get_animations_from_request_results( data, 27, resource_index, container->m_stand_animations[1] );
	get_animations_from_request_results( data, 6, resource_index, container->m_stand_hands_only_animations[0] );
	get_animations_from_request_results( data, 6, resource_index, container->m_stand_hands_only_animations[1] );
	get_animations_from_request_results( data, 27, resource_index, container->m_aimed_stand_animations[0] );
	get_animations_from_request_results( data, 27, resource_index, container->m_aimed_stand_animations[1] );
	get_animations_from_request_results( data, 6, resource_index, container->m_aimed_stand_hands_only_animations[0] );
	get_animations_from_request_results( data, 6, resource_index, container->m_aimed_stand_hands_only_animations[1] );
	get_animations_from_request_results( data, 27, resource_index, container->m_crouch_animations[0] );
	get_animations_from_request_results( data, 27, resource_index, container->m_crouch_animations[1] );
	get_animations_from_request_results( data, 6, resource_index, container->m_crouch_hands_only_animations[0] );
	get_animations_from_request_results( data, 6, resource_index, container->m_crouch_hands_only_animations[1] );
	get_animations_from_request_results( data, 27, resource_index, container->m_aimed_crouch_animations[0] );
	get_animations_from_request_results( data, 27, resource_index, container->m_aimed_crouch_animations[1] );
	get_animations_from_request_results( data, 6, resource_index, container->m_aimed_crouch_hands_only_animations[0] );
	get_animations_from_request_results( data, 6, resource_index, container->m_aimed_crouch_hands_only_animations[1] );
	get_animations_from_request_results( data, 2, resource_index, container->m_sprint_animations[0] );
	get_animations_from_request_results( data, 2, resource_index, container->m_sprint_animations[1] );
	get_animations_from_request_results( data, 100, resource_index, container->m_jump_animations[0] );
	get_animations_from_request_results( data, 100, resource_index, container->m_jump_animations[1] );

	ASSERT( UNKNOWN_EXPRESSION );

	data.get_parent_query( )->set_unmanaged_resource(
		resources::unmanaged_resource_ptr( container ),
		resources::memory_usage_type( resources::nocache_memory, sizeof( weapon_user_animations_container ) )
	);
	data.get_parent_query( )->finish_query( result_success );
}

} // namespace survarium
