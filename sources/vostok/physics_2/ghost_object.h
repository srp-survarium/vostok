////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.09.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_2_BT_GHOST_OBJECT_H_INCLUDED
#define VOSTOK_PHYSICS_2_BT_GHOST_OBJECT_H_INCLUDED

#include <vostok/physics_2/base_physics_object.h>
#include <vostok/physics_2/collision_shapes.h>
#include <vostok/physics_2/contact_test_predicate.h>

class btPairCachingGhostObject;

namespace vostok {
namespace physics_2 {

typedef buffer_vector<base_physics_object *> base_physics_objects_type; // sushi@TODO: Find proper place for the typedef

class bt_ghost_object : base_physics_object {
public:

	bt_ghost_object( bt_collision_shape_ptr shape, btPairCachingGhostObject* obj );
	~bt_ghost_object( );

	void		set_transform							( float4x4 const& transform );
	float4x4	get_transform							( ) const;

	void		insert									( world* w, u16 group, u16 mask );
	void		remove									( world* w );

	void		get_overlapping_objects					( base_physics_objects_type& result ) const;
	u32			get_overlapping_objects_count			( ) const;

	bool		contact_test							( world* world );
	void		contact_test							( world* world, base_physics_object* object, contact_test_predicate& predicate );


	void		dbg_render								( world* world, math::color const& color ) const /* no source */;

	u32			non_compound_shapes_count				( ) const /* no source */;
	void		non_compound_shapes_centers				( vectora<float3>& centres_results );


	virtual btCollisionObject*	get_bt_collision_obect	( )			override;
	virtual u16					get_collision_group		( ) const	override;

private:
	/* offset 0x0000 */ /* fields for base_physics_object */
	/* offset 0x000c */ bt_collision_shape_ptr              m_shape;
	/* offset 0x0010 */ btPairCachingGhostObject*           m_bt_object;
}; // class bt_ghost_object

namespace {
	typedef char size_assert[
		sizeof(bt_ghost_object) == 0x14 ? 1 : -1
	];
}

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_2_BT_GHOST_OBJECT_H_INCLUDED