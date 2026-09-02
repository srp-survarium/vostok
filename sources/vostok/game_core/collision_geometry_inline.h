// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////
#ifndef COLLISION_GEOMETRY_INLINE_H_INCLUDED
#define COLLISION_GEOMETRY_INLINE_H_INCLUDED

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

} // namespace survarium

#endif // #ifndef COLLISION_GEOMETRY_INLINE_H_INCLUDED
