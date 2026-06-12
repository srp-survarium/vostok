////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FIND_CLOSEST_COLLISION_PREDICATE_H_INCLUDED
#define FIND_CLOSEST_COLLISION_PREDICATE_H_INCLUDED

/* INCLUDES */
struct vostok::physics::closest_ray_result;

namespace survarium {

struct find_closest_collision_predicate {
	inline			find_closest_collision_predicate( ) { /* no source */ }

	inline	void	operator()						( physics::closest_ray_result const& arg_0 ) { /* no source */ }

public:
	/* 0x0000 */	physics::closest_ray_result const*		m_result;
	/* 0x0004 */	float									m_closest_fraction;
}; // struct find_closest_collision_predicate

STATIC_SIZE_ASSERT(find_closest_collision_predicate, 0x8);

} // namespace survarium

#endif // #ifndef FIND_CLOSEST_COLLISION_PREDICATE_H_INCLUDED
