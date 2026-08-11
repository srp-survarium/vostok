////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BULLET_PHYSICS_WORLD_H_INCLUDED
#define BULLET_PHYSICS_WORLD_H_INCLUDED

#include <vostok/physics/world.h>
#include <vostok/physics/engine.h>
#include <vostok/physics/soft_body.h>
#include <vostok/physics/contact_test_predicate.h>

#include "bullet_include.h"

namespace vostok {
namespace physics {

class bullet_physics_world : public world, public boost::noncopyable {
public:
											bullet_physics_world		( memory::base_allocator& allocator, engine& engine );

	virtual	void							initialize					( ) override;
	virtual	void							destroy						( ) override;
	virtual	void							tick						( const u32 current_time_in_ms ) override;

	virtual	void							create_test_scene			( ) override;

	virtual	void							set_renderer				( btIDebugDraw* const renderer ) override;
	virtual	void							draw_object					( btCollisionShape* const shape, btTransform const& transform, btVector3 const& color ) override;
	virtual	void							debug_draw_world			( ) override;

	virtual	void							add							( bt_rigid_body_base* body, u16 filter_group, u16 filter_mask ) override;
	virtual	void							add							( bt_soft_body_rope* body ) override;
	virtual	void							add							( bt_constraint* constraint ) override;

	virtual	void							remove						( bt_rigid_body_base* body ) override;
	virtual	void							remove						( bt_soft_body_rope* body ) override;
	virtual	void							remove						( bt_constraint* constraint ) override;

	virtual	void							move						( bt_rigid_body_base* body, float4x4 const& new_transform ) override;

	virtual	closest_ray_result				ray_test					(
												float3 const&		ray_from,
												float3 const&		ray_dir,
												const float			ray_length,
												u16					filter_group,
												u16					filter_mask
											) override;

	virtual	void							ray_query					(
												float3 const&					ray_from,
												float3 const&					ray_dir,
												const float						ray_length,
												vectora<closest_ray_result>&	results,
												u16								filter_group,
												u16								filter_mask
											) override;

	virtual	void							object_query				(
												bt_collision_shape*				const shape,
												float4x4 const&					transform_from,
												float4x4 const&					transform_to,
												vectora<closest_ray_result>&	results,
												u16								filter_group,
												u16								filter_mask
											) override;

	virtual	math::aabb						get_world_aabb				( ) const override { return m_world_aabb; }

	virtual	void							on_before_reuse				( ) override;

	virtual	bool							recover_from_penetrations	(
												bt_collision_shape*		const shape,
												float4x4 const&			transform_initial,
												float4x4&				transform_result,
												u16						filter_group,
												u16						filter_mask
											) override;

	virtual	void							subscribe_on_contact		( base_physics_object* object, callback_type* callback ) override;
	virtual	void							unsubscribe_from_contact	( base_physics_object* object, callback_type* callback ) override;


	inline	memory::base_allocator&			allocator					( ) { return m_allocator; }
	inline	btSoftRigidDynamicsWorld*		get_bt_internal				( ) { return m_dynamicsWorld; }

	// STATE[REMOVED]: rope create/destroy are referenced only from editor object_wire_set.cpp,
	// not compiled into survarium.exe; no free create/destroy_soft_body_rope nor these inlines
	// appear in either binary. Empty stubs correct.
	inline	bt_soft_body_rope*				create_soft_body_rope		( rope_construction_info const& arg_0 ) { /* no source */ }
	inline	void							destroy_soft_body_rope		( bt_soft_body_rope* arg_0 ) { /* no source */ } // STATE[REMOVED]

			void							contact_pair_test			( contact_test_predicate& predicate, btCollisionObject* first_object, btCollisionObject* second_object );
			bool							adjust_foot_transform		(
												float3 const&		half_size,
												float3 const&		start,
												float3 const&		finish,
												float				rotation_koef0,
												float				__formal,
												float4x4&			transform
											);

private:
			void							notify_about_contact		( );

private:
	/* 0x0000 */	/* world */
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	callbacks_type						m_contact_callbacks;
	/* 0x001c */	memory::base_allocator&				m_allocator;
	/* 0x0020 */	btCollisionConfiguration*			m_collisionConfiguration;
	/* 0x0024 */	btCollisionDispatcher*				m_dispatcher;
	/* 0x0028 */	btBroadphaseInterface*				m_overlappingPairCache;
	/* 0x002c */	btConstraintSolver*					m_constraintSolver;
	/* 0x0030 */	btSoftBodyWorldInfo*				m_softBodyWorldInfo;
	/* 0x0034 */	btSoftRigidDynamicsWorld*			m_dynamicsWorld;
	/* 0x0038 */	btGhostPairCallback*				m_ghost_pair_callback;
	/* 0x003c */	engine&								m_engine;
	/* 0x0040 */	math::aabb							m_world_aabb;
	/* 0x0058 */	float								m_last_frame_time;
	/* 0x005c */	float								m_last_frame_delta;
}; // class bullet_physics_world

STATIC_SIZE_ASSERT(bullet_physics_world, 0x60);

} // namespace physics
} // namespace vostok

#endif // #ifndef BULLET_PHYSICS_WORLD_H_INCLUDED
