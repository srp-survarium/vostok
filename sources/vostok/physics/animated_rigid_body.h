// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_PHYSICS_ANIMATED_RIGID_BODY_H_INCLUDED
#define VOSTOK_PHYSICS_ANIMATED_RIGID_BODY_H_INCLUDED

#include <vostok/physics/api.h>
#include <vostok/physics/rigid_body_base.h>
#include <vostok/math_aabb.h>
#include <vostok/collision/primitives.h>

class btCompoundShape;
class btRigidBody;
class btCollisionShape;

namespace vostok {

namespace animation {
	class skeleton;

	typedef	resources::resource_ptr<
		skeleton,
		resources::unmanaged_intrusive_base
	> skeleton_ptr;
}

namespace memory {
	class base_allocator;
}

namespace collision {
	class bone_collision_data;
	class animated_object;
}

namespace physics {

class bt_animated_rigid_body : public bt_rigid_body_base {
public:
								bt_animated_rigid_body	( btCompoundShape* shape, btRigidBody* body, u16 game_material_id );

	/* bt_rigid_body_base overrides */
	virtual btRigidBody*		get_rigid_body			( )														override;

	virtual u16					get_triangle_material	( const s32 triangle_id, const bool is_shape_index ) const			override;
	virtual void				apply_impulse			( float3 const& impulse, float3 const& point_in_world )	override;

	virtual void				set_transform			( float4x4 const& transform )							override;
	virtual float4x4			get_transform			( ) const												override;

	/* self virtual methods */
	virtual float3 const&		center_of_mass_offset	( ) const;

	/* base_physics_object overrides */
	virtual u16					get_collision_group		( ) const override;
	virtual btCollisionObject*	get_bt_collision_obect	( )	override;

	/* self methods: sushi@NOTE: Seems like those didn't inline in target, but did here */
	void		update_bone_matrix	( const u32 index, float4x4 const& new_transform, bool recalculate_aabb );
	math::aabb	get_aabb			( )				const;
	float4x4	get_bone_transform	( const u32 index )	const;


private:
	/* 0x0000 */ /* fields for physics::bt_rigid_body_base */
	/* 0x000c */ btRigidBody*		m_bt_body;
	/* 0x0010 */ btCompoundShape*	m_shape;
	/* 0x0014 */ u16				m_game_material_id;
}; // class bt_animated_rigid_body

STATIC_SIZE_ASSERT(bt_animated_rigid_body, 0x18);

typedef buffer_vector<collision::bone_collision_data>										geometries_type;

VOSTOK_PHYSICS_API btCompoundShape*				new_compound_shape_from_hit_targets_config				( configs::binary_config_value const& config, geometries_type& geometries_data, memory::base_allocator* allocator );
VOSTOK_PHYSICS_API bt_animated_rigid_body*		new_animated_rigid_body									( btCompoundShape* shape, u16 game_material_id, memory::base_allocator* allocator );
VOSTOK_PHYSICS_API void							destroy_animated_rigid_body								( bt_animated_rigid_body* body, memory::base_allocator* allocator );
VOSTOK_PHYSICS_API collision::animated_object*	new_animated_bt_hit_model								( configs::binary_config_value const& config, animation::skeleton_ptr const& model_skeleton, memory::base_allocator* allocator );
VOSTOK_PHYSICS_API u32							calculate_bt_animated_body_size_from_hit_targets_config	( configs::binary_config_value const& config );


} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_ANIMATED_RIGID_BODY_H_INCLUDED
