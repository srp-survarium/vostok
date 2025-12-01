////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef COLLISION_SENSOR_H_INCLUDED
#define COLLISION_SENSOR_H_INCLUDED

#include <vostok/game_core/collision_geometry_subscriber.h>
#include <vostok/game_core/link_resolver.h>

namespace vostok {
namespace physics {
	struct world;
	class contact_test_predicate;
	class base_physics_object;
}
}

namespace survarium {

class base_project;
class collision_geometry;

class collision_sensor : public collision_geometry_subscriber , public link_resolver {
public:
									collision_sensor				( );
	virtual							~collision_sensor				( );

	virtual	void					resolve_links					( base_project* p, vostok::configs::binary_config_value cfg ) override;
	virtual	bool					is_filter_passed				( vostok::physics::base_physics_object* object ) const;

	virtual	void					tick							( u32 time_delta_ms, u32 current_time_ms );
	virtual	void					load							( vostok::configs::binary_config_value const& cfg );

	inline	bool					is_active						( ) const { /* no source */ }

			bool					contact_test					( vostok::physics::base_physics_object* __formal );
			void					contact_test					( vostok::physics::base_physics_object* object, vostok::physics::contact_test_predicate& predicate );

	inline	void					dbg_render						( math::color const& arg_0 ) const { /* no source */ }

			void					insert							( vostok::physics::world* world );
			void					remove							( );

			collision_geometry*		get_collision_geometry			( u32 index );
	inline	u32						collision_geometries_count		( ) const { return m_collision_geometries_count; }

	virtual	void					on_inside						( vostok::buffer_vector<vostok::physics::base_physics_object *> const& objects )	{ }
	virtual	void					on_leave						( vostok::buffer_vector<vostok::physics::base_physics_object *> const& objects )	{ }
	virtual	void					on_enter						( vostok::buffer_vector<vostok::physics::base_physics_object *> const& objects )	{ }
	virtual	void					on_objetcs_loosed				( vector<vostok::physics::base_physics_object *> const& objects )					{ }

			void					get_shapes_centers				( vostok::vectora<float3>& centers ) const;

			void					notify_and_erase_left_objects	( vostok::buffer_vector<vostok::physics::base_physics_object *>& sensed_objects );
			void					notify_objects_inside			( );
			void					notify_and_add_incoming_objects	( vostok::buffer_vector<vostok::physics::base_physics_object *>& sensed_objects );

			void					filter_sensed_objects			( vostok::buffer_vector<vostok::physics::base_physics_object *>& sensed_objects );

private:
	/* 0x0008 */	vector<vostok::physics::base_physics_object *>	m_old_objects;
	/* 0x0014 */	collision_geometry**							m_collision_geometries;
	/* 0x0018 */	u32												m_collision_geometries_count;
	/* 0x001c */	bool											m_is_active;
}; // class collision_sensor

STATIC_SIZE_ASSERT(collision_sensor, 0x20);

} // namespace survarium

#endif // #ifndef COLLISION_SENSOR_H_INCLUDED
