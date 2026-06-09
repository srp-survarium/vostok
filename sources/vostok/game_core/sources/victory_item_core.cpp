////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/victory_item_core.h>

#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/collision_user.h>

namespace survarium {

// STATE[94.12%|DONE]: LTCG for unmanaged_resource base ctor, non-steerable. trail: victory_item_core.md
// STRUCTURE DIFF[target 0x58c450 | base 0x4495e0]: target 1 / base 1 stmts
// .. same ..
// ; aligned 1, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - init-list + identity() aligns; residual is the unmanaged_resource base ctor inline-vs-call (whole-program LTCG), non-steerable.
victory_item_core::victory_item_core( ) :
	id				( u8(-1) ),
	m_is_inserted	( false ),
	m_spoted_to_team( team_undefined ),
	m_carrier_id	( u8(-1) )
{
	m_transform.identity( );
}

// STATE[74.33%|PARTIAL]: collision_geometry dtor inline-vs-call LTCG, non-steerable. trail: victory_item_core.md
// STRUCTURE DIFF[target 0x58c3f0 | base 0x449580]: target 1 / base 1 stmts
// 0x027 <0x19> | 0x026 <0x1a> | VOSTOK_DELETE_IMPL( g_allocator, m_collision_geometries[0] );   SIZE
// ; aligned 0, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE on VOSTOK_DELETE_IMPL; the collision_geometry dtor it inlines differs by one byte (whole-program inline form), non-steerable.
victory_item_core::~victory_item_core( )
{
	VOSTOK_DELETE_IMPL( g_allocator, m_collision_geometries[0] ); // sushi@TODO: Why is this called from this destructor and not the one in `usabe_object`
}

// STATE[100%|DONE]
void victory_item_core::unload( )
{
	usable_object::remove( );
}

// STATE[85.05%|PARTIAL]: VOSTOK_NEW / config-value op inline-vs-call LTCG, non-steerable. trail: victory_item_core.md
// STRUCTURE DIFF[target 0x58c4e0 | base 0x449670]: target 4 / base 4 stmts
// 0x015 <0x47> | 0x015 <0x56> | *m_collision_geometries = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );   SIZE
// 0x05c <0x24> | 0x06b <0x26> | m_collision_geometries[0]->load( cfg["collision_geometries"][0] );   SIZE
// ; aligned 2, size-diffs 2, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - 2 SIZE: VOSTOK_NEW collision_geometry allocation + the cfg[...] config-value index op inline form, whole-program LTCG, non-steerable.
void victory_item_core::load( configs::binary_config_value const& cfg )
{
	usable_object::load( cfg );

	*m_collision_geometries = VOSTOK_NEW_IMPL( g_allocator, collision_geometry );
	m_collision_geometries[0]->load( cfg["collision_geometries"][0] );
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
