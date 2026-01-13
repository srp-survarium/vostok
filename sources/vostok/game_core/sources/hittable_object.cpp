////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/hittable_object.h>

#include <vostok/physics/static_rigid_body.h>
#include <vostok/physics/world.h>

namespace survarium {

// STATE[UNCHECKED]
 hittable_object::hittable_object( ) :
	m_rigid_body	( NULL ),
	m_physics_world	( NULL ),
	m_group			( 0 ),
	m_mask			( 0 )
{
	// FUNCTION BODY[0x599f30]: 0
	// <0x599f30>|0x000|+0x041:'19'	{
	// <0x599f71>|0x041|      :'20'	}
	// ******
}

// STATE[69.59%|PARTIAL]: Manual statements matched. Destructor of hit_receiver inlined in base.
 hittable_object::~hittable_object( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	physics::destroy_static_rigid_body( m_rigid_body );

	// FUNCTION BODY[0x599ee0]: 4
	// <0x599ef3>|0x013|+0x00c:'24'
	// <0x599eff>|0x01f|+0x00c:'25'
	// <0>
	// <0x599f0b>|0x02b|+0x00b:'27'
	// ******
}

// STATE[86.74%|PARTIAL]: LTCG for linear math ops.
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

	// FUNCTION BODY[0x599f80]: 30
	// <0x599f91>|0x011|+0x012:'32'
	// <0x599fa3>|0x023|+0x012:'33'
	// <0x599fb5>|0x035|+0x012:'34'
	// <0x599fc7>|0x047|+0x012:'35'
	// <0x599fd9>|0x059|+0x012:'36'
	// <0x599feb>|0x06b|+0x012:'37'
	// <0x599ffd>|0x07d|+0x012:'38'
	// <0>
	// <0x59a00f>|0x08f|+0x015:'40'
	// <0x59a024>|0x0a4|+0x015:'41'
	// <0x59a039>|0x0b9|+0x015:'42'
	// <0x59a04e>|0x0ce|+0x015:'43'
	// <0x59a063>|0x0e3|+0x056:'44'
	// <0>
	// <0x59a0b9>|0x139|+0x030:'46'	configs::binary_config_value meshes = cfg_val["meshes"];
	// <0x59a0e9>|0x169|+0x032:'47'
	// <0x59a11b>|0x19b|+0x00f:'48'
	// <0>
	// <1>
	// <0x59a12a>|0x1aa|+0x008:'51'
	// <0x59a132>|0x1b2|+0x00c:'52'
	// <0>
	// <0x59a13e>|0x1be|+0x011:'54'
	// <0>
	// <0x59a14f>|0x1cf|+0x01c:'56'
	// <0x59a16b>|0x1eb|+0x01c:'57'
	// <0>
	// <0x59a187>|0x207|+0x012:'59'
	// <0x59a199>|0x219|+0x012:'60'
	// <0x59a1ab>|0x22b|+0x012:'61'
	// ******

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