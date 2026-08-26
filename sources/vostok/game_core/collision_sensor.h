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
	struct contact_test_predicate;
	class base_physics_object;
}
}

namespace survarium {

class base_project;
class collision_geometry;
// sushi@NOTE: They forgot to implement cast for collision_sensor?
class collision_sensor : public collision_geometry_subscriber , public link_resolver {
public:
									collision_sensor				( );
	virtual							~collision_sensor				( );

	virtual	void					resolve_links					( base_project* p, configs::binary_config_value cfg ) override;
	virtual	bool					is_filter_passed				( physics::base_physics_object* object ) const;

	virtual	void					tick							( const u32 time_delta_ms, const u32 current_time_ms );
	virtual	void					load							( configs::binary_config_value const& cfg );

	inline	bool					is_active						( ) const { /* no source */ }

			void					contact_test					( physics::base_physics_object* object, physics::contact_test_predicate& predicate );
			bool					contact_test					( physics::base_physics_object* __formal );

	inline	void					dbg_render						( math::color const& arg_0 ) const { /* no source */ }

			void					insert							( physics::world* world );
			void					remove							( );

			collision_geometry*		get_collision_geometry			( u32 index );
	inline	u32						collision_geometries_count		( ) const { return m_collision_geometries_count; }

	typedef physics::base_physics_object* base_physics_object_ptr;
	typedef buffer_vector< base_physics_object_ptr > local_container;
	typedef vector< base_physics_object_ptr > objects_container;
	typedef vector< collision_geometry* > collision_geometries;

protected:	// claude@MATCH: target mangles these four overrides `MAE` (protected), not `UAE` - must be protected: to pair.
	virtual	void					on_inside						( buffer_vector< physics::base_physics_object* > const& objects )	{ VOSTOK_UNREFERENCED_PARAMETER( objects ); }
	virtual	void					on_leave						( buffer_vector< physics::base_physics_object* > const& objects )	{ VOSTOK_UNREFERENCED_PARAMETER( objects ); }
	virtual	void					on_enter						( buffer_vector< physics::base_physics_object* > const& objects )	{ VOSTOK_UNREFERENCED_PARAMETER( objects ); }
	virtual	void					on_objetcs_loosed				( vector< physics::base_physics_object* > const& objects )	{ VOSTOK_UNREFERENCED_PARAMETER( objects ); }

protected:
			void					get_shapes_centers				( vectora<float3>& centers ) const;

private:
			void					notify_and_erase_left_objects	( buffer_vector< physics::base_physics_object* >& sensed_objects );
			void					notify_objects_inside			( );
			void					notify_and_add_incoming_objects	( buffer_vector< physics::base_physics_object* >& sensed_objects );

			void					filter_sensed_objects			( buffer_vector< physics::base_physics_object* >& sensed_objects );

private:
	/* 0x0008 */	objects_container						m_old_objects;
protected:
	/* 0x0014 */	collision_geometry**					m_collision_geometries;
	/* 0x0018 */	u32										m_collision_geometries_count;
	/* 0x001c */	bool									m_is_active;
}; // class collision_sensor

STATIC_SIZE_ASSERT(collision_sensor, 0x20);

} // namespace survarium

#endif // #ifndef COLLISION_SENSOR_H_INCLUDED
