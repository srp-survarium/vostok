// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/base_project.h>
#include <vostok/game_core/static_collision.h> // sushi@TODO: Most likely needs to be merged into `base_project.h`.
#include <vostok/game_core/link_resolver.h>
#include <vostok/physics/world.h>
#include <vostok/physics/static_rigid_body.h>


namespace survarium {

base_project::base_project( ) :
	m_static_collision_objects		( NULL ),
	m_static_collision_objects_count( 0 )
{
}

base_project::~base_project( )
{
	if ( m_static_collision_objects )
		VOSTOK_DELETE_ARRAY_IMPL( g_allocator, m_static_collision_objects );
}

base_game_object* base_project::get_object_by_name( pcstr name )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_objects_registry.find( name ) != m_objects_registry.end( )  ) );
	return m_objects_registry[name];
}

void base_project::resolve_links( )
{
	base_project::resolve_link_object* it = m_objects_to_resolve.begin( );
	base_project::resolve_link_object* end = m_objects_to_resolve.end( );
	for ( ; it != end ; ++it )
		it->object->resolve_links( this, it->config );

	m_objects_registry.clear( );
}

// sushi@NOTE: Used from `survarium::project_cooker_simple::create_game_objects`.
void read_transform( vostok::configs::binary_config_value const& cfg, float4x4& result )
{	// object_visual::load
	float3 const& scale		= cfg["scale"];
	float3 const& rotation	= cfg["rotation"];
	float3 const& position	= cfg["position"];
	result = create_scale( scale ) * create_rotation( rotation ) * create_translation( position );
}

void static_collision::insert( vostok::physics::world* w )
{
	vostok::physics::bt_rigid_body_construction_info info;
	info.m_collisionShape = shape_;
	info.m_mass = 0.0f;

	physics_rigid_body_ = vostok::physics::create_static_rigid_body( info );
	physics_rigid_body_->set_transform( matrix_ );

	w->add( physics_rigid_body_, filter_group_, filter_mask_ );
}

void static_collision::remove( vostok::physics::world* w )
{
	w->remove( physics_rigid_body_ );
	vostok::physics::destroy_static_rigid_body( physics_rigid_body_ );
	physics_rigid_body_ = NULL;
}

} // namespace survarium
