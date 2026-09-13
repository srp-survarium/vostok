// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "shotgun_weapon_reload_state_cook.h"
#include "shotgun_weapon_reload_state.h"
#include <vostok/game_core/weapon_state_creation_params.h>

namespace survarium {

shotgun_weapon_reload_state_cook::shotgun_weapon_reload_state_cook( ) :
	resources::unmanaged_cook( resources::weapon_shotgun_reload_state_class, reuse_false, use_current_thread_id, use_current_thread_id )
{
	resources::register_cook( this );
}

shotgun_weapon_reload_state_cook::~shotgun_weapon_reload_state_cook( )
{
}

mutable_buffer shotgun_weapon_reload_state_cook::allocate_resource(
	resources::query_result_for_cook&		in_query,
	const_buffer							raw_file_data,
	bool									file_exist
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( in_query, raw_file_data, file_exist );
	return mutable_buffer( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( shotgun_weapon_reload_state ), "shotgun_weapon_reload_state" ), sizeof( shotgun_weapon_reload_state ) );
}

void shotgun_weapon_reload_state_cook::deallocate_resource( void* buffer )
{
	VOSTOK_FREE_IMPL( g_allocator, (resources::resource_base*&)buffer );
}

void shotgun_weapon_reload_state_cook::create_resource(
	resources::query_result_for_cook&		parent,
	const_buffer							raw_file_data,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
	weapon_state_creation_params const*	params	= static_cast< weapon_state_creation_params const* >( raw_file_data.c_ptr( ) );
	configs::binary_config_value		cfg;
	if ( !parent.user_data( )->try_get( cfg ) )
	{
		DEBUG_BREAK		( );
		parent.finish_query( result_error );
		return;
	}

	resources::creation_request requests[3] = {
		resources::creation_request( "start_substate",		raw_file_data, resources::weapon_shotgun_reload_start_substate_class ),
		resources::creation_request( "reload_one_substate",	raw_file_data, resources::weapon_shotgun_reload_one_substate_class ),
		resources::creation_request( "finish_substate",		raw_file_data, resources::weapon_shotgun_reload_finish_substate_class ),
	};

	variant< 32 > user_data[3];
	for ( u32 i = 0; i != array_size( requests ); ++i )
		user_data[i].set( cfg[requests[i].get_name( )] );

	variant< 32 > const* user_data_ptrs[3] = { &user_data[0], &user_data[1], &user_data[2] };

	resources::query_create_resources(
		requests,
		boost::bind( &shotgun_weapon_reload_state_cook::on_substates_ready, this, _1, in_out_unmanaged_resource_buffer, params ),
		g_allocator,
		user_data_ptrs,
		&parent
	);
	parent.finish_query( result_postponed );
}

void shotgun_weapon_reload_state_cook::on_substates_ready(
	resources::queries_result&				data,
	mutable_buffer							buffer,
	weapon_state_creation_params const*		params
)
{
	weapon_core_shotgun_reload_base_substate_ptr start_substate		= static_cast_resource_ptr< weapon_core_shotgun_reload_base_substate_ptr >( data[0].get_unmanaged_resource( ) );
	weapon_core_shotgun_reload_base_substate_ptr reload_one_substate	= static_cast_resource_ptr< weapon_core_shotgun_reload_base_substate_ptr >( data[1].get_unmanaged_resource( ) );
	weapon_core_shotgun_reload_base_substate_ptr finish_substate	= static_cast_resource_ptr< weapon_core_shotgun_reload_base_substate_ptr >( data[2].get_unmanaged_resource( ) );

	shotgun_weapon_reload_state* object_to_cook = new ( buffer.c_ptr( ) ) shotgun_weapon_reload_state( params->weapon, start_substate, reload_one_substate, finish_substate );

	data.get_parent_query( )->set_unmanaged_resource( object_to_cook, resources::memory_usage_type( resources::nocache_memory, sizeof( *this ) ) );
	data.get_parent_query( )->finish_query( result_success );
}

void shotgun_weapon_reload_state_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	shotgun_weapon_reload_state* wpn_state = static_cast< shotgun_weapon_reload_state* >( resource );
	wpn_state->~shotgun_weapon_reload_state( );
}

} // namespace survarium
