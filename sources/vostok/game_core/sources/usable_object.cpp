////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/usable_object.h>

#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/base_project.h>

namespace survarium {

// STATE[84.24%|PARTIAL]: link_resolver base-subobject ctor inline-vs-call (LTCG), shape matches
// STRUCTURE DIFF:
// target: 0x590960            base: 0x549a90
// ; survarium::usable_object::usable_object() ; target 0 stmts / base 0 stmts
// ; aligned 0, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - 0-stmt member-init; residual is the link_resolver base-subobject init (target inlines the +4 store, base calls it out-of-line), decided by the base class decl, non-steerable. trail: usable_object.md
usable_object::usable_object( ) :
	m_collision_geometries		( NULL ),
	m_collision_geometries_count( 0 )
{
}

// STATE[100%|DONE]
usable_object::~usable_object( )
{
	VOSTOK_FREE_IMPL( g_allocator, m_collision_geometries );
}

// STATE[100%|DONE]
void usable_object::load( configs::binary_config_value const& cfg )
{
	configs::binary_config_value collision_table = cfg["collision_geometries"];
	m_collision_geometries_count = collision_table.size( );
	ASSERT( UNKNOWN_EXPRESSION ); // sushi@TODO: Compare with other `load` functions.

	m_collision_geometries = (collision_geometry**)VOSTOK_MALLOC_IMPL( g_allocator, sizeof( collision_geometry* ) * m_collision_geometries_count, "usable_object_collision_geometry" );
}

// STATE[99.9%|DONE]: only [ebp-N] slot-numbering noise remains, structure matches
// STRUCTURE DIFF:
// target: 0x590a40            base: 0x549b30
// ; void survarium::usable_object::resolve_links(survarium::base_project*, vostok::configs::binary_config_value) ; target 5 stmts / base 5 stmts
// ; aligned 5, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH - 5/5 stmts byte-aligned; sole residual is [ebp-38h/3Ch/24h] slot-number assignment (allocation noise), non-steerable. trail: usable_object.md
void usable_object::resolve_links( base_project* p, configs::binary_config_value cfg )
{
	configs::binary_config_value collision_table = cfg["collision_geometries"];
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i ) // sushi@TODO: Disasm says it is `i < m_collision_geometries`. Why?
	{
		pcstr geom_name = collision_table[i]["name"];
		m_collision_geometries[i] = static_cast<collision_geometry*>( p->get_object_by_name( geom_name ) );
	}
}

// STATE[100%|DONE]
void usable_object::insert( physics::world* world )
{
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->subscribe( world, this );
}

// STATE[100%|DONE]
void usable_object::remove( )
{
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->unsubscribe( this );
}

// STATE[BLOCKED] Called from `survarium::game_world_ui::update_minimap_objects`
float4x4 usable_object::get_transform( )
{
	return m_collision_geometries[0]->get_transform( );

	// FUNCTION BODY
	// <0x5a08e7>|0x007|+0x01e:'58'
	// ******
}

} // namespace survarium
