////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/static_rigid_body.h>

#include "bullet_include.h"
#include <vostok/physics/bullet_utils.h>

namespace vostok {
namespace physics {

// STATE[100%|DONE]
bt_static_rigid_body::bt_static_rigid_body( bt_collision_shape_ptr shape, btRigidBody* body ):
	m_bt_body	( body ),
	m_shape		( shape )
{
	m_bt_body->setUserPointer( this );	// <0x584294>|0x000|0x000:'41'
}

// STATE[100%|DONE]: Same as `get_bt_collision_obect`.
btRigidBody* bt_static_rigid_body::get_rigid_body( ) {
	return m_bt_body;	// <0x584000>|0x000|0x000:'51'
}

// STATE[100%|DONE]
float4x4 bt_static_rigid_body::get_transform( ) const {
	return from_bullet( m_bt_body->getWorldTransform() );	// <0x584130>|0x000|0x000:'70'
}

// STATE[100%|DONE]
void bt_static_rigid_body::set_transform( float4x4 const& transform ) {
	m_bt_body->setWorldTransform( from_vostok( transform ) );						// <0x58415b>|0x000|0x000:'79'
	m_bt_body->setInterpolationWorldTransform( m_bt_body->getWorldTransform( ) );	// <0x584172>|0x017|0x017:'80'
}

// STATE[83%|DONE]: First statement used registers slightly differntly to retrieve `point_in_world`.
void bt_static_rigid_body::apply_impulse( float3 const& impulse, float3 const& point_in_world ) {
	btVector3 rel_pos = from_vostok( point_in_world ) - m_bt_body->getWorldTransform().getOrigin();	// <0x584079>|0x000|0x000:'85'
	m_bt_body->setActivationState( ACTIVE_TAG );													// <0x584097>|0x01e|0x01e:'86'
	m_bt_body->applyImpulse( from_vostok( impulse ), rel_pos );										// <0x5840df>|0x066|0x048:'87'
}

// STATE[52%|DONE]: `resource_ptr` problems.
u16 bt_static_rigid_body::get_triangle_material( s32 triangle_id, bool is_shape_index ) const {
	return m_shape->get_triangle_material( triangle_id, is_shape_index );
}

// STATE[100%|DONE]
u16 bt_static_rigid_body::get_collision_group( ) const {
	return m_bt_body->getBroadphaseHandle()->m_collisionFilterGroup;	// <0x584040>|0x000|0x000:'97'
}

// STATE[100%|DONE]
btCollisionObject* bt_static_rigid_body::get_bt_collision_obect( ) {
	return m_bt_body;	// <0x583ff0>|0x000|0x000:'108'
}

// STATE[100%|DONE]: sushi@NOTE: `m_bt_body` is deleted in destructor.
void destroy_static_rigid_body( bt_static_rigid_body* body ) {
	VOSTOK_DELETE_IMPL( g_ph_allocator, body );	// <0x584011>|0x000|0x000:'120'
}

// STATE[75%|DONE]: In target compiler thinks `shape` could have been destroyed because of uninlined `operator->`. I still don't know what's up with that.
bt_static_rigid_body* create_static_rigid_body( bt_rigid_body_construction_info const& construction_info ) {
	btVector3 local_inertia = btVector3( 0., 0., 0. );																			// <0x5842dc>|0x000|0x000:'126'

	bt_collision_shape_ptr shape = construction_info.m_collisionShape;															// <0x5842e4>|0x008|0x008:'128'

	if ( math::abs( construction_info.m_mass ) > math::epsilon_5 )																// <0x58431b>|0x03f|0x037:'130'
	{
		shape->get_bt_shape( )->calculateLocalInertia( construction_info.m_mass, local_inertia );								// <0x584342>|0x066|0x027:'132'
	} else																														// <0x584363>|0x087|0x021:'133'
		local_inertia = btVector3( 0., 0., 0. );																				// <0x584365>|0x089|0x002:'134'

	btRigidBody::btRigidBodyConstructionInfo info( construction_info.m_mass, NULL, shape->get_bt_shape( ), local_inertia );		// <0x58437d>|0x0a1|0x018:'141'

	///When a motionState is provided, the rigid body will initialize its world transform from the motion state
	///In this case, m_startWorldTransform is ignored.

	info.m_localInertia		= local_inertia;																					// <0x58439f>|0x0c3|0x022:'145'
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
	info.m_additionalDamping					= construction_info.m_additionalDamping;										// <0x5843a5>|0x0c9|0x006:'160'
	info.m_additionalDampingFactor				= construction_info.m_additionalDampingFactor;									// <0x584405>|0x129|0x060:'161'
	info.m_additionalLinearDampingThresholdSqr	= construction_info.m_additionalLinearDampingThresholdSqr;						// <0x584413>|0x137|0x00e:'162'
	info.m_additionalAngularDampingThresholdSqr	= construction_info.m_additionalAngularDampingThresholdSqr;
	info.m_additionalAngularDampingFactor		= construction_info.m_additionalAngularDampingFactor;

	btRigidBody* bt_rigid_body = VOSTOK_NEW_IMPL( g_ph_allocator, btRigidBody )( info );										// <0x58441f>|0x143|0x00c:'166'
	bt_static_rigid_body* static_rigid_body = VOSTOK_NEW_IMPL( g_ph_allocator, bt_static_rigid_body )( shape, bt_rigid_body );	// <0x58448d>|0x1b1|0x06e:'167'
	return static_rigid_body;																									// <0x5844d4>|0x1f8|0x047:'168'
}

// STATE[63%|DONE]: loose_ptr_base inline in target
bt_static_rigid_body::~bt_static_rigid_body( ) {
	ASSERT( m_bt_body );
	VOSTOK_DELETE_IMPL( g_ph_allocator, m_bt_body );	// <0x58419a>|0x000|0x000:'174'
}

} // namespace physics
} // namespace vostok
