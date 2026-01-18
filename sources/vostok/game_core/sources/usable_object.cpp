////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/usable_object.h>

#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/base_project.h>

namespace survarium {

// STATE[84.24%|PARTIAL]: Problems with `link_resolver` constructor
usable_object::usable_object( ) :
	m_collision_geometries		( NULL ),
	m_collision_geometries_count( 0 )
{
}

// STATE[100%|DONE]
usable_object::~usable_object( )
{
	VOSTOK_FREE_IMPL( g_allocator, m_collision_geometries );

	// FUNCTION BODY
	// <0x5a0b1c>|0x01c|+0x03d:'22'
	// ******
}

// STATE[94.15%|DONE]: LTCG for `binary_config`.
void usable_object::load( configs::binary_config_value const& cfg )
{
	configs::binary_config_value collision_table = cfg["collision_geometries"];
	m_collision_geometries_count = collision_table.size( );
	ASSERT( UNKNOWN_EXPRESSION ); // sushi@TODO: Compare with other `load` functions.

	m_collision_geometries = (collision_geometry**)VOSTOK_MALLOC_IMPL( g_allocator, sizeof( collision_geometry* ) * m_collision_geometries_count, "usable_object_collision_geometry" );

	// FUNCTION BODY
	// <0x5a0b8a>|0x00a|+0x030:'27'
	// <0x5a0bba>|0x03a|+0x00e:'28'
	// <0x5a0bc8>|0x048|+0x00c:'29'
	// <0>
	// <0x5a0bd4>|0x054|+0x022:'31'
	// ******
}

// STATE[98.15%|DONE]
void usable_object::resolve_links( base_project* p, configs::binary_config_value cfg )
{
	configs::binary_config_value collision_table = cfg["collision_geometries"];
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i ) // sushi@TODO: Disasm says it is `i < m_collision_geometries`. Why?
	{
		pcstr geom_name = collision_table[i]["name"];
		m_collision_geometries[i] = static_cast<collision_geometry*>( p->get_object_by_name( geom_name ) );
	}

	// FUNCTION BODY
	// <0x5a0a49>|0x009|+0x030:'36'
	// <0x5a0a79>|0x039|+0x01d|[1]:'37'
	// <0>
	// <0x5a0a96>|0x056|+0x01f|[2]:'39'
	// <0x5a0ab5>|0x075|+0x03d:'40'
	// <0>
	// ******
}

// STATE[100%|DONE]
void usable_object::insert( physics::world* world )
{
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->subscribe( world, this );

	// FUNCTION BODY
	// <0x5a09f9>|0x009|+0x01d|[1]:'46'
	// <0x5a0a16>|0x026|+0x01b:'47'
	// ******
}

// STATE[100%|DONE]
void usable_object::remove( )
{
	for ( u32 i = 0 ; i < m_collision_geometries_count ; ++i )
		m_collision_geometries[i]->unsubscribe( this );

	// FUNCTION BODY
	// <0x5a0919>|0x009|+0x01d|[1]:'52'
	// <0x5a0936>|0x026|+0x017:'53'
	// ******
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
