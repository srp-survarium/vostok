////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/victory_item_core.h>

#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/collision_user.h>

namespace survarium {

// STATE[UNCHECKED]
victory_item_core::victory_item_core( ) :
	id				( u8(-1) ),
	m_is_inserted	( false ),
	m_spoted_to_team( team_undefined ),
	m_carrier_id	( u8(-1) )
{
	m_transform.identity( );

	// FUNCTION BODY
	// <0x59c450>|0x000|+0x074:'20'	{
	// <0x59c4c4>|0x074|+0x00e:'21'
	// <0x59c4d2>|0x082|      :'22'	}
	// ******
}

// STATE[UNCHECKED]
victory_item_core::~victory_item_core( )
{
	VOSTOK_DELETE_IMPL( g_allocator, m_collision_geometries ); // sushi@TODO: Why is this called from this destructor and not the one in `usabe_object`

	// FUNCTION BODY
	// <0x59c417>|0x027|+0x019:'26'
	// ******
}

// STATE[UNCHECKED]
void victory_item_core::unload( )
{
	remove( );

	// FUNCTION BODY
	// <0x59c390>|0x000|+0x007:'30'	{
	// <0x59c397>|0x007|+0x008:'31'
	// <0x59c39f>|0x00f|      :'32'	}
	// ******
}

// STATE[UNCHECKED]
void victory_item_core::load( configs::binary_config_value const& cfg )
{
	load( cfg );

	*m_collision_geometries = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );
	m_collision_geometries[0]->load( cfg["collision_geometries"][0] );

	// FUNCTION BODY
	// <0x59c4e0>|0x000|+0x009:'35'	{
	// <0x59c4e9>|0x009|+0x00c:'36'
	// <0>
	// <0x59c4f5>|0x015|+0x047:'38'
	// <0x59c53c>|0x05c|+0x024:'39'
	// <0x59c560>|0x080|      :'40'	}
	// ******
}

// STATE[UNCHECKED]
bool victory_item_core::use_initialize( usable_object_user_data* user )
{
	if ( !m_usable_object_users.empty( ) )
		return false;

	user->owner->use_victory_item( this );

	return true;

	// FUNCTION BODY
	// <0x59c330>|0x000|+0x007:'43'	{
	// <0x59c337>|0x007|+0x013:'44'
	// <0x59c34a>|0x01a|+0x004:'45'
	// <0>
	// <0x59c34e>|0x01e|+0x015:'47'
	// <0>
	// <0x59c363>|0x033|+0x002:'49'
	// <0x59c365>|0x035|      :'50'	}
	// ******
}

// STATE[UNCHECKED]
bool victory_item_core::use_execute( usable_object_user_data* user )
{
	VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;

	// FUNCTION BODY
	// <0x59c310>|0x000|+0x009:'53'	{
	// <0x59c319>|0x009|+0x00c:'54'
	// <0x59c325>|0x015|+0x002:'55'
	// <0x59c327>|0x017|      :'56'	}
	// ******
}

// STATE[UNCHECKED]
bool victory_item_core::use_finalize( usable_object_user_data* user )
{
	VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;

	// FUNCTION BODY
	// <0x59c2f0>|0x000|+0x009:'59'	{
	// <0x59c2f9>|0x009|+0x00c:'60'
	// <0x59c305>|0x015|+0x002:'61'
	// <0x59c307>|0x017|      :'62'	}
	// ******
}

// STATE[UNCHECKED]
void victory_item_core::put( physics::world* world, float4x4 const& transform )
{
	insert( world );
	set_transform( transform );
	m_is_inserted = true;

	// FUNCTION BODY
	// <0x59c3b0>|0x000|+0x007:'65'	{
	// <0x59c3b7>|0x007|+0x00c:'66'
	// <0x59c3c3>|0x013|+0x011:'67'
	// <0x59c3d4>|0x024|+0x00a:'68'
	// <0x59c3de>|0x02e|      :'69'	}
	// ******
}

// STATE[UNCHECKED]
void victory_item_core::take( )
{
	remove( );
	m_is_inserted = false;

	// FUNCTION BODY
	// <0x59c370>|0x000|+0x007:'72'	{
	// <0x59c377>|0x007|+0x008:'73'
	// <0x59c37f>|0x00f|+0x00a:'74'
	// <0x59c389>|0x019|      :'75'	}
	// ******
}

// STATE[UNCHECKED]
void victory_item_core::set_transform( float4x4 const& transform )
{
	m_transform = transform;
	m_collision_geometries[0]->set_transform( transform );

	// FUNCTION BODY
	// <0x59c2b0>|0x000|+0x009:'78'	{
	// <0x59c2b9>|0x009|+0x013:'79'
	// <0x59c2cc>|0x01c|+0x01b:'80'
	// <0x59c2e7>|0x037|      :'81'	}
	// ******
}

// STATE[UNCHECKED]
float4x4 victory_item_core::get_transform( )
{
	return m_transform;

	// FUNCTION BODY
	// <0x59c280>|0x000|+0x009:'83'	{
	// <0x59c289>|0x009|+0x016:'84'
	// <0x59c29f>|0x01f|      :'85'	}
	// ******
}

} // namespace survarium
