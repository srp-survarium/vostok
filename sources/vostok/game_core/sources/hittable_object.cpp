////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/hittable_object.h>

#include <vostok/physics/static_rigid_body.h>
#include <vostok/physics/world.h>

namespace survarium {

// STATE[72.88%|PARTIAL]: hit_receiver sub-object ctor inlined in base, out-of-line in target; non-steerable
hittable_object::hittable_object( ) :
	m_rigid_body	( NULL ),
	m_physics_world	( NULL ),
	m_group			( 0 ),
	m_mask			( 0 )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (0x49 vs 0x5e, init-list only)
	// VERDICT: STRUCTURE MATCH (shape ok) - target calls the hit_receiver base ctor out-of-line (a header-COMDAT in the target's hit_receiver.h unit) while base inlines it; whole-program LTCG, non-steerable.
}

// STATE[69.59%|PARTIAL]: hit_receiver sub-object dtor inlined in base, out-of-line in target; non-steerable
hittable_object::~hittable_object( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	physics::destroy_static_rigid_body( m_rigid_body );

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts (0x43 vs 0x58) - no diverging rows
	// VERDICT: STRUCTURE MATCH (shape ok) - target calls the hit_receiver base dtor out-of-line (header-COMDAT) while base inlines it; whole-program LTCG, non-steerable.
}

// STATE[98.14%|PARTIAL]: resource_ptr / shape-op inline-vs-call LTCG, non-steerable
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

	// STRUCTURE DIFF: target 23 stmts / base 23 stmts
	// SIZE +0x1 | 62 | physics::bt_collision_shape_ptr shape = physics::create_compound_shape( meshes, float3( 1.0f, 1.0f, 1.0f ), name );
	// SIZE -0x2 | 63 | shape->set_no_delete( );
	// SIZE -0x1 | 66 | info.m_collisionShape = shape;
	// VERDICT: STRUCTURE MATCH (shape ok) - 3 tiny SIZE on bt_collision_shape_ptr construct/assign + set_no_delete; whole-program inline-vs-call of the ptr ops, non-steerable.
}

// STATE[100%|DONE]
void hittable_object::set_transform( float4x4 const& transform )
{
	m_rigid_body->set_transform( transform );
}

// STATE[MISSING]
float4x4 hittable_object::get_transform( )
{
	return m_rigid_body->get_transform( );
}

// STATE[100%|DONE]
void hittable_object::insert( physics::world* world )
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_physics_world ));
	ASSERT( UNKNOWN_EXPRESSION );

	m_physics_world = world;
	m_rigid_body->user_data = this;
	world->add( m_rigid_body, m_group, m_mask );
}

// STATE[100%|DONE]
void hittable_object::remove( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_physics_world ));
	ASSERT( UNKNOWN_EXPRESSION );

	m_physics_world->remove( m_rigid_body );
	m_physics_world = NULL;
}

} // namespace survarium