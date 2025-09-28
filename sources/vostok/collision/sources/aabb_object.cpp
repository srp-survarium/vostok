////////////////////////////////////////////////////////////////////////////
//	Created		: 29.04.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "aabb_object.h"
#include <vostok/render/facade/debug_renderer.h>
#include "colliders_ray_aabb.h"

namespace vostok {
namespace collision {

using namespace math;

aabb_object::aabb_object	(
		object_type const object_type,
		aabb const& aabb,
		pvoid const user_data
	)// :
//	m_local_aabb					( aabb )
{
	set_type						( object_type );
	m_user_data						= user_data;

	m_aabb							= aabb;
}

aabb_object::~aabb_object( )
{
}

math::aabb aabb_object::update_aabb( float4x4 const& local_to_world )
{
	return m_aabb = m_aabb.modify( local_to_world );
}

void aabb_object::render	( render::scene_ptr const& scene, render::debug::renderer& renderer ) const
{
	renderer.draw_aabb				( scene, m_aabb.center(), m_aabb.extents(), color( 255u, 255u, 255u, 255u ) );
}

bool aabb_object::aabb_query		( math::aabb const& aabb, triangles_type& triangles ) const
{
	if ( m_aabb.intersect( aabb ) )									// if ( aabb_test( aabb ) )
	{
		triangles.push_back( triangle_result( this, u32(-1) ) );	// add_triangles( triangles );
		return true;
	}

	return false;
}

bool aabb_object::cuboid_query		( math::cuboid const& cuboid, triangles_type& triangles ) const
{
	if ( cuboid.test_inexact( m_aabb ) != intersection_outside )	// if ( cuboid_test( cuboid ) ) // though no?
	{
		triangles.push_back( triangle_result( this, u32(-1) ) );	// add_triangles( triangles );
		return true;
	}

	return false;
}

bool aabb_object::ray_query			(
		math::float3 const& origin,
		math::float3 const& direction,
		float max_distance,
		float& distance,
		ray_triangles_type& triangles,
		triangles_predicate_type const& predicate
	) const
{
	VOSTOK_UNREFERENCED_PARAMETERS	( &origin, &direction, max_distance, distance, &triangles, &predicate );
	return true;
}

bool aabb_object::aabb_test			( math::aabb const& aabb ) const
{
	return							m_aabb.intersect( aabb );
}

bool aabb_object::cuboid_test		( math::cuboid const& cuboid ) const
{
	intersection result				= cuboid.test_inexact( m_aabb );
	return							result == intersection_inside || result == intersection_intersect;
}

bool aabb_object::ray_test				(
		math::float3 const& origin,
		math::float3 const& direction,
		float max_distance,
		float& distance
	) const
{
	colliders::sse::aabb_a16			aabb;
	colliders::sse::construct_aabb_a16	( aabb, m_aabb.center(), m_aabb.extents() );
	bool result							= colliders::ray_aabb_collider( origin, direction ).intersects_aabb_sse( aabb, distance );
	result								= result && (distance <= max_distance);
	return								result;
}

void aabb_object::add_triangles		( triangles_type& triangles ) const
{
	triangles.push_back( triangle_result( this, u32(-1) ) );
}

} // namespace collision
} // namespace vostok
