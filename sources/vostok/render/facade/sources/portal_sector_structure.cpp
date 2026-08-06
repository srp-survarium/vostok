#include "pch.h"
#include <vostok/render/culling/portal_sector_structure.h>

#include <vostok/collision/api.h>
#include <vostok/collision/common_types.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/collision/geometry.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/render/core/memory.h>

namespace vostok {
namespace render {
namespace culling {

bool get_first( collision::ray_triangle_result const& ) { return true; }

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

// claude@NOTE: legacy body diverged - legacy model_manager dtor deletes heap arrays and D3D9 buffers; only the collision::destroy tail maps; matcher-phase work.
// STATE[STUB]
portal_sector_structure::~portal_sector_structure( )
{
	// FUNCTION BODY[0x75e8c0]
}

// claude@NOTE: ported from the ONLY surviving ancestor,
// dx9/model_manager.cpp model_manager::detect_sector(view_point) +
// detect_sector(view_point, dir), adapted to the canonical id-based
// structure: the down-then-up two-ray wrapper is folded into the single
// canonical method; sector*-returning get_sector_facing is inlined as the
// legacy header defined it (plane.classify(v)>0 ? face : back, face =
// sector_front = m_sectors[0]); the static-geometry branch was already
// disabled in the legacy body (id2 stays -1); query mask adapted from the
// legacy object_type_geometry to the canonical portal_object_type; not-found
// keeps the carcass return 0. Verify against 0x75e620 at matcher phase.
u32 portal_sector_structure::get_sector_id( memory::base_allocator& allocator, float3 const& pos ) const
{
	// FUNCTION BODY[0x75e620]
	float3	dir( 0.f, -1.f, 0.f );

	for ( u32 attempt = 0; attempt < 2; ++attempt, dir = float3( 0.f, 1.f, 0.f ) )
	{
		collision::ray_triangles_type	results( &allocator );

		// Portals model
		int		id1		= -1;
		float	range1	= 500.f;
		if ( m_portals_spatial_tree )
		{
			m_portals_spatial_tree->ray_query( portal_object_type, pos, dir, range1, results, collision::triangles_predicate_type( &get_first ) );

			if ( !results.empty( ) )
				id1 = results[0].triangle_id;
		}

		if ( id1 < 0 )
			continue;

		// Take sector, facing to our point from portal
		u32 const portal_id = m_portals_geometry->get_custom_data( id1 );
		portal const& p = m_portals[portal_id];
		return p.get_plane( ).classify( pos ) > 0 ? p.get_sectors( )[0] : p.get_sectors( )[1];
	}

	return 0;
}

void portal_sector_structure::load( configs::binary_config_value* value_ptr )
{
	configs::binary_config_value portals_cfg = (*value_ptr)["portals"];
	m_portal_ids_buffer = static_cast<u32*>( VOSTOK_MALLOC_IMPL(
		*m_allocator,
		portals_cfg.size() * 2 * sizeof( u32 ),
		"portal ids"
	) );
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

		if ( sector_it->value_exists( "volumes" ) )
		{
			configs::binary_config_value volumes_cfg = (*sector_it)["volumes"];
			configs::binary_config_value const* const volumes_end = volumes_cfg.end();
			for (
				configs::binary_config_value const* volume_it = volumes_cfg.begin();
				volume_it != volumes_end;
				++volume_it
			)
			{
				float3 const& minimum = (*volume_it)[0];
				float3 const& maximum = (*volume_it)[1];
				math::aabb const volume_aabb = math::create_aabb_min_max( minimum, maximum );

				non_null<collision::object>::ptr object = collision::new_aabb_object(
					m_allocator,
					sectors_volume_object_type,
					volume_aabb.center(),
					volume_aabb.extents(),
					reinterpret_cast<pvoid>( sector_id )
				);
				m_sectors_spatial_tree->insert( object, identity_matrix );

				if ( sector_aabb_initialized )
					sector_aabb.modify( volume_aabb );
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
		for (
			configs::binary_config_value const* portal_id_it = portal_ids_cfg.begin();
			portal_id_it != portal_ids_end;
			++portal_id_it, ++current_portal_ids
		)
			*current_portal_ids = static_cast<u32>( *portal_id_it );

		spatial_sector new_sector( sector_portal_ids, portal_ids_cfg.size(), sector_aabb );
		m_sectors.push_back( new_sector );
	}

	configs::binary_config_value const* const portals_cfg_end = portals_cfg.end();
	for (
		configs::binary_config_value const* portal_it = portals_cfg.begin();
		portal_it != portals_cfg_end;
		++portal_it
	)
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
			m_allocator,
			portal_object_type,
			portal_aabb.center(),
			portal_aabb.extents(),
			&m_portals.back()
		);
		m_portals_spatial_tree->insert( object, identity_matrix );
	}

	initialize_portals_geometry();
}

// claude@NOTE: no legacy ancestor - absent from the held dx9/model_manager.cpp reference; matcher-phase work.
// STATE[STUB]
void portal_sector_structure::sort_portal_ids( float const* )
{
	// FUNCTION BODY[0x75e5d0]
}

void portal_sector_structure::initialize_portals_geometry( )
{
	u32 const vertices_count = m_portals.size() * 4;
	buffer_vector<float3> vertices( ALLOCA( vertices_count * sizeof( float3 ) ), vertices_count );
	u32 const indices_count = m_portals.size() * 6;
	buffer_vector<u32> indices( ALLOCA( indices_count * sizeof( u32 ) ), indices_count );

	portal const* const portals_end = m_portals.end();
	for ( portal const* it = m_portals.begin(); it != portals_end; ++it )
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

// claude@NOTE: no legacy ancestor - absent from the held dx9/model_manager.cpp reference (portal marker/dual_render flags are the retired equivalent); matcher-phase work.
// STATE[STUB]
void portal_sector_structure::update_portals_visability( math::frustum const&, pcbyte )
{
	// FUNCTION BODY[0x75e520]
}

} // namespace culling
} // namespace render
} // namespace vostok
