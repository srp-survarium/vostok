// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_PHYSICS_STATIC_RIGID_BODY_H_INCLUDED
#define VOSTOK_PHYSICS_STATIC_RIGID_BODY_H_INCLUDED

#include <vostok/physics/rigid_body_base.h>
#include <vostok/physics/collision_shapes.h> // for typedef

class btRigidBody;

namespace vostok {
namespace physics {

class bt_static_rigid_body : public bt_rigid_body_base {
typedef bt_rigid_body_base super;

public:

	bt_static_rigid_body( bt_collision_shape_ptr shape, btRigidBody* body );
	virtual ~bt_static_rigid_body( );

	virtual btRigidBody*		get_rigid_body				( ) override;
	virtual u16					get_triangle_material		( const s32 triangle_id, const bool is_shape_index ) const override;

	virtual void				apply_impulse				( float3 const& impulse, float3 const& point_in_world ) override;

	virtual void				set_transform				( float4x4 const& transform ) override;
	virtual float4x4			get_transform				( ) const override;

	virtual u16					get_collision_group			( ) const override;
	virtual btCollisionObject*	get_bt_collision_obect		( ) override;

	// STATE[UNMATCHABLE]: the client target emits no procedure or inline expansion for these helpers.
	inline	bool								is_active					( ) const { /* no source */ }
	inline	void								set_ccd_motion_thresholds	( float arg_0, float arg_1 ) { /* no source */ }
	inline	const bt_collision_shape_ptr		get_collision_shape			( ) const { return m_shape; }
	inline	void								predict_integrated_transform( float arg_0, float4x4& arg_1 ) const { /* no source */ }

private:
	/* 0x0000 */	/* bt_rigid_body_base */
	/* 0x000c */	btRigidBody*						m_bt_body;
	/* 0x0010 */	bt_collision_shape_ptr				m_shape;
}; // class bt_static_rigid_body

STATIC_SIZE_ASSERT(bt_static_rigid_body, 0x14);


struct bt_rigid_body_construction_info  {
public:
	/* 0x0000 */	float						m_mass;
	/* 0x0004 */	bt_collision_shape_ptr		m_collisionShape;
	/* 0x0008 */	float						m_linearDamping;
	/* 0x000c */	float						m_angularDamping;
	/* 0x0010 */	float						m_friction;
	/* 0x0014 */	float						m_restitution;
	/* 0x0018 */	float						m_linearSleepingThreshold;
	/* 0x001c */	float						m_angularSleepingThreshold;
	/* 0x0020 */	bool						m_additionalDamping;
	/* 0x0024 */	float						m_additionalDampingFactor;
	/* 0x0028 */	float						m_additionalLinearDampingThresholdSqr;
	/* 0x002c */	float						m_additionalAngularDampingThresholdSqr;
	/* 0x0030 */	float						m_additionalAngularDampingFactor;

	bt_rigid_body_construction_info( );

	// STATE[UNMATCHABLE]: the client constructs this record field-by-field and never emits load.
	inline	bool	load						( configs::binary_config_value const& arg_0 ) { /* no source */ return true; }
}; // struct bt_rigid_body_construction_info

STATIC_SIZE_ASSERT(bt_rigid_body_construction_info, 0x34);


VOSTOK_PHYSICS_API bt_static_rigid_body*	create_static_rigid_body	( bt_rigid_body_construction_info const& construction_info );
VOSTOK_PHYSICS_API void						destroy_static_rigid_body	( bt_static_rigid_body* body );

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_STATIC_RIGID_BODY_H_INCLUDED
