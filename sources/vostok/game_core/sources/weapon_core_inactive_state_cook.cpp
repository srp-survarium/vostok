// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/weapon_core_inactive_state_cook.h>

#include "weapon_core_inactive_state.h"
#include <vostok/game_core/weapon_state_creation_params.h>
#include "game_core_memory.h"

namespace survarium {

weapon_core_inactive_state_cook::weapon_core_inactive_state_cook( ) :
	resources::unmanaged_cook( resources::weapon_inactive_state_class, reuse_false, use_current_thread_id, use_current_thread_id )
{
	resources::register_cook( this );
}

weapon_core_inactive_state_cook::~weapon_core_inactive_state_cook( )
{
}

mutable_buffer weapon_core_inactive_state_cook::allocate_resource( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist )
{
	VOSTOK_UNREFERENCED_PARAMETERS( &in_query, &raw_file_data, file_exist );
	return mutable_buffer( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( weapon_core_inactive_state ), "weapon_core_inactive_state" ), sizeof( weapon_core_inactive_state ) );
}

void weapon_core_inactive_state_cook::deallocate_resource( void* buffer )
{
	VOSTOK_FREE_IMPL( g_allocator, (resources::resource_base*&)buffer );
}

void weapon_core_inactive_state_cook::create_resource( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer )
{
	weapon_state_creation_params const*	params	= static_cast< weapon_state_creation_params const* >( raw_file_data.c_ptr( ) );

	weapon_core_inactive_state*	object_to_cook	= new ( in_out_unmanaged_resource_buffer.c_ptr( ) ) weapon_core_inactive_state( params->weapon );

	parent.set_unmanaged_resource(
		resources::unmanaged_resource_ptr( object_to_cook ),
		resources::memory_usage_type( resources::nocache_memory, sizeof( weapon_core_inactive_state ) )
	);
	parent.finish_query( resources::cook_base::result_success );
}

void weapon_core_inactive_state_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	weapon_core_inactive_state*	wpn_state	= static_cast< weapon_core_inactive_state* >( resource );
	wpn_state->~weapon_core_inactive_state( );
}

} // namespace survarium
