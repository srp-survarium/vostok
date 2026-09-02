// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/render/culling/portal_sector_structure.h>
#include <vostok/collision/api.h>
#include <vostok/collision/common_types.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/collision/geometry.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/math_cuboid.h>
#include <vostok/render/culling/collision_result_user_data_equalls_to.h>
#include <vostok/render/core/memory.h>

namespace vostok {
namespace render {
namespace culling {

portal_sector_structure::portal_sector_structure(
	memory::base_allocator* allocator,
	u32 sectors_count,
	u32 portals_count
)
	: m_allocator( g_allocator ),
	  m_portals_buffer( VOSTOK_MALLOC_IMPL( *m_allocator, portals_count * sizeof( portal ), "portals" ) ),
	  m_portals( m_portals_buffer, portals_count ),
	  m_portal_ids_buffer( 0 ),
	  m_sectors_buffer( VOSTOK_MALLOC_IMPL( *m_allocator, sectors_count * sizeof( spatial_sector ), "sectors" ) ),
	  m_sectors( m_sectors_buffer, sectors_count ),
	  m_sectors_spatial_tree( &*collision::new_space_partitioning_tree( m_allocator, 1.f, sectors_count * 100 ) ),
	  m_portals_spatial_tree( &*collision::new_space_partitioning_tree( m_allocator, 1.f, portals_count ) ),
	  m_portals_geometry( 0 )
{
}

portal_sector_structure::~portal_sector_structure( )
{
	collision::delete_space_partitioning_tree( m_portals_spatial_tree );
	collision::delete_space_partitioning_tree( m_sectors_spatial_tree );
	m_sectors.clear( );
	VOSTOK_FREE_IMPL( *m_allocator, m_sectors_buffer );
	m_portals.clear( );
	VOSTOK_FREE_IMPL( *m_allocator, m_portals_buffer );
	VOSTOK_FREE_IMPL( *m_allocator, m_portal_ids_buffer );
}

u32 portal_sector_structure::get_sector_id( memory::base_allocator& allocator, float3 const& pos ) const
{
	u32 active_sector_id = 0;

	math::aabb const bbox = math::create_aabb_center_radius( pos, float3( .1f, .1f, .1f ) );
	collision::triangles_type results( allocator );
	if ( m_sectors_spatial_tree->aabb_query( sectors_volume_object_type, bbox, results ) )
	{

		active_sector_id = reinterpret_cast<u32>( results[0].object->user_data( ) );
		if ( std::find_if( results.begin( ) + 1, results.end( ), std::not1( collision_result_user_data_equalls_to( active_sector_id ) ) ) != results.end( ) )
		{
			results.clear( );
			math::cuboid c( bbox, math::float4x4( ).identity( ) );
			if ( m_portals_geometry->cuboid_query( 0, c, results ) )
			{
				active_sector_id = results[0].triangle_id / 2;
				float min_dist = math::abs( m_portals[active_sector_id].get_plane( ).classify( pos ) );

				for ( collision::triangles_type::const_iterator i = results.begin( ) + 1, e = results.end( ); i != e; ++i )
				{
					u32 const portal_id = i->triangle_id / 2;
					if ( portal_id == active_sector_id )
						continue;
					float const dist = math::abs( m_portals[portal_id].get_plane( ).classify( pos ) );
					if ( min_dist > dist )
					{
						min_dist = dist;
						active_sector_id = portal_id;
					}
				}
				portal const& p = m_portals[active_sector_id];
				bool const is_front = p.get_plane( ).classify( pos ) > 0;
				return p.get_sectors( )[is_front ? 0 : 1];
			}
		}
	}
	return active_sector_id;
}

void portal_sector_structure::load( configs::binary_config_value* value_ptr )
{

	configs::binary_config_value portals_cfg = (*value_ptr)["portals"];
	m_portal_ids_buffer = static_cast<u32*>( VOSTOK_MALLOC_IMPL(
		*m_allocator, portals_cfg.size() * 2 * sizeof( u32 ), "portal ids" ) );
	u32* current_portal_ids = m_portal_ids_buffer;
	u32 sector_id = 0;
	math::float4x4 identity_matrix;

	identity_matrix.identity();
	configs::binary_config_value sectors_cfg = (*value_ptr)["sectors"];
	configs::binary_config_value const* const sectors_end = sectors_cfg.end();
	configs::binary_config_value const* sector_it = sectors_cfg.begin();
	for ( ; sector_it != sectors_end; ++sector_it, ++sector_id )
	{
		math::aabb sector_aabb = math::create_zero_aabb();
		bool sector_aabb_initialized = false;
		if ( sector_it->value_exists( "volumes" ) ) {
			configs::binary_config_value volumes_cfg = (*sector_it)["volumes"];
			configs::binary_config_value const* const volumes_end = volumes_cfg.end();
			for ( configs::binary_config_value const* volume_it = volumes_cfg.begin(); volume_it != volumes_end; ++volume_it )
			{
				float3 const& minimum = (*volume_it)[0];
				float3 const& maximum = (*volume_it)[1];
				math::aabb const volume_aabb = math::create_aabb_min_max( minimum, maximum );
				non_null<collision::object>::ptr object = collision::new_aabb_object( m_allocator, sectors_volume_object_type, volume_aabb.center(), volume_aabb.extents(), reinterpret_cast<pvoid>( sector_id ) );
				m_sectors_spatial_tree->insert( object, identity_matrix );
				if ( sector_aabb_initialized )
				{
					sector_aabb.modify( volume_aabb );
				}
				else
				{
					sector_aabb = volume_aabb;
					sector_aabb_initialized = true;
				}
			}
		}

		configs::binary_config_value portal_ids_cfg = (*sector_it)["portals"];
		u32* const sector_portal_ids = current_portal_ids;
		configs::binary_config_value const* const portal_ids_end = portal_ids_cfg.end();
		for ( configs::binary_config_value const* portal_id_it = portal_ids_cfg.begin(); portal_id_it != portal_ids_end; ++portal_id_it )
		{
			*current_portal_ids = static_cast<u32>( *portal_id_it );
			++current_portal_ids;
		}
		spatial_sector new_sector( sector_portal_ids, portal_ids_cfg.size(), sector_aabb );
		m_sectors.push_back( new_sector );
	}

	configs::binary_config_value const* const portals_cfg_end = portals_cfg.end();
	for ( configs::binary_config_value const* portal_it = portals_cfg.begin(); portal_it != portals_cfg_end; ++portal_it )
	{
		configs::binary_config_value points_cfg = (*portal_it)["points"];
		float3 v0 = points_cfg[0];
		float3 v1 = points_cfg[1];
		float3 v2 = points_cfg[2];
		float3 v3 = points_cfg[3];

		configs::binary_config_value sectors_cfg = (*portal_it)["sectors"];
		u32 const sector0 = sectors_cfg[0];
		u32 const sector1 = sectors_cfg[1];

		m_portals.push_back( portal( sector0, sector1, v0, v1, v2, v3 ) );

		math::aabb portal_aabb = math::create_aabb_min_max( v0, v0 ).modify( v1 ).modify( v2 ).modify( v3 );
		non_null<collision::object>::ptr object = collision::new_aabb_object(
			m_allocator, portal_object_type, portal_aabb.center(), portal_aabb.extents(), &m_portals.back()
		);
		m_portals_spatial_tree->insert( object, identity_matrix );
	}

	initialize_portals_geometry();
}

void portal_sector_structure::sort_portal_ids( float const* distances )
{
	spatial_sector* const sectors_end = m_sectors.end( );
	for ( spatial_sector* i = m_sectors.begin( ); i != sectors_end; ++i )
		i->sort_portal_ids( distances );
}

void portal_sector_structure::initialize_portals_geometry( )
{
	u32 const vertices_count = m_portals.size() * 4;
	buffer_vector<float3> vertices( ALLOCA( vertices_count * sizeof( float3 ) ), vertices_count );
	u32 const indices_count = m_portals.size() * 6;
	buffer_vector<u32> indices( ALLOCA( indices_count * sizeof( u32 ) ), indices_count );
	portals_type::const_iterator const portals_end = m_portals.end();
	for ( portals_type::const_iterator it = m_portals.begin(); it != portals_end; ++it )
	{
		u32 const vertex_index = vertices.size();
		vertices.insert( vertices.end(), it->get_points(), it->get_points() + 4 );
		indices.push_back( vertex_index );
		indices.push_back( vertex_index + 1 );
		indices.push_back( vertex_index + 2 );
		indices.push_back( vertex_index );
		indices.push_back( vertex_index + 2 );
		indices.push_back( vertex_index + 3 );
	}
	m_portals_geometry = &*collision::new_triangle_mesh_geometry(
		m_allocator,
		vertices.begin(),
		vertices_count,
		indices.begin(),
		indices_count
	);
}

void portal_sector_structure::update_portals_visability( math::frustum const& f, pcbyte oclusion_results )
{
	portal* const portals_end = m_portals.end( );
	for ( portal* i = m_portals.begin( ); i != portals_end; ++i )
	{
		i->set_visible( false );
	}
	collision::triangles_type triangles( *m_allocator );
	if ( m_portals_geometry->cuboid_query( 0, f, triangles ) )
	{

		collision::triangle_result const* const triangles_end = triangles.end( );
		for ( collision::triangle_result const* i = triangles.begin( ); i != triangles_end; ++i ) {
			u32 const portal_id = i->triangle_id / 2;
			m_portals[portal_id].set_visible( oclusion_results[portal_id] != 0 );
		}
	}
}

} // namespace culling
} // namespace render
} // namespace vostok
