////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef COLLISION_SENSOR_H_INCLUDED
#define COLLISION_SENSOR_H_INCLUDED

namespace survarium {

// STATE[STUB]
// void survarium::collision_sensor::on_inside(vostok::buffer_vector<vostok::physics::base_physics_object *> const&)
void collision_sensor::on_inside( vostok::buffer_vector<vostok::physics::base_physics_object *> const& objects )
{
}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

	typedef
		scheduler::record*
		iterator_type;

	typedef
		vostok::collision::bone_collision_data const*
		iterator_type;

	typedef
		vostok::collision::bone_collision_data*
		iterator_type;

} // namespace survarium

#endif // #ifndef COLLISION_SENSOR_H_INCLUDED
