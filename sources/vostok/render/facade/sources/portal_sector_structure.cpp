#include "pch.h"
#include <vostok/render/culling/portal_sector_structure.h>

#include <vostok/collision/common_types.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/collision/geometry.h>

namespace vostok {
namespace render {
namespace culling {

bool get_first( collision::ray_triangle_result const& ) { return true; }

// claude@NOTE: legacy body diverged - legacy model_manager is a quasi_singleton without allocator/buffer_vector arenas; matcher-phase work.
// STATE[STUB]
portal_sector_structure::portal_sector_structure(
	memory::base_allocator* allocator,
	u32,
	u32
)
	: m_allocator( allocator ),
	  m_portals_buffer( 0 ),
	  m_portals( 0, 0 ),
	  m_portal_ids_buffer( 0 ),
	  m_sectors_buffer( 0 ),
	  m_sectors( 0, 0 ),
	  m_sectors_spatial_tree( 0 ),
	  m_portals_spatial_tree( 0 ),
	  m_portals_geometry( 0 )
{
	// FUNCTION BODY[0x75e9c0]
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

// claude@NOTE: legacy body diverged - legacy load_sectors reads chunked b_portal records via memory::chunk_reader, not a binary_config_value; matcher-phase work.
// STATE[STUB]
void portal_sector_structure::load( configs::binary_config_value* )
{
	// FUNCTION BODY[0x75ebf0]
}

// claude@NOTE: no legacy ancestor - absent from the held dx9/model_manager.cpp reference; matcher-phase work.
// STATE[STUB]
void portal_sector_structure::sort_portal_ids( float const* )
{
	// FUNCTION BODY[0x75e5d0]
}

// STATE[STUB]
// claude@NOTE: legacy ancestor idiom (dx9/model_manager.cpp load_sectors
// tail): fan-triangulate each portal's points
// (v[0], v[j-1], v[j] with the portal id as per-triangle custom data via
// collector::add_face_packed_d) then collision::create_triangle_mesh_geometry
// over the collected vertices/indices/tri data -> m_portals_geometry; the
// canonical portals are fixed 4-point quads. Matcher-phase against 0x75ea80.
void portal_sector_structure::initialize_portals_geometry( )
{
	// FUNCTION BODY[0x75ea80]
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
