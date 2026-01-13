////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/base_project.h>
#include <vostok/game_core/static_collision.h> // sushi@TODO: Most likely needs to be merged into `base_project.h`.
#include <vostok/game_core/link_resolver.h>
#include <vostok/physics/world.h>
#include <vostok/physics/static_rigid_body.h>


namespace survarium {

// STATE[SKIPPED]
base_project::base_project( ) :
	m_static_collision_objects		( NULL ),
	m_static_collision_objects_count( 0 )
{
}

// STATE[SKIPPED]
base_project::~base_project( )
{
	if ( m_static_collision_objects )
		VOSTOK_DELETE_ARRAY_IMPL( g_allocator, m_static_collision_objects );

	// FUNCTION BODY
	// <0x6ff432>|0x000|0x000:'24'
	// <0x6ff43b>|0x009|0x009:'25'
	// ******
}

// STATE[SKIPPED]
base_game_object* base_project::get_object_by_name( pcstr name )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_objects_registry.find( name ) != m_objects_registry.end( )  ) );
	return m_objects_registry[name];

	// FUNCTION BODY
	// <0x6ff3f9>|0x000|0x000:'30'
	// <0x6ff405>|0x00c|0x00c:'31'
	// ******
}

// STATE[SKIPPED]
void base_project::resolve_links( )
{
	base_project::resolve_link_object* it = m_objects_to_resolve.begin( );
	base_project::resolve_link_object* end = m_objects_to_resolve.end( );
	for ( ; it != end ; ++it )
		it->object->resolve_links( this, it->config );

	m_objects_registry.clear( );

	// FUNCTION BODY
	// <0x6ff2c9>|0x000|0x000:'36'
	// <0x6ff2dd>|0x014|0x014:'37'
	// <0x6ff2f1>|0x028|0x014:'38'
	// <0x6ff304>|0x03b|0x013:'39'
	// <1>
	// <0x6ff348>|0x07f|0x044:'41'
	// ******
}

// STATE[SKIPPED]
// sushi@NOTE: Used from `survarium::project_cooker_simple::create_game_objects`.
void read_transform( vostok::configs::binary_config_value const& cfg, float4x4& result )
{	// object_visual::load
	float3 const& scale		= cfg["scale"];
	float3 const& rotation	= cfg["rotation"];
	float3 const& position	= cfg["position"];
	result = create_scale( scale ) * create_rotation( rotation ) * create_translation( position );

	// FUNCTION BODY
	// <0x6ff21b>|0x000|0x000:'46'
	// <0x6ff230>|0x015|0x015:'47'
	// <0x6ff245>|0x02a|0x015:'48'
	// <0x6ff25a>|0x03f|0x015:'49'
	// ******
}

// STATE[SKIPPED]
void static_collision::insert( vostok::physics::world* w )
{
	vostok::physics::bt_rigid_body_construction_info info;
	info.m_collisionShape = shape_;
	info.m_mass = 0.0f;

	physics_rigid_body_ = vostok::physics::create_static_rigid_body( info );
	physics_rigid_body_->set_transform( matrix_ );

	w->add( physics_rigid_body_, filter_group_, filter_mask_ );

	// FUNCTION BODY
	// <0x6ff499>|0x000|0x000:'109'
	// <0x6ff4a1>|0x008|0x008:'110'
	// <0x6ff4b0>|0x017|0x00f:'111'
	// <1>
	// <0x6ff4bd>|0x024|0x00d:'113'
	// <1>
	// <0x6ff4cb>|0x032|0x00e:'115'
	// <1>
	// <2>
	// <3>
	// <0x6ff4e4>|0x04b|0x019:'119'
	// ******
}

// STATE[SKIPPED]
void static_collision::remove( vostok::physics::world* w )
{
	w->remove( physics_rigid_body_ );
	vostok::physics::destroy_static_rigid_body( physics_rigid_body_ );
	physics_rigid_body_ = NULL;

	// FUNCTION BODY
	// <0x6ff1d8>|0x000|0x000:'124'
	// <0x6ff1ec>|0x014|0x014:'125'
	// <0x6ff1f7>|0x01f|0x00b:'126'
	// ******
}

} // namespace survarium
