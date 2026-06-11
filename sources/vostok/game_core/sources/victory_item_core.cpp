////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/victory_item_core.h>

#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/collision_user.h>

namespace survarium {

// STATE[100%|DONE]
victory_item_core::victory_item_core( ) :
	id				( u8(-1) ),
	m_is_inserted	( false ),
	m_spoted_to_team( team_undefined ),
	m_carrier_id	( u8(-1) )
{
	m_transform.identity( );
}

// STATE[74.33%|PARTIAL]: delete_helper LTCG promoted convention (object in edi), non-steerable.
victory_item_core::~victory_item_core( )
{
	VOSTOK_DELETE_IMPL( g_allocator, m_collision_geometries[0] ); // sushi@TODO: Why is this called from this destructor and not the one in `usabe_object`

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0x1 | 34 | VOSTOK_DELETE_IMPL( g_allocator, m_collision_geometries[0] );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is delete_helper's LTCG-promoted edi-arg convention vs our stack args, non-steerable.
}

// STATE[100%|DONE]
void victory_item_core::unload( )
{
	usable_object::remove( );
}

// STATE[85.05%|PARTIAL]: new_helper inline-vs-promoted-call LTCG, non-steerable.
void victory_item_core::load( configs::binary_config_value const& cfg )
{
	usable_object::load( cfg );

	*m_collision_geometries = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );
	m_collision_geometries[0]->load( cfg["collision_geometries"][0] );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// SIZE +0xf | 53 | *m_collision_geometries = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );
	// SIZE +0x2 | 54 | m_collision_geometries[0]->load( cfg["collision_geometries"][0] );
	// VERDICT: STRUCTURE MATCH (shape ok) - target keeps new_helper out-of-line (LTCG ecx-promoted), base inlines malloc_impl (+0xf); +0x2 is an operator[] result register shuffle. Non-steerable.
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
}

// STATE[100%|DONE]
bool victory_item_core::use_finalize( usable_object_user_data* user )
{
	ASSERT( user ); VOSTOK_UNREFERENCED_PARAMETER( user );
	return true;
}

// STATE[100%|DONE]
void victory_item_core::put( physics::world* world, float4x4 const& transform )
{
	usable_object::insert( world );
	set_transform( transform );
	m_is_inserted = true;
}

// STATE[100%|DONE]
void victory_item_core::take( )
{
	usable_object::remove( );
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
