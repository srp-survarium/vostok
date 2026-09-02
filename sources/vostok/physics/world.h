// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////
#ifndef VOSTOK_PHYSICS_WORLD_H_INCLUDED
#define VOSTOK_PHYSICS_WORLD_H_INCLUDED

#include <vostok/physics/ray_result.h>

class btCollisionShape;
class btTransform;
class btVector3;
class btIDebugDraw;
class btTypedConstraint;

namespace vostok {
namespace physics {

class bt_rigid_body_base;
class bt_soft_body_rope;
class bt_collision_shape;
class base_physics_object;

class bt_constraint {
public:
	bt_constraint		( ) {}
	virtual void load	( configs::binary_config_value const& ) {}

public:
	// target has this protected too, but bullet_physics_world.cpp add/remove
	// read it directly - kept public until that access path is understood
	/* 0x0004 */	btTypedConstraint*		m_bt_typed_constraint;
protected:
	/* 0x0008 */	bt_rigid_body_base*		m_body_a;
	/* 0x000c */	bt_rigid_body_base*		m_body_b;
};

STATIC_SIZE_ASSERT(bt_constraint, 0x10);


struct VOSTOK_NOVTABLE world {
public:
	virtual							~world				( )												{};
	virtual	void					tick				( const u32 current_time_in_ms )				= 0;

	virtual void					initialize			( )												= 0;
	virtual void					destroy				( )												= 0;

	virtual void					set_renderer		( btIDebugDraw* const renderer )				= 0;
	virtual void					debug_draw_world	( )												= 0;
	virtual void					draw_object			( btCollisionShape* const shape, btTransform const& transform, btVector3 const& color ) = 0;
	virtual void					create_test_scene	( )												= 0;

	virtual void					add					( bt_rigid_body_base* body, u16 filter_group, u16 filter_mask ) = 0;
	virtual void					add					( bt_soft_body_rope* body )										= 0;
	virtual void					add					( bt_constraint* constraint )									= 0;

	virtual void					remove				( bt_rigid_body_base* body )									= 0;
	virtual void					remove				( bt_soft_body_rope* body )										= 0;
	virtual void					remove				( bt_constraint* constraint )									= 0;

	virtual void					move				( bt_rigid_body_base* body, float4x4 const& new_transform )		= 0;

	virtual	closest_ray_result		ray_test					(
										float3 const&		ray_from,
										float3 const&		ray_dir,
										const float			ray_length,
										u16					filter_group,
										u16					filter_mask
									) = 0;

	virtual	void					ray_query					(
										float3 const&					ray_from,
										float3 const&					ray_dir,
										const float						ray_length,
										vectora<closest_ray_result>&	results,
										u16								filter_group,
										u16								filter_mask
									) = 0;

	virtual	void					object_query				(
										bt_collision_shape*				const shape,
										float4x4 const&					transform_from,
										float4x4 const&					transform_to,
										vectora<closest_ray_result>&	results,
										u16								filter_group,
										u16								filter_mask
									) = 0;

	virtual	bool					recover_from_penetrations	(
										bt_collision_shape*		const shape,
										float4x4 const&			transform_initial,
										float4x4&				transform_result,
										u16						filter_group,
										u16						filter_mask
									) = 0;

	virtual math::aabb				get_world_aabb			( ) const	= 0;
	virtual void					on_before_reuse			( )			= 0;

public:
	typedef
		boost::function<void ( base_physics_object *, base_physics_object *, float3 const & )>
		callback_type;

	typedef std::multimap< base_physics_object*, callback_type* >				callbacks_type;
	typedef std::pair< callbacks_type::iterator, callbacks_type::iterator >		callbacks_begin_end_pair;

	virtual void				subscribe_on_contact		( base_physics_object* object, callback_type* callback )	= 0;
	virtual void				unsubscribe_from_contact	( base_physics_object* object, callback_type* callback )	= 0;
}; // struct world

STATIC_SIZE_ASSERT(world, 0x4);

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_WORLD_H_INCLUDED
