////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/usable_object.h>

#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/base_project.h>

namespace survarium {

usable_object::usable_object( ) :
	m_collision_geometries		( NULL ),
	m_collision_geometries_count( 0 )
{
}

usable_object::~usable_object( )
{
	VOSTOK_FREE_IMPL( g_allocator, m_collision_geometries );
}

void usable_object::load( configs::binary_config_value const& cfg )
{
	configs::binary_config_value collision_table = cfg["collision_geometries"];
	m_collision_geometries_count = collision_table.size( );
	ASSERT( UNKNOWN_EXPRESSION ); // sushi@TODO: Compare with other `load` functions.

	m_collision_geometries = (collision_geometry**)VOSTOK_MALLOC_IMPL( g_allocator, sizeof( collision_geometry* ) * m_collision_geometries_count, "usable_object_collision_geometry" );
}

void usable_object::resolve_links( base_project* p, configs::binary_config_value cfg )
{
	configs::binary_config_value collision_table = cfg["collision_geometries"];
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i ) // sushi@TODO: Disasm says it is `i < m_collision_geometries`. Why?
	{
		pcstr geom_name = collision_table[i]["name"];
		m_collision_geometries[i] = static_cast<collision_geometry*>( p->get_object_by_name( geom_name ) );
	}
}

void usable_object::insert( physics::world* world )
{
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->subscribe( world, this );
}

void usable_object::remove( )
{
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->unsubscribe( this );
}

float4x4 usable_object::get_transform( )
{
	return m_collision_geometries[0]->get_transform( );
}

} // namespace survarium
