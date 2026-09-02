// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/hittable_object.h>

#include <vostok/physics/static_rigid_body.h>
#include <vostok/physics/world.h>

namespace survarium {

// claude@NOTE: STRUCTURE MATCH. Residual is the base-ctor call-vs-inline wall: target CALLs
// survarium::hit_receiver::hit_receiver (its user-declared inline ctor survives as an
// ICF-folded out-of-line COMDAT - 3 copies in the target index, ZERO in our base), so our
// build inlines hit_receiver's body (game_object vtable store + loose_ptr_base ctor call) and
// uses a different frame. Decided by whether the implicit/inline base ctor is emitted as a
// COMDAT globally - not steerable from this TU. See patterns/base-ctor-call-vs-inlined-init.md.
hittable_object::hittable_object( ) :
	m_rigid_body	( NULL ),
	m_physics_world	( NULL ),
	m_group			( 0 ),
	m_mask			( 0 )
{
}

// claude@NOTE: STRUCTURE MATCH. Same wall as the ctor, on the dtor side: target CALLs
// survarium::hit_receiver::~hit_receiver (out-of-line in target, absent from our base), so our
// build inlines the base dtor (hit_receiver vtable store + loose_ptr_base::~loose_ptr_base)
// and grows the frame to 0x38 (vs target 0x0C). Not steerable from this TU.
hittable_object::~hittable_object( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	physics::destroy_static_rigid_body( m_rigid_body );
}

void hittable_object::load( configs::binary_config_value const& cfg_val )
{
	ASSERT( UNKNOWN_EXPRESSION_T( cfg_val.value_exists( "full_name" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg_val.value_exists( "scale" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg_val.value_exists( "rotation" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg_val.value_exists( "position" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg_val.value_exists( "meshes" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg_val.value_exists( "filter_group" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg_val.value_exists( "filter_mask" ) ) );

	pcstr const 	name		= (pcstr)cfg_val["full_name"];
	float3 const& 	scale		= cfg_val["scale"];
	float3 const& 	rotation	= cfg_val["rotation"];
	float3 const& 	position	= cfg_val["position"];
	float4x4 transform = math::create_scale( scale ) * math::create_rotation( rotation ) * math::create_translation( position ); // sush@MATCH. Specifically here

	configs::binary_config_value meshes = cfg_val["meshes"];
	physics::bt_collision_shape_ptr shape = physics::create_compound_shape( meshes, float3( 1.0f, 1.0f, 1.0f ), name ); // sushi@TODO: model_path is unused
	shape->set_no_delete( );	// sushi@TODO: Why

	physics::bt_rigid_body_construction_info info;
	info.m_collisionShape = shape;

	m_rigid_body = physics::create_static_rigid_body( info );

	m_group = (u16)cfg_val["filter_group"];
	m_mask  = (u16)cfg_val["filter_mask"];

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
}

void hittable_object::set_transform( float4x4 const& transform )
{
	m_rigid_body->set_transform( transform );
}

float4x4 hittable_object::get_transform( )
{
	return m_rigid_body->get_transform( );
}

void hittable_object::insert( physics::world* world )
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_physics_world ));
	ASSERT( UNKNOWN_EXPRESSION );

	m_physics_world = world;
	m_rigid_body->user_data = this;
	world->add( m_rigid_body, m_group, m_mask );
}

void hittable_object::remove( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_physics_world ));
	ASSERT( UNKNOWN_EXPRESSION );

	m_physics_world->remove( m_rigid_body );
	m_physics_world = NULL;
}

} // namespace survarium
