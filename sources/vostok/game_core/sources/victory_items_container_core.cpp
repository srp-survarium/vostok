////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/victory_items_container_core.h>

#include <vostok/game_core/collision_user.h>

namespace survarium {

// STATE[UNCHECKED]
victory_items_container_core::victory_items_container_core( ) :
	m_victory_items		( g_allocator ),
	m_owner_team		( team_undefined )
{
	// FUNCTION BODY
	// <0x59dee0>|0x000|+0x06f:'21'	{
	// <0>
	// <0x59df4f>|0x06f|      :'23'	}
	// ******
}

// STATE[UNCHECKED]
void victory_items_container_core::load( configs::binary_config_value const& cfg )
{
	usable_object::load( cfg );
	m_owner_team	= (game_team_id)(u32)cfg["team"];
	m_container_id	= (u8)cfg["id"];

	// FUNCTION BODY
	// <0x59df69>|0x009|+0x00c:'27'
	// <0x59df75>|0x015|+0x01b:'28'
	// <0x59df90>|0x030|+0x018:'29'
	// ******
}

// STATE[UNCHECKED]
bool victory_items_container_core::use_initialize( usable_object_user_data* user )
{
	if ( !m_usable_object_users.empty( ) )
		return false;

	user->owner->use_victory_items_container( this );
	return true;

	// FUNCTION BODY
	// <0x59dde7>|0x007|+0x013:'34'
	// <0x59ddfa>|0x01a|+0x004:'35'
	// <0>
	// <0x59ddfe>|0x01e|+0x015:'37'
	// <0>
	// <0x59de13>|0x033|+0x002:'39'
	// ******
}

// STATE[UNCHECKED]
pcstr victory_items_container_core::use_info( usable_object_user_data* __formal )
{
	return "";

	// FUNCTION BODY
	// <0x59ddc7>|0x007|+0x005:'44'
	// ******
}

// STATE[UNCHECKED]
bool victory_items_container_core::use_execute( usable_object_user_data* user )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	return true;

	// FUNCTION BODY
	// <0x59de89>|0x009|+0x00c:'49'
	// <0x59de95>|0x015|+0x00c:'50'
	// <0>
	// <1>
	// <0x59dea1>|0x021|+0x002:'53'
	// ******
}

// STATE[UNCHECKED]
bool victory_items_container_core::use_finalize( usable_object_user_data* __formal )
{
	return true;

	// FUNCTION BODY
	// <0x59ddb7>|0x007|+0x002:'58'
	// ******
}

// STATE[UNCHECKED]
void victory_items_container_core::put_item( victory_item_core* item )
{
	m_victory_items.push_back( item );
	// FUNCTION BODY
	// <0x59deb9>|0x009|+0x01b:'63'
	// ******
}

// STATE[UNCHECKED]
victory_item_core* victory_items_container_core::take_item( )
{
	victory_item_core* last_item = m_victory_items.back( );
	m_victory_items.pop_back( );
	return last_item;

	// FUNCTION BODY
	// <0x59de29>|0x009|+0x022:'67'
	// <0x59de4b>|0x02b|+0x027:'68'
	// <0x59de72>|0x052|+0x003:'69'
	// ******
}

} // namespace survarium
