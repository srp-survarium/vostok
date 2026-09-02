// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/collision_geometry.h>

#include <vostok/physics/ghost_object.h>

namespace survarium {

collision_geometry::collision_geometry( ) :
	m_physics_world		( NULL ),
	m_ghost_object		( NULL ),
	m_group				( 0 ),
	m_mask				( 0 )
{
}
// claude@NOTE: frame-reservation wall; structure, locals, instructions, and size match.
collision_geometry::~collision_geometry( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	destroy_ghost_object( );
}
// claude@NOTE: target passes the physics helper argument in EAX; base uses the stack.
void collision_geometry::destroy_ghost_object( )
{
	physics::destroy_ghost_object( m_ghost_object );
}

// claude@NOTE: inline-context wall: target keeps fixed_string's config assignment
// and the collision-shape resource_ptr constructor out of line; base inlines both.

void collision_geometry::load( configs::binary_config_value const& cfg_val )
{
	m_name					= cfg_val["full_name"];
	float3 const& scale		= cfg_val["scale"];			// sushi@NOTE: Can reuse `read_transform` in base_project.cpp
	float3 const& rotation	= cfg_val["rotation"];
	float3 const& position	= cfg_val["position"];
	float4x4 transform = create_scale( scale ) * create_rotation( rotation ) * create_translation( position );

	if ( !cfg_val.value_exists( "meshes" ) )
	{
		LOG_WARNING( "invalid collision_geometry" );
		return;
	}

	m_mode									= (collision_geometry::collision_geometry_mode)(u32)cfg_val["mode"];
	configs::binary_config_value meshes		= cfg_val["meshes"];
	physics::bt_collision_shape_ptr shape	= physics::create_compound_shape( meshes, float3( 1.0f, 1.0f, 1.0f ), m_name.c_str( ) );
	m_ghost_object							= physics::create_ghost_object( shape, transform );
	shape->set_no_delete( );	// sushi@TODO: `unmanaged_intrusive_base::destroy` will not delete this resource. Understand for what reasons this was done.

	m_group									= cfg_val["filter_group"];
	m_mask									= cfg_val["filter_mask"];
}

u32 collision_geometry::get_overlapping_objects_count( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	return m_ghost_object->get_overlapping_objects_count( );
}

void collision_geometry::get_overlapping_objects(physics::base_physics_objects_type& result ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->get_overlapping_objects( result );
}

void collision_geometry::contact_test( physics::base_physics_object* object, physics::contact_test_predicate& predicate )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->contact_test( m_physics_world, object, predicate );
}

bool collision_geometry::contact_test( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	return m_ghost_object->contact_test( m_physics_world );
}

void collision_geometry::get_shapes_centers( vectora<float3>& centers_results ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->non_compound_shapes_centers( centers_results );
}

void collision_geometry::insert( physics::world* world )
{
	m_physics_world = world;
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->user_data = this;
	m_ghost_object->insert( m_physics_world, m_group, m_mask );
}

void collision_geometry::remove( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );	// sushi@NOTE: Might also be `m_physics_world` check.
	m_ghost_object->remove( m_physics_world );			// sushi@NOTE: `user_data` was set in `insert`, but is not unset in `remove`.
	m_physics_world = NULL;
}

void collision_geometry::subscribe( physics::world* world, collision_geometry_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );

	if ( m_subscribers.empty( ) )
		insert( world );

	m_subscribers.push_back( subscriber );
}

void collision_geometry::unsubscribe( collision_geometry_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_subscribers.erase( std::find( m_subscribers.begin( ), m_subscribers.end( ), subscriber ) );

	if ( m_subscribers.empty( ) )
		remove( );
}

void collision_geometry::set_transform( float4x4 const& transform )
{
	m_ghost_object->set_transform( transform );
}

float4x4 collision_geometry::get_transform( )
{
	return m_ghost_object->get_transform( );
}

} // namespace survarium
