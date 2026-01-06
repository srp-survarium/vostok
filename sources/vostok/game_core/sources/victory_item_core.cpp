////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/victory_item_core.h>

#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/collision_user.h>

namespace survarium {

// STATE[94.12%|DONE]: LTCG for unmanaged resource
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

// STATE[74.33%|PARTIAL]
victory_item_core::~victory_item_core( )
{
	VOSTOK_DELETE_IMPL( g_allocator, m_collision_geometries[0] ); // sushi@TODO: Why is this called from this destructor and not the one in `usabe_object`

	// FUNCTION BODY
	// <0x59c417>|0x027|+0x019:'26'
	// ******
}

// STATE[100%|DONE]
void victory_item_core::unload( )
{
	usable_object::remove( );
}

// STATE[80.17%|PARTIAL]: LTCG for load.
void victory_item_core::load( configs::binary_config_value const& cfg )
{
	usable_object::load( cfg );

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

// STATE[100%|DONE]
bool victory_item_core::use_initialize( usable_object_user_data* user )
{
	if ( !m_usable_object_users.empty( ) )
		return false;

	user->owner->use_victory_item( this );

	return true;
}

// STATE[100%|DONE]
bool victory_item_core::use_execute( usable_object_user_data* user )
{
	ASSERT( user ); VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;

	// FUNCTION BODY
	// <0x59c310>|0x000|+0x009:'53'	{
	// <0x59c319>|0x009|+0x00c:'54'
	// <0x59c325>|0x015|+0x002:'55'
	// <0x59c327>|0x017|      :'56'	}
	// ******
}

// STATE[100%|DONE]
bool victory_item_core::use_finalize( usable_object_user_data* user )
{
	ASSERT( user ); VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;

	// FUNCTION BODY
	// <0x59c2f0>|0x000|+0x009:'59'	{
	// <0x59c2f9>|0x009|+0x00c:'60'
	// <0x59c305>|0x015|+0x002:'61'
	// <0x59c307>|0x017|      :'62'	}
	// ******
}

// STATE[100%|DONE]
void victory_item_core::put( physics::world* world, float4x4 const& transform )
{
	insert( world );
	set_transform( transform );
	m_is_inserted = true;
}

// STATE[100%|DONE]
void victory_item_core::take( )
{
	remove( );
	m_is_inserted = false;
}

// STATE[100%|DONE]
void victory_item_core::set_transform( float4x4 const& transform )
{
	m_transform = transform;
	m_collision_geometries[0]->set_transform( transform );
}

// STATE[100%|DONE]
float4x4 victory_item_core::get_transform( )
{
	return m_transform;
}

} // namespace survarium
