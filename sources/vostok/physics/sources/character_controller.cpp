// SPDX-License-Identifier: GPL-3.0-or-later
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

namespace survarium {
	extern vostok::memory::doug_lea_allocator_type*	g_allocator;
} // namespace survarium

namespace vostok {
namespace physics {

bt_character_controller* create_character_controller( vostok::memory::base_allocator& allocator, world* w )
{
	return VOSTOK_NEW_IMPL( static_cast<vostok::memory::base_allocator&>( *::survarium::g_allocator ), bt_character_controller )( w );
}

bt_character_controller::bt_character_controller( world* w ) :
	m_active ( false )
{
	m_bt_physics_world = static_cast<bullet_physics_world*>( w );
}

bt_character_controller::~bt_character_controller( )
{
	VOSTOK_DELETE_IMPL( allocator( ), m_bt_controller );
}

memory::base_allocator& bt_character_controller::allocator( )
{
	return m_bt_physics_world->allocator( );
}

void bt_character_controller::initialize( )
{
	btPairCachingGhostObject* ghost = VOSTOK_NEW_IMPL( allocator( ), btPairCachingGhostObject );
	ghost->setCollisionFlags( 0x10 );
	ghost->setFriction( 100.0f );
	m_bt_controller =  VOSTOK_NEW_IMPL( allocator( ), bullet_character_controller )(
		ghost,
		float2( 0.9f, 1.8f ),
		float2( 0.9f, 1.1f ),
		1 << 2,
		1 << 1
	);
}

void bt_character_controller::activate( float4x4 const& t )
{
	ASSERT( !m_bt_controller->m_collision_world );
	m_bt_controller->insert( m_bt_physics_world->get_bt_internal( ) );
	m_bt_controller->set_transform( from_vostok( t ) );
}

void bt_character_controller::deactivate( )
{
	ASSERT( m_bt_controller->m_collision_world );
	m_bt_controller->remove( m_bt_controller->m_collision_world );
}

float4x4 bt_character_controller::get_transform( )
{
	return from_bullet( m_bt_controller->get_transform( ) );
}

void bt_character_controller::set_transform( float4x4 const& transform )
{
	m_bt_controller->set_transform( from_vostok( transform ) );
}

void bt_character_controller::set_walk_direction( float3 const& direction )
{
	m_bt_controller->set_desired_walk_vector( from_vostok ( direction ) );
}

bool bt_character_controller::has_updates( ) const
{
	return m_bt_controller->has_updates( );
}

void bt_character_controller::jump( )
{
	m_bt_controller->jump( );
}

void bt_character_controller::end_jump( )
{
	m_bt_controller->end_jump( );
}

bool bt_character_controller::adjust_foot_transform(
	float3 const&		half_size,
	float3 const&		start,
	float3 const&		finish,
	float				rotation_koef0,
	float				__formal,
	float4x4&			transform
)
{
	return m_bt_physics_world->adjust_foot_transform( half_size, start, finish, rotation_koef0, __formal, transform );
}

void bt_character_controller::update_action( const u32 time_delta_in_ms )
{
	m_bt_controller->updateAction( m_bt_physics_world->get_bt_internal( ), time_delta_in_ms * math::epsilon_3 );
}

bool bt_character_controller::can_jump( ) const
{
	return m_bt_controller->can_jump( );
}

bool bt_character_controller::on_ground( ) const
{
	return m_bt_controller->on_ground( );
}

void bt_character_controller::set_crouch( bool crouch )
{
	m_bt_controller->set_crouch( crouch );
}

bool bt_character_controller::can_crouch( ) const
{
	return true;
}

bool bt_character_controller::can_stand( ) const
{
	return m_bt_controller->can_stand( );
}

} // namespace physics
} // namespace vostok
