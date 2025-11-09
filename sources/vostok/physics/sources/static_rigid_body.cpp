////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/static_rigid_body.h>

#include "bullet_include.h"
#include <vostok/physics/bullet_utils.h>

namespace vostok {
namespace physics {

// STATE[PARTIAL: 48%]
bt_static_rigid_body::bt_static_rigid_body( bt_collision_shape_ptr shape, btRigidBody* body ):
	m_bt_body	( body ),
	m_shape		( shape )
{
	body->setUserPointer( this );	// <0x584294>|0x000|0x000:'41'
}

// STATE[DONE]: Same as `get_bt_collision_obect`
btRigidBody* bt_static_rigid_body::get_rigid_body( ) {
	return m_bt_body;	// <0x584000>|0x000|0x000:'51'
}

// STATE[DONE]
float4x4 bt_static_rigid_body::get_transform( ) const {
	return from_bullet( m_bt_body->getWorldTransform() );	// <0x584130>|0x000|0x000:'70'
}

// STATE[DONE]
void bt_static_rigid_body::set_transform( float4x4 const& transform ) {
	m_bt_body->setWorldTransform( from_vostok( transform ) );						// <0x58415b>|0x000|0x000:'79'
	m_bt_body->setInterpolationWorldTransform( m_bt_body->getWorldTransform( ) );	// <0x584172>|0x017|0x017:'80'
}

// STATE[PARTIAL: 83%]: the asm looks the same, the rigsters were used slightly differently
void bt_static_rigid_body::apply_impulse( float3 const& impulse, float3 const& point_in_world ) {
	btVector3 rel_pos = from_vostok( point_in_world ) - m_bt_body->getWorldTransform().getOrigin();	// <0x584079>|0x000|0x000:'85'
	m_bt_body->setActivationState( ACTIVE_TAG );													// <0x584097>|0x01e|0x01e:'86'
	m_bt_body->applyImpulse( from_vostok( impulse ), rel_pos );										// <0x5840df>|0x066|0x048:'87'
}

// STATE[PARTIAL: 52%]: target didn't inline `->` for `resource_ptr`. Maybe it has different impl?
u16 bt_static_rigid_body::get_triangle_material( s32 triangle_id, bool is_shape_index ) const {
	return m_shape->get_triangle_material( triangle_id, is_shape_index );
}

// STATE[DONE]
u16 bt_static_rigid_body::get_collision_group( ) const {
	return m_bt_body->getBroadphaseHandle()->m_collisionFilterGroup;	// <0x584040>|0x000|0x000:'97'
}

// STATE[DONE]
btCollisionObject* bt_static_rigid_body::get_bt_collision_obect( ) {
	return m_bt_body;	// <0x583ff0>|0x000|0x000:'108'
}

// STATE[STUB]
void destroy_static_rigid_body( bt_static_rigid_body* body ) {
	VOSTOK_DELETE_IMPL( g_ph_allocator, body );	// <0x584011>|0x000|0x000:'120'
}

// STATE[STUB]
// vostok::physics::bt_static_rigid_body* vostok::physics::create_static_rigid_body(vostok::physics::bt_rigid_body_construction_info const&)
bt_static_rigid_body* create_static_rigid_body( bt_rigid_body_construction_info const& construction_info ) {
	// LOCALS
	// resources::resource_ptr<bt_collision_shape,resources::unmanaged_intrusive_base> shape
	// btRigidBody::btRigidBodyConstructionInfo info
	// btVector3                       local_inertia
	// ******
	return NULL;


	// FUNCTION BODY

	// <0x5842dc>|0x000|0x000:'126'

	// <0x5842e4>|0x008|0x008:'128'

	// <0x58431b>|0x03f|0x037:'130'

	// <0x584342>|0x066|0x027:'132'
	// <0x584363>|0x087|0x021:'133'
	// <0x584365>|0x089|0x002:'134'






	// <0x58437d>|0x0a1|0x018:'141'



	// <0x58439f>|0x0c3|0x022:'145'














	// <0x5843a5>|0x0c9|0x006:'160'
	// <0x584405>|0x129|0x060:'161'
	// <0x584413>|0x137|0x00e:'162'



	// <0x58441f>|0x143|0x00c:'166'
	// <0x58448d>|0x1b1|0x06e:'167'
	// <0x5844d4>|0x1f8|0x047:'168'
	// ******
}

// STATE[PARTIAL: 45%]: target inlined parent constructors, base didn't
bt_static_rigid_body::~bt_static_rigid_body( ) {
	// ASSERT?
	VOSTOK_DELETE_IMPL( g_ph_allocator, m_bt_body );	// <0x58419a>|0x000|0x000:'174'
}

} // namespace physics
} // namespace vostok