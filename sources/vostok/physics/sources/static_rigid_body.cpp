// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/physics/static_rigid_body.h>

#include "bullet_include.h"
#include <vostok/physics/bullet_utils.h>

namespace vostok {
namespace physics {

bt_static_rigid_body::bt_static_rigid_body( bt_collision_shape_ptr shape, btRigidBody* body ):
	m_bt_body	( body ),
	m_shape		( shape )
{
	m_bt_body->setUserPointer( this );
}

btRigidBody* bt_static_rigid_body::get_rigid_body( ) {
	return m_bt_body;
}

float4x4 bt_static_rigid_body::get_transform( ) const {
	return from_bullet( m_bt_body->getWorldTransform() );
}

void bt_static_rigid_body::set_transform( float4x4 const& transform ) {
	m_bt_body->setWorldTransform( from_vostok( transform ) );
	m_bt_body->setInterpolationWorldTransform( m_bt_body->getWorldTransform( ) );
}

void bt_static_rigid_body::apply_impulse( float3 const& impulse, float3 const& point_in_world ) {
	btVector3 rel_pos = from_vostok( point_in_world ) - m_bt_body->getWorldTransform().getOrigin();
	m_bt_body->setActivationState( ACTIVE_TAG );
	m_bt_body->applyImpulse( from_vostok( impulse ), rel_pos );
}

u16 bt_static_rigid_body::get_triangle_material( const s32 triangle_id, const bool is_shape_index ) const {
	return m_shape->get_triangle_material( triangle_id, is_shape_index );
}

u16 bt_static_rigid_body::get_collision_group( ) const {
	return m_bt_body->getBroadphaseHandle()->m_collisionFilterGroup;
}

btCollisionObject* bt_static_rigid_body::get_bt_collision_obect( ) {
	return m_bt_body;
}

// sushi@NOTE: `m_bt_body` is deleted in destructor.
void destroy_static_rigid_body( bt_static_rigid_body* body ) {
	VOSTOK_DELETE_IMPL( g_ph_allocator, body );
}

bt_static_rigid_body* create_static_rigid_body( bt_rigid_body_construction_info const& construction_info ) {
	btVector3 local_inertia = btVector3( 0., 0., 0. );

	bt_collision_shape_ptr shape = construction_info.m_collisionShape;

	if ( math::abs( construction_info.m_mass ) > math::epsilon_5 )
	{
		shape->get_bt_shape( )->calculateLocalInertia( construction_info.m_mass, local_inertia );
	} else
		local_inertia = btVector3( 0., 0., 0. );

	btRigidBody::btRigidBodyConstructionInfo info( construction_info.m_mass, NULL, shape->get_bt_shape( ), local_inertia );

	///When a motionState is provided, the rigid body will initialize its world transform from the motion state
	///In this case, m_startWorldTransform is ignored.

	info.m_localInertia		= local_inertia;
	info.m_linearDamping	= construction_info.m_linearDamping;
	info.m_angularDamping	= construction_info.m_angularDamping;

	///best simulation results when friction is non-zero
	info.m_friction			= construction_info.m_friction;
	///best simulation results using zero restitution.
	info.m_restitution		= construction_info.m_restitution;

	info.m_linearSleepingThreshold		= construction_info.m_linearSleepingThreshold;
	info.m_angularSleepingThreshold		= construction_info.m_angularSleepingThreshold;

	//Additional damping can help avoiding lowpass jitter motion, help stability for ragdolls etc.
	//Such damping is undesirable, so once the overall simulation quality of the rigid body dynamics system has improved, this should become obsolete
	info.m_additionalDamping					= construction_info.m_additionalDamping;
	info.m_additionalDampingFactor				= construction_info.m_additionalDampingFactor;
	info.m_additionalLinearDampingThresholdSqr	= construction_info.m_additionalLinearDampingThresholdSqr;
	info.m_additionalAngularDampingThresholdSqr	= construction_info.m_additionalAngularDampingThresholdSqr;
	info.m_additionalAngularDampingFactor		= construction_info.m_additionalAngularDampingFactor;

	btRigidBody* bt_rigid_body = VOSTOK_NEW_IMPL( g_ph_allocator, btRigidBody )( info );
	bt_static_rigid_body* static_rigid_body = VOSTOK_NEW_IMPL( g_ph_allocator, bt_static_rigid_body )( shape, bt_rigid_body );
	return static_rigid_body;
}

bt_static_rigid_body::~bt_static_rigid_body( ) {
	ASSERT( m_bt_body );
	VOSTOK_DELETE_IMPL( g_ph_allocator, m_bt_body );
}

} // namespace physics
} // namespace vostok
