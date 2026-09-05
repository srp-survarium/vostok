// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_PHYSICS_BT_GHOST_OBJECT_H_INCLUDED
#define VOSTOK_PHYSICS_BT_GHOST_OBJECT_H_INCLUDED

#include <vostok/physics/base_physics_object.h>
#include <vostok/physics/collision_shapes.h>
#include <vostok/physics/contact_test_predicate.h>

class btPairCachingGhostObject;

namespace survarium {
class booby_trap_set_core;
}

namespace vostok {
namespace physics {

typedef buffer_vector<base_physics_object *> base_physics_objects_type; // sushi@TODO: Find proper place for the typedef. Obviously in  base_physics_object module

class bt_ghost_object;
VOSTOK_PHYSICS_API void destroy_ghost_object( bt_ghost_object* obj );

class bt_ghost_object : public base_physics_object {
friend void destroy_ghost_object( bt_ghost_object* obj );
friend class survarium::booby_trap_set_core;

public:

	VOSTOK_PHYSICS_API bt_ghost_object( bt_collision_shape_ptr shape, btPairCachingGhostObject* obj );
	VOSTOK_PHYSICS_API ~bt_ghost_object( );

	VOSTOK_PHYSICS_API void		set_transform			( float4x4 const& transform );
	VOSTOK_PHYSICS_API float4x4	get_transform			( ) const;

	VOSTOK_PHYSICS_API void		insert					( world* w, u16 group, u16 mask );
	VOSTOK_PHYSICS_API void		remove					( world* w );

	VOSTOK_PHYSICS_API void		get_overlapping_objects		( base_physics_objects_type& result ) const;
	VOSTOK_PHYSICS_API u32		get_overlapping_objects_count	( ) const;

	VOSTOK_PHYSICS_API void		contact_test				( world* world, base_physics_object* object, contact_test_predicate& predicate );
	VOSTOK_PHYSICS_API bool		contact_test				( world* world );


	void		dbg_render								( world* world, math::color const& color ) const /* no source */;

	u32			non_compound_shapes_count				( ) const /* no source */;
	VOSTOK_PHYSICS_API void		non_compound_shapes_centers	( vectora<float3>& centres_results ) const;

	virtual VOSTOK_PHYSICS_API btCollisionObject*	get_bt_collision_obect	( )			override;
	virtual VOSTOK_PHYSICS_API u16					get_collision_group		( ) const	override;

	/* 0x0000 */	/* base_physics_object */
	/* 0x000c */	bt_collision_shape_ptr				m_shape;
private:
	/* 0x0010 */	btPairCachingGhostObject*			m_bt_object;
}; // class bt_ghost_object

STATIC_SIZE_ASSERT(bt_ghost_object, 0x14);

VOSTOK_PHYSICS_API	bt_ghost_object*	create_ghost_object		( bt_collision_shape_ptr shape, float4x4 const& transform );

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_BT_GHOST_OBJECT_H_INCLUDED
