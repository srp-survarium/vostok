////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_2_WORLD_H_INCLUDED
#define VOSTOK_PHYSICS_2_WORLD_H_INCLUDED

class btIDebugDraw;
class btCollisionShape;



namespace vostok {
namespace physics_2 {

/* vostok::physics::world */

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

struct VOSTOK_NOVTABLE world {
public:
	virtual										~world				( )												{};
	virtual	void								tick				( u32 )											= 0;

	virtual void								initialize			( )												= 0;
	virtual void								destroy				( )												= 0;
#if 0
	virtual void set_renderer(btIDebugDraw*) /* no source */ = 0;

	virtual void debug_draw_world() /* no source */ = 0;

	virtual void draw_object(btCollisionShape*, btTransform const&, btVector3 const&) /* no source */ = 0;

	virtual void create_test_scene() /* no source */ = 0;

	virtual void add	(bt_constraint*)				= 0;
	virtual void add	(bt_soft_body_rope*)			= 0;
	virtual void add	(bt_rigid_body_base*, u16, u16) = 0;

	virtual void remove	(bt_constraint*)				= 0;
	virtual void remove	(bt_soft_body_rope*)			= 0;
	virtual void remove	(bt_rigid_body_base*)			= 0;

	virtual void move(bt_rigid_body_base*, float4x4 const&) /* no source */ = 0;

	virtual closest_ray_result ray_test(float3 const&, float3 const&, float, u16, u16) /* no source */ = 0;

	virtual void ray_query(float3 const&, float3 const&, float, vectora<closest_ray_result>&, u16, u16) /* no source */ = 0;

	virtual void object_query(bt_collision_shape*, float4x4 const&, float4x4 const&, vectora<closest_ray_result>&, u16, u16) /* no source */ = 0;

	virtual bool recover_from_penetrations(bt_collision_shape*, float4x4 const&, float4x4&, u16, u16) /* no source */ = 0;

	virtual aabb get_world_aabb() const /* no source */ = 0;

	virtual void on_before_reuse() /* no source */ = 0;

	virtual void subscribe_on_contact(base_physics_object*, boost::function<void __cdecl(base_physics_object *,base_physics_object *,float3 const &)>*) /* no source */ = 0;

	virtual void unsubscribe_from_contact(base_physics_object*, boost::function<void __cdecl(base_physics_object *,base_physics_object *,float3 const &)>*) /* no source */ = 0;
#endif
private:
}; // struct world

namespace {
	typedef char size_assert[
		sizeof(world) == 0x4 ? 1 : -1
	];
}

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_2_WORLD_H_INCLUDED