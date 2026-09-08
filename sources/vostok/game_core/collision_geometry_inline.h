// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef COLLISION_GEOMETRY_INLINE_H_INCLUDED
#define COLLISION_GEOMETRY_INLINE_H_INCLUDED

#include <vostok/physics/ghost_object.h>

namespace survarium {

template<typename object_type>
inline void collision_geometry::query_objects_by_type(
	vostok::vectora<object_type *>&					results,
	object_type* ( collision_geometry_subscriber::* functor_ptr )( )
)
{
	for ( u32 i = 0; i < m_subscribers.size( ); ++i )
	{
		object_type* const result = ( m_subscribers[i]->*functor_ptr )( );
		if ( result )
			results.push_back( result );
	}
}

inline void collision_geometry::dbg_render( math::color const& color ) const
{
	// sushi@TODO: Verify the ghost/world forwarding model, initialization guards and original assertion policy.
	m_ghost_object->dbg_render( m_physics_world, color );
}

} // namespace survarium

#endif // #ifndef COLLISION_GEOMETRY_INLINE_H_INCLUDED
