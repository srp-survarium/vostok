////////////////////////////////////////////////////////////////////////////
//	Created		: 14.04.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include <vostok/collision/animated_object.h>
#include <vostok/collision/geometry.h>
#include <vostok/collision/shell_creation_utils.h>
#include "vostok/math_randoms_generator.h"
#include "composite_geometry.h"
#include "composite_geometry_instance.h"

#include <vostok/physics/bullet_utils.h>

namespace vostok {
namespace collision {

animated_object::animated_object(
		configs::binary_config_value const& config,
		animation::skeleton_ptr const& model_skeleton,
		u32 const bones_count,
		memory::stack_allocator& allocator
	) :
	m_geometries_data			( VOSTOK_MALLOC_IMPL( allocator, bones_count * sizeof( bone_collision_data ), "geometry instances vector" ), bones_count ),
	m_head_bone_index			( u32(-1) ),
	m_geometry					( NULL )
{
	btCompoundShape* bt_shape = physics::new_compound_shape_from_hit_targets_config( config, m_geometries_data, &allocator );
	
	m_body = physics::new_animated_rigid_body( bt_shape, 10, &allocator );

	u32 const non_root_bones_count				= model_skeleton->get_non_root_bones_count();
	R_ASSERT_CMP								( non_root_bones_count, <, model_skeleton->get_bones_count() );
	u32 const root_bones_count					= model_skeleton->get_bones_count() - non_root_bones_count;

	bone_collisions_type::iterator iter			= m_geometries_data.begin();
	bone_collisions_type::const_iterator end	= m_geometries_data.end();
	
	for ( ; iter != end; ++iter )
	{
		iter->skeleton_bone_index				= model_skeleton->get_bone_index( iter->bone_name.c_str() );
		R_ASSERT_CMP							( iter->skeleton_bone_index, >=, root_bones_count );
		iter->skeleton_bone_index				-= root_bones_count;
		if ( iter->bone_name == "Head" )
			m_head_bone_index					= std::distance( m_geometries_data.begin(), iter );
	}

	m_allocator.swap( allocator );
}

animated_object::~animated_object	( )
{
	physics::destroy_animated_rigid_body( m_body, &m_allocator );
}

void animated_object::destroy		( memory::base_allocator* allocator )
{
	if ( !m_geometry )
		return;

	composite_geometry* geometry	= static_cast_checked< composite_geometry* >( m_geometry );
	
	m_geometries_data.clear			( );
	bone_collision_data* begin_data	= m_geometries_data.begin();
	VOSTOK_FREE_IMPL					( allocator, begin_data );
	
	geometry_instance* const* begin	= geometry->begin(); geometry_instance* const* end = geometry->end();

	size_t const instances_count	= std::distance( begin, end );
	size_t const buffer_size		= instances_count * sizeof( geometry_instance* );
	
	buffer_vector< geometry_instance* >	instances( ALLOCA( buffer_size ), instances_count );
	for ( geometry_instance* const* iter = begin; iter != end; ++iter )
		instances.push_back			( *iter );

	geometry->destroy				( allocator );
	
	for ( u32 i = 0; i < instances_count; ++i )
		delete_geometry_instance	( allocator, instances[i] );
}

void animated_object::update	( float4x4 const* const bones_matrices_begin, float4x4 const* const bones_matrices_end )
{	
	VOSTOK_UNREFERENCED_PARAMETER( bones_matrices_end );
	for ( u32 i = 0; i < m_geometries_data.size()  ; ++i )
	{
		if ( m_geometry )
		{
			m_geometries_data[i].bone_geometry_instance->set_matrix( bones_matrices_begin[m_geometries_data[i].skeleton_bone_index] );
		}
		else
		{
			m_body->update_bone_matrix( i, bones_matrices_begin[m_geometries_data[i].skeleton_bone_index], i == m_geometries_data.size() - 1 );
		}
	}
}
math::aabb animated_object::get_aabb	( ) const
{
 	return m_geometry ? m_geometry->get_aabb( ) : m_body->get_aabb();
}

float3 animated_object::get_random_surface_point( u32 const current_time ) const
{
	if (!m_geometry) {
		return float3( 0.f, 0.f, 0.f );
	}

 	typedef buffer_vector< float >		bones_type;
	u32 const bones_count				= get_bones_count();
 	bones_type bones					( ALLOCA( sizeof( float ) * bones_count ), bones_count );

	for ( u32 i = 0; i < get_bones_count(); ++i )
	{
		composite_geometry_instance const* gi	= static_cast_checked< composite_geometry_instance const* >( &*m_geometries_data[i].bone_geometry_instance );
		non_null< composite_geometry const >::ptr collision_element = gi->get_geometry();
		float const surface_area		= collision_element->get_surface_area( );
		R_ASSERT_CMP					( surface_area, >, 0.f );
		bones.push_back					( surface_area + ( i > 0 ? bones[i - 1] : 0 ) );
	}

 	math::random32 random_number		( current_time );
	float const random_area				= random_number.random_f( bones.back() );
 	bones_type::iterator iter			= std::lower_bound( bones.begin(), bones.end(), random_area, std::less<float>() );
 	R_ASSERT							( iter != bones.end() );
	
	animation::bone_index_type bone_index = std::distance( bones.begin(), iter );
	
	// tried to avoid this, but :(
	composite_geometry_instance const* const cgi = static_cast_checked< composite_geometry_instance const* >( &*m_geometries_data[bone_index].bone_geometry_instance );
	non_null< composite_geometry const >::ptr collision_element = cgi->get_geometry();
	geometry_instance* const* bone_instance	= collision_element->begin();

	R_ASSERT								( *bone_instance );
	
	float3 const bone_coords				= (*bone_instance)->get_random_surface_point( random_number );
	float3 const collision_coords			= bone_coords * (*bone_instance)->get_matrix();
	return									collision_coords * m_geometries_data[bone_index].bone_geometry_instance->get_matrix();
}

float3 animated_object::get_head_bone_center	( ) const
{
	R_ASSERT									( m_head_bone_index != u32(-1) );
	if ( m_geometry )
	{
		const float4x4& matrix = m_geometries_data[m_head_bone_index].bone_geometry_instance->get_matrix();
		return matrix.c.xyz();
	}
	else 
	{
		const float4x4& matrix = m_body->get_bone_transform(m_head_bone_index);
		return matrix.c.xyz();
	}
}

float3 animated_object::get_eyes_direction( ) const
{
	R_ASSERT								( m_head_bone_index != u32(-1) );

	if ( m_geometry )	
	{
		composite_geometry_instance const* const cgi = static_cast_checked< composite_geometry_instance const* >( &*m_geometries_data[m_head_bone_index].bone_geometry_instance );
		non_null< composite_geometry const >::ptr collision_element	= cgi->get_geometry();
 		geometry_instance* const* bone_instance	= collision_element->begin();
		R_ASSERT								( *bone_instance );
		float4x4 const&	head_bone_transform			= (*bone_instance)->get_matrix();
		float4x4 const& head_collision_transform	= m_geometries_data[m_head_bone_index].bone_geometry_instance->get_matrix();

		float3 direction						= head_collision_transform.transform_direction( head_bone_transform.k.xyz() );
		return									direction.normalize();
	}
	else
	{
		float4x4 const& matrix = m_body->get_bone_transform(m_head_bone_index);
		float3 direction = matrix.k.xyz();
		return direction.normalize();
	}
}

pcstr animated_object::body_part_name(
	u32                                bone_index) const
{
	R_ASSERT ( bone_index < m_geometries_data.size() ); // sushi@NOTE: It was compiled out, not sure this one is correct
	return m_geometries_data[bone_index].body_part_name.c_str();
}


} // namespace collision
} // namespace vostok
