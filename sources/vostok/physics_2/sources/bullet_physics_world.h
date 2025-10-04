////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BULLET_PHYSICS_WORLD_H_INCLUDED
#define BULLET_PHYSICS_WORLD_H_INCLUDED

#include <vostok/physics_2/world.h>
#include <vostok/physics_2/engine.h>

class btGhostPairCallback;
class btSoftRigidDynamicsWorld;
struct btSoftBodyWorldInfo;
class btConstraintSolver;
class btBroadphaseInterface;
class btCollisionConfiguration;
class btCollisionDispatcher;

namespace vostok {
namespace physics_2 {

class base_physics_object;

/*
// STATE[STUB]
// void vostok::resources::resource_ptr<vostok::physics::bt_collision_shape,vostok::resources::unmanaged_intrusive_base>::~resource_ptr<vostok::physics::bt_collision_shape,vostok::resources::unmanaged_intrusive_base>()
resources::resource_ptr<bt_collision_shape,resources::unmanaged_intrusive_base>::~resource_ptr<bt_collision_shape,resources::unmanaged_intrusive_base>( )
{
}
*/

class bullet_physics_world : world, boost::noncopyable {
public:
	bullet_physics_world( engine& engine );

	virtual void initialize( ) override;

	virtual void destroy( ) override;

	virtual void tick( u32 current_time_in_ms ) override;
#if 0
	virtual void create_test_scene( ) override;

	virtual void set_renderer( btIDebugDraw* renderer ) override;

	virtual void draw_object( btCollisionShape* shape, btTransform const& transform, btVector3 const& color ) override;

	virtual void debug_draw_world( ) override;

	virtual void add( bt_constraint* constraint ) override;
	virtual void add( bt_soft_body_rope* body ) override;
	virtual void add( bt_rigid_body_base* body, u16 filter_group, u16 filter_mask ) override;

	virtual void remove( bt_constraint* constraint ) override;
	virtual void remove( bt_soft_body_rope* body ) override;
	virtual void remove( bt_rigid_body_base* body ) override;

	virtual void move( bt_rigid_body_base* body, float4x4 const& new_transform ) override;

	virtual closest_ray_result ray_test(
		float3 const&                      ray_from,
		float3 const&                      ray_dir,
		float                              ray_length,
		u16                                filter_group,
		u16                                filter_mask) override;

	virtual void ray_query(
		float3 const&                      ray_from,
		float3 const&                      ray_dir,
		float                              ray_length,
		vectora<closest_ray_result>&       results,
		u16                                filter_group,
		u16                                filter_mask) override;

	virtual void object_query(
		bt_collision_shape*                shape,
		float4x4 const&                    transform_from,
		float4x4 const&                    transform_to,
		vectora<closest_ray_result>&       results,
		u16                                filter_group,
		u16                                filter_mask) override;

	virtual math::aabb get_world_aabb() const /* no source */ override;

	virtual void on_before_reuse( ) override;

	virtual bool recover_from_penetrations(
		bt_collision_shape*                shape,
		float4x4 const&                    transform_initial,
		float4x4&                          transform_result,
		u16                                filter_group,
		u16                                filter_mask) override;

	virtual void subscribe_on_contact(
		base_physics_object*               object,
		boost::function<void __cdecl(base_physics_object *,base_physics_object *,float3 const &)>* callback) override;

	virtual void unsubscribe_from_contact(
		base_physics_object*               object,
		boost::function<void __cdecl(base_physics_object *,base_physics_object *,float3 const &)>* callback) override;
	/*
	memory::base_allocator& allocator() /* no source */;

	btSoftRigidDynamicsWorld* get_bt_internal() /* no source */;

	bt_soft_body_rope* create_soft_body_rope(rope_construction_info const&) /* no source */;

	void destroy_soft_body_rope(bt_soft_body_rope*) /* no source */;
	*/
	void contact_pair_test(
		contact_test_predicate&            predicate,
		btCollisionObject*                 first_object,
		btCollisionObject*                 second_object);

	bool adjust_foot_transform(
		float3 const&                      half_size,
		float3 const&                      start,
		float3 const&                      finish,
		float                              rotation_koef0,
		float                              __formal,
		float4x4&                          transform);

	void notify_about_contact( );
#endif
	typedef std::multimap<
		base_physics_object *,
		boost::function<void __cdecl(base_physics_object *,base_physics_object *,float3 const &)> *,
		std::less<base_physics_object *>,
		std::allocator<
			std::pair<
				base_physics_object * const,
				boost::function<void __cdecl(base_physics_object *,base_physics_object *,float3 const &)> 
			*>
		>
	> callbacks_type;
private:
#if 0
	/* offset 0x0000 */ /* fields for world */
	/* offset 0x0004 */ /* fields for boost::noncopyable */
	/* offset 0x0004 */ callbacks_type                      m_contact_callbacks;
	/* offset 0x001c */ memory::base_allocator&             m_allocator;
	/* offset 0x0020 */ btCollisionConfiguration*           m_collisionConfiguration;
	/* offset 0x0024 */ btCollisionDispatcher*              m_dispatcher;
	/* offset 0x0028 */ btBroadphaseInterface*              m_overlappingPairCache;
	/* offset 0x002c */ btConstraintSolver*                 m_constraintSolver;
	/* offset 0x0030 */ btSoftBodyWorldInfo*                m_softBodyWorldInfo;
	/* offset 0x0034 */ btSoftRigidDynamicsWorld*           m_dynamicsWorld;
	/* offset 0x0038 */ btGhostPairCallback*                m_ghost_pair_callback;
	/* offset 0x003c */ engine&                             m_engine;
	/* offset 0x0040 */ math::aabb                          m_world_aabb;
	/* offset 0x0058 */ float                               m_last_frame_time;
	/* offset 0x005c */ float                               m_last_frame_delta;
#endif
}; // class bullet_physics_world
#if 0
namespace {
	typedef char size_assert[
		sizeof(bullet_physics_world) == 0x60 ? 1 : -1
	];
}
#endif

} // namespace physics
} // namespace vostok

#endif // #ifndef BULLET_PHYSICS_WORLD_H_INCLUDED