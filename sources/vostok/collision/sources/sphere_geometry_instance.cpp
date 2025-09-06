////////////////////////////////////////////////////////////////////////////
//	Created		: 14.09.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sphere_geometry_instance.h"
#include <vostok/collision/primitives.h>
#include <vostok/collision/contact_info.h>
#include <vostok/render/facade/debug_renderer.h>
#include <vostok/math_randoms_generator.h>

namespace vostok {
namespace collision {

sphere_geometry_instance::sphere_geometry_instance	( float4x4 const& matrix ) : 
	m_matrix			( matrix )
{ }

sphere_geometry_instance::~sphere_geometry_instance	( )
{ }

bool sphere_geometry_instance::aabb_query			( object const* object, math::aabb const& aabb, triangles_type& triangles ) const
{
	VOSTOK_UNREFERENCED_PARAMETERS	(object, &aabb, &triangles);
	return false; 
}

bool sphere_geometry_instance::cuboid_query			( object const* object, math::cuboid const& cuboid, triangles_type& triangles ) const
{
	VOSTOK_UNREFERENCED_PARAMETERS	(object, &cuboid, &triangles);
	return false; 
}

bool sphere_geometry_instance::ray_query			(
	object const*					object,
	math::float3 const&				origin,
	math::float3 const&				direction,
	float							max_distance,
	float&							distance,
	ray_triangles_type&				triangles, 
	triangles_predicate_type const&	predicate ) const
{
	VOSTOK_UNREFERENCED_PARAMETERS	( object, &origin, &direction, max_distance, distance, &triangles, &predicate );

	if ( !ray_test( origin, direction, max_distance, distance ) )
		return false;

	ray_triangle_result		result( object, u32(-1), distance );
	predicate( result );
	triangles.push_back( result );

	return true;
}

bool sphere_geometry_instance::aabb_test			( math::aabb const& aabb ) const
{
	VOSTOK_UNREFERENCED_PARAMETER		( aabb );
	return true;
}

bool sphere_geometry_instance::cuboid_test			( math::cuboid const& cuboid ) const
{
	VOSTOK_UNREFERENCED_PARAMETER		( cuboid );
	return true;
}

bool sphere_geometry_instance::ray_test				( math::float3 const& origin, math::float3 const& direction, float max_distance, float& distance ) const 
{
	float3 const& to_sphere			= m_matrix.c.xyz( ) - origin;
	float const projection_squared_length	= math::sqr( to_sphere | direction );
	float const squared_distance	= to_sphere.squared_length( ) - projection_squared_length;
	float const squared_radius		= math::sqr(radius_unit());
	if ( squared_distance > squared_radius )
		return						false;

	distance						= math::max( math::sqrt( projection_squared_length ) - math::sqrt( squared_radius - squared_distance ), 0.f );
	return							distance <= max_distance;
}

math::aabb sphere_geometry_instance::get_aabb			( ) const
{
	math::aabb tmp				= math::create_aabb_center_radius	
								( 
									float3( 0.f, 0.f, 0.f ),
									float3( radius_unit(), radius_unit(), radius_unit() )
								);
	return						tmp.modify( m_matrix );
}

math::aabb sphere_geometry_instance::get_geometry_aabb	( ) const
{
	return						math::create_aabb_center_radius	
								( 
									float3( 0.f, 0.f, 0.f ),
									float3( radius_unit(), radius_unit(), radius_unit() )
								);
}

bool sphere_geometry_instance::is_valid					( ) const
{
	return						true;
}

void sphere_geometry_instance::render					( render::scene_ptr const& scene, render::debug::renderer& renderer ) const
{
	render		( scene, renderer, m_matrix );
}

void sphere_geometry_instance::render					( render::scene_ptr const& scene, render::debug::renderer& renderer, float4x4 const& transform ) const
{
	renderer.draw_sphere		( scene, math::create_translation(transform.c.xyz()), radius(), math::color( 255u, 255u, 255u, 255u ) );
}

void sphere_geometry_instance::render					( render::scene_ptr const& scene, render::debug::renderer& renderer, float4x4 const& transform, math::color const& color ) const
{
	renderer.draw_sphere_solid	( scene, transform.c.xyz(), radius(), color );
	renderer.draw_sphere		( scene, math::create_translation(transform.c.xyz()), radius(), math::color( 255u, 255u, 255u, 255u ) );
}

void sphere_geometry_instance::enumerate_primitives		( enumerate_primitives_callback& cb ) const
{
	cb.enumerate( float4x4().identity(), primitive( sphere( radius() ) ) );
}

void sphere_geometry_instance::enumerate_primitives		( float4x4 const& transform, enumerate_primitives_callback& cb ) const
{
	cb.enumerate( transform, primitive( sphere( radius() ) ) );
}

void sphere_geometry_instance::add_triangles		( triangles_type& triangles ) const
{
	VOSTOK_UNREFERENCED_PARAMETER		( triangles );
	UNREACHABLE_CODE				( );
}

math::float3 const* sphere_geometry_instance::vertices	( ) const
{
	NOT_IMPLEMENTED				( return 0 );
}

u32 sphere_geometry_instance::vertex_count				( ) const
{
	NOT_IMPLEMENTED				( return 0 );
}

u32 const* sphere_geometry_instance::indices			( ) const
{
	NOT_IMPLEMENTED				( return 0 );
}

u32 const* sphere_geometry_instance::indices			( u32 triangle_id ) const
{
	VOSTOK_UNREFERENCED_PARAMETER ( triangle_id );
	NOT_IMPLEMENTED				( return 0 );
}

u32 sphere_geometry_instance::index_count				( ) const
{
	NOT_IMPLEMENTED				( return 0 );
}

float sphere_geometry_instance::get_surface_area		( ) const
{
	return 4 * math::pi * math::sqr( radius() );
}

float3 sphere_geometry_instance::get_random_surface_point( math::random32& randomizer ) const
{
	float const z_random		= -radius() + randomizer.random_f( 2.f * radius() );
	float const teta_angle		= randomizer.random_f( math::pi_x2 );
	float const r_coefficient	= math::sqrt( math::sqr( radius() ) - math::sqr( z_random ) );
	float const x_random		= r_coefficient * math::cos( teta_angle );
 	float const y_random		= r_coefficient * math::sin( teta_angle );

	return						float3( x_random, y_random, z_random );
}

float3 sphere_geometry_instance::get_closest_point_to	( float3 const& point, float4x4 const& origin ) const
{
	float4x4 transform			= origin * m_matrix;
	float3 position				= transform.c.xyz();
	float3 direction			= point - position;

	if ( direction.squared_length( ) < radius_unit() )
		return point;

	float3 result				= direction.normalize();
	// result						*= m_radius; // * 1.0
	result						+= position;
	return						result;
}

} // namespace collision
} // namespace vostok