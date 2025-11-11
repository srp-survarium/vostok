////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/character_controller.h>

#include "bullet_character_controller.h"
#include "bullet_include.h"
#include "bullet_physics_world.h"
#include <vostok/physics/bullet_utils.h>
#include <vostok/physics/world.h>

namespace vostok {
namespace physics {

// STATE[STUB]
bt_character_controller* create_character_controller( vostok::memory::base_allocator& allocator, world* w )
{
	return VOSTOK_NEW_IMPL( allocator, bt_character_controller )( w ); // <0x586866>|0x000|0x000:'23'
}

// STATE[STUB]
bt_character_controller::bt_character_controller( world* w ) :
	m_active ( false )
{
	m_bt_physics_world = static_cast<bullet_physics_world*>( w );	// <0x586854>|0x000|0x000:'29'
}

// STATE[STUB]
bt_character_controller::~bt_character_controller( )
{
	// <1>
	VOSTOK_DELETE_IMPL( allocator( ), m_bt_controller ); // <0x586810>|0x000|0x000:'35'
}

// STATE[STUB]
memory::base_allocator& bt_character_controller::allocator( )
{
	return m_bt_physics_world->allocator( );	// <0x586800>|0x000|0x000:'40'
}

// STATE[STUB]: sushi@NOTE: A lot of whitespace
void bt_character_controller::initialize( )
{
	btPairCachingGhostObject* ghost = VOSTOK_NEW_IMPL( allocator( ), btPairCachingGhostObject );	// <0x5869d8>|0x000|0x000:'51'
	ghost->setCollisionFlags(0x10);																	// <0x5869fd>|0x025|0x025:'54'
	ghost->setFriction(100.0f);
	m_bt_controller =  VOSTOK_NEW_IMPL( allocator( ), bullet_character_controller )(
		ghost,
		float2( 0.9, 1.1 ),
		float2( 0.9, 1.8 ),
		1 << 2,
		1 << 1
	);																								// <0x586a17>|0x03f|0x01a:'61'
}

// STATE[STUB]
void bt_character_controller::activate( float4x4 const& t )
{
	ASSERT( !m_bt_controller->m_collision_world );						// <1> sushi@NOTE: Just my assumption
	m_bt_controller->insert( m_bt_physics_world->get_bt_internal( ) );	// <0x586acd>|0x000|0x000:'68'
	m_bt_controller->set_transform( from_vostok( t ) );					// <0x586adb>|0x00e|0x00e:'69'
}

// STATE[STUB]
void bt_character_controller::deactivate( )
{
	ASSERT( m_bt_controller->m_collision_world );					// <1> sushi@NOTE: Just my assumption
	m_bt_controller->remove( m_bt_controller->m_collision_world );	// <0x586930>|0x000|0x000:'75'
}

// STATE[STUB]
float4x4 bt_character_controller::get_transform( )
{
	return from_bullet( m_bt_controller->get_transform( ) );	// <0x5869a6>|0x000|0x000:'80'
}

// STATE[STUB]
void bt_character_controller::set_transform( float4x4 const& transform )
{
	m_bt_controller->set_transform( from_vostok( transform ) );	// <0x586a9a>|0x000|0x000:'85'
}

// STATE[STUB]
void bt_character_controller::set_walk_direction( float3 const& direction )
{
	m_bt_controller->set_desired_walk_vector( from_vostok ( direction ) );	// <0x586959>|0x000|0x000:'90'
}

// STATE[STUB]
bool bt_character_controller::has_updates( ) const
{
	return m_bt_controller->has_updates( );	// <0x5867f0>|0x000|0x000:'95'
}

// STATE[STUB]
void bt_character_controller::jump( )
{
	m_bt_controller->jump( ); // sushi@TODO: This function doesn't have breakpoints in structure
}

// STATE[STUB]
void bt_character_controller::end_jump( )
{
	m_bt_controller->end_jump( ); // <0x5867b0>|0x000|0x000:'105'
}

// STATE[STUB]: sushi@TODO: Figure out what __format is supposed to be used for
bool bt_character_controller::adjust_foot_transform(
	float3 const&		half_size,
	float3 const&		start,
	float3 const&		finish,
	float				rotation_koef0,
	float				__formal,
	float4x4&			transform
)
{
	return m_bt_physics_world->adjust_foot_transform( half_size, start, finish, rotation_koef0, __formal, transform ); // <0x586b00>|0x000|0x000:'110'
}

// STATE[STUB]
void bt_character_controller::update_action( u32 time_delta_in_ms )
{
	m_bt_controller->updateAction( m_bt_physics_world->get_bt_internal( ), time_delta_in_ms ); // <0x5867c1>|0x000|0x000:'121'
}

// STATE[STUB]
bool bt_character_controller::can_jump( ) const
{
	return m_bt_controller->can_jump( ); // <0x5868d1>|0x000|0x000:'126'
}

// STATE[STUB]
bool bt_character_controller::on_ground( ) const
{
	return m_bt_controller->on_ground( ); // <0x586891>|0x000|0x000:'131'
}

// STATE[STUB]
void bt_character_controller::set_crouch( bool crouch )
{
	m_bt_controller->set_crouch( crouch );
}

// STATE[STUB]
bool bt_character_controller::can_crouch( ) const
{
	return true;	// <0x5867a0>|0x000|0x000:'146' sushi@NOTE: There is no `can_crouch` in the `m_bt_controller`.
}

// STATE[STUB]
bool bt_character_controller::can_stand( ) const
{
	return m_bt_controller->can_stand( ); // <0x586790>|0x000|0x000:'151'
}

} // namespace physics
} // namespace vostok