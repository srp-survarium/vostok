////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef COLLISION_GEOMETRY_H_INCLUDED
#define COLLISION_GEOMETRY_H_INCLUDED

#include <vostok/game_core/base_game_object.h>
#include <vostok/collision/game_object.h>

namespace vostok {
namespace physics {
	struct world; // sushi@TODO: Just import API instead of all of this?
	class base_physics_object;
	struct contact_test_predicate;
	class bt_ghost_object;

	typedef vostok::buffer_vector< vostok::physics::base_physics_object* > base_physics_objects_type; // sushi@TODO: Move?
}

namespace configs {
	class binary_config_value;
}
}

namespace survarium {

class collision_geometry_subscriber;


class collision_geometry : public base_game_object , public vostok::collision::game_object {
public:
	enum collision_geometry_mode
	{
		collision_geometry_mode_mesh_containment	= 0x0000,
		collision_geometry_mode_mesh_intersection	= 0x0001,
	};

public:
										collision_geometry				( );
	virtual								~collision_geometry				( );

			void						load							( configs::binary_config_value const& cfg_val );

	virtual	collision_geometry*			cast_to_collision_geometry		( ) override { return this; };

			void						subscribe						( physics::world* world, collision_geometry_subscriber* subscriber );
			void						unsubscribe						( collision_geometry_subscriber* subscriber );

			u32							get_overlapping_objects_count	( ) const;
			void						get_overlapping_objects			( physics::base_physics_objects_type& result ) const;

	template< typename object_type >
	inline	void						query_objects_by_type			(
											vectora< object_type* >& results,
											object_type* ( collision_geometry_subscriber::* functor_ptr )( )
										);

			bool						contact_test					( );
			void						contact_test					( physics::base_physics_object* object, physics::contact_test_predicate& predicate );

	inline	void						dbg_render						( math::color const& arg_0 ) const { /* no source */ }

			void						get_shapes_centers				( vectora<float3>& centers_results ) const;

	inline	physics::bt_ghost_object*	ghost_object					( ) { return m_ghost_object; }
			void						destroy_ghost_object			( );

	virtual	void						set_transform					( float4x4 const& transform );
	virtual	float4x4					get_transform					( );

private:
			void						insert							( physics::world* world );
			void						remove							( );

private:
	typedef std::vector< collision_geometry_subscriber* > collision_geometry_subscribers_type;

	/* 0x0004 */	physics::world*								m_physics_world;
	/* 0x0008 */	collision_geometry_subscribers_type			m_subscribers;
	/* 0x0014 */	fixed_string<260>							m_name;
	/* 0x0124 */	physics::bt_ghost_object*					m_ghost_object;
	/* 0x0128 */	collision_geometry::collision_geometry_mode	m_mode;
	/* 0x012c */	u16											m_group;
	/* 0x012e */	u16											m_mask;
}; // class collision_geometry

STATIC_SIZE_ASSERT(collision_geometry, 0x130);

} // namespace survarium

#include <vostok/game_core/collision_geometry_inline.h>

#endif // #ifndef COLLISION_GEOMETRY_H_INCLUDED
