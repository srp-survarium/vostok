// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/victory_items_container_core.h>

#include <vostok/game_core/collision_user.h>

namespace survarium {

victory_items_container_core::victory_items_container_core( ) :
	m_victory_items		( g_allocator ),
	m_owner_team		( team_undefined )
{
}

void victory_items_container_core::load( configs::binary_config_value const& cfg )
{
	usable_object::load( cfg );
	m_owner_team	= (game_team_id)(u8)cfg["team"];
	m_container_id	= (u8)cfg["id"];
}

bool victory_items_container_core::use_initialize( usable_object_user_data* user )
{
	if ( !m_usable_object_users.empty( ) )
		return false;

	user->owner->use_victory_items_container( this );
	return true;
}

pcstr victory_items_container_core::use_info( usable_object_user_data* __formal )
{
	return "";
}

bool victory_items_container_core::use_execute( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION_T( user ) );
	ASSERT( UNKNOWN_EXPRESSION_T( user->owner ) );

	return true;
}

bool victory_items_container_core::use_finalize( usable_object_user_data* __formal )
{
	return true;
}

void victory_items_container_core::put_item( victory_item_core* item )
{
	m_victory_items.push_back( item );
}

victory_item_core* victory_items_container_core::take_item( )
{
	victory_item_core* last_item = m_victory_items.back( );
	m_victory_items.pop_back( );
	return last_item;
}

} // namespace survarium
