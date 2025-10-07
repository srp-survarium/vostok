////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_STATIC_RIGID_BODY_H_INCLUDED
#define VOSTOK_PHYSICS_STATIC_RIGID_BODY_H_INCLUDED

#include <vostok/physics/rigid_body_base.h>
#include <vostok/physics/collision_shapes.h> // for typedef

class btRigidBody;

namespace vostok {
namespace physics {

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class bt_static_rigid_body : bt_rigid_body_base {
public:

	bt_static_rigid_body( bt_collision_shape_ptr shape, btRigidBody* body );
	virtual ~bt_static_rigid_body( );

	virtual btRigidBody*		get_rigid_body				( ) override;
	virtual u16					get_triangle_material		( s32 triangle_id, bool is_shape_index ) const override;

	virtual void				apply_impulse				( float3 const& impulse, float3 const& point_in_world ) override;

	virtual void				set_transform				( float4x4 const& transform ) override;
	virtual float4x4			get_transform				( ) const override;

	virtual u16					get_collision_group			( ) const override;
	virtual btCollisionObject*	get_bt_collision_obect		( ) override;

	bool						is_active					( ) const					/* no source */;
	void						set_ccd_motion_thresholds	(float, float)				/* no source */;
	bt_collision_shape_ptr		get_collision_shape			( ) const					/* no source */;
	void						predict_integrated_transform(float, float4x4&) const	/* no source */;

private:
	/* offset 0x0000 */ /* fields for bt_rigid_body_base */
	/* offset 0x000c */ btRigidBody*                        m_bt_body;
	/* offset 0x0010 */ bt_collision_shape_ptr              m_shape;
}; // class bt_static_rigid_body

namespace {
	typedef char size_assert[
		sizeof(bt_static_rigid_body) == 0x14 ? 1 : -1
	];
}


struct bt_rigid_body_construction_info  {
public:
	bt_rigid_body_construction_info( );

	bool load(configs::binary_config_value const&) { return false; } // sushi@TODO: No source


	/* offset 0x0000 */ float                               m_mass;
	/* offset 0x0004 */ bt_collision_shape_ptr              m_collisionShape;
	/* offset 0x0008 */ float                               m_linearDamping;
	/* offset 0x000c */ float                               m_angularDamping;
	/* offset 0x0010 */ float                               m_friction;
	/* offset 0x0014 */ float                               m_restitution;
	/* offset 0x0018 */ float                               m_linearSleepingThreshold;
	/* offset 0x001c */ float                               m_angularSleepingThreshold;
	/* offset 0x0020 */ bool                                m_additionalDamping;
	/* offset 0x0024 */ float                               m_additionalDampingFactor;
	/* offset 0x0028 */ float                               m_additionalLinearDampingThresholdSqr;
	/* offset 0x002c */ float                               m_additionalAngularDampingThresholdSqr;
	/* offset 0x0030 */ float                               m_additionalAngularDampingFactor;
}; // struct bt_rigid_body_construction_info

namespace {
	typedef char size_assert[
		sizeof(bt_rigid_body_construction_info) == 0x34 ? 1 : -1
	];
}

VOSTOK_PHYSICS_API bt_static_rigid_body*	create_static_rigid_body	( bt_rigid_body_construction_info const& construction_info );
VOSTOK_PHYSICS_API void					destroy_static_rigid_body	(  bt_static_rigid_body* body );

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_STATIC_RIGID_BODY_H_INCLUDED