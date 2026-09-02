// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/victory_item_core.h>

#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/collision_user.h>

namespace survarium {

victory_item_core::victory_item_core( ) :
	id				( u8(-1) ),
	m_is_inserted	( false ),
	m_spoted_to_team( team_undefined ),
	m_carrier_id	( u8(-1) )
{
	m_transform.identity( );
}

victory_item_core::~victory_item_core( )
{
	VOSTOK_DELETE_IMPL( g_allocator, m_collision_geometries[0] ); // sushi@TODO: Why is this called from this destructor and not the one in `usabe_object`
}

void victory_item_core::unload( )
{
	usable_object::remove( );
}

void victory_item_core::load( configs::binary_config_value const& cfg )
{
	usable_object::load( cfg );

	*m_collision_geometries = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );
	m_collision_geometries[0]->load( cfg["collision_geometries"][0] );
}

bool victory_item_core::use_initialize( usable_object_user_data* user )
{
	if ( !m_usable_object_users.empty( ) )
		return false;

	user->owner->use_victory_item( this );

	return true;
}

bool victory_item_core::use_execute( usable_object_user_data* user )
{
	ASSERT( user ); VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;
}

bool victory_item_core::use_finalize( usable_object_user_data* user )
{
	ASSERT( user ); VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;
}

void victory_item_core::put( physics::world* world, float4x4 const& transform )
{
	usable_object::insert( world );
	set_transform( transform );
	m_is_inserted = true;
}

void victory_item_core::take( )
{
	usable_object::remove( );
	m_is_inserted = false;
}

void victory_item_core::set_transform( float4x4 const& transform )
{
	m_transform = transform;
	m_collision_geometries[0]->set_transform( transform );
}

float4x4 victory_item_core::get_transform( )
{
	return m_transform;
}

} // namespace survarium
