#include "pch.h"
#include "portal_sector_system.h"

#include <vostok/collision/common_types.h>
#include <vostok/fixed_vector.h>
#include <vostok/math_frustum.h>
#include <vostok/math_plane.h>
#include <vostok/math_sphere.h>

#include <vostok/console_command.h>
#include <vostok/math_color.h>

#include "sector_double_query_preventer.h"
#include "system_renderer.h"

static bool s_use_screeen_space_portals_intersection_value = false;
static vostok::console_commands::cc_bool s_use_screeen_space_portals_intersection_cc(
	"r_use_ss_portals_intersection",
	s_use_screeen_space_portals_intersection_value,
	false,
	vostok::console_commands::command_type_engine_internal
);

static bool s_portals_occlusion_culling_value = false;
static vostok::console_commands::cc_bool s_portals_occlusion_culling_cc(
	"r_portals_occlusion_culling",
	s_portals_occlusion_culling_value,
	false,
	vostok::console_commands::command_type_engine_internal
);

static bool s_draw_portals_value = false;
static vostok::console_commands::cc_bool s_draw_portals_cc(
	"r_draw_portals",
	s_draw_portals_value,
	false,
	vostok::console_commands::command_type_engine_internal
);

static bool s_draw_draw_frustum_images_value = false;
static vostok::console_commands::cc_bool s_draw_draw_frustum_images_cc(
	"r_draw_frustum_images",
	s_draw_draw_frustum_images_value,
	false,
	vostok::console_commands::command_type_engine_internal
);

namespace vostok {
namespace render {
namespace culling {

struct collision_result_user_data_equalls_to :
	public std::unary_function< collision::triangle_result const&, bool >
{
	explicit collision_result_user_data_equalls_to( u32 user_data ) :
		m_user_data( user_data )
	{
	}

	bool operator()( collision::triangle_result const& ) const
	{
		return false;
	}

private:
	u32 m_user_data;
};

struct portal_id_closer_to_point : public std::binary_function< u32, u32, bool > {
	explicit portal_id_closer_to_point( float const* distances ) :
		m_distances( distances )
	{
	}

	bool operator()( u32 left, u32 right ) const
	{
		return m_distances[left] < m_distances[right];
	}

private:
	float const* m_distances;
};

struct portal_object_belongs_to_sector :
	public std::unary_function< collision::triangle_result const&, bool >
{
	explicit portal_object_belongs_to_sector( u32 sector_id ) :
		m_sector_id( sector_id )
	{
	}

	bool operator()( collision::triangle_result const& ) const
	{
		return false;
	}

private:
	u32 m_sector_id;
};

struct portal_object_closer_to_point :
	public std::binary_function<
		collision::triangle_result const&,
		collision::triangle_result const&,
		bool
	>
{
	explicit portal_object_closer_to_point( float3 const& point ) :
		m_point( point )
	{
	}

	bool operator()(
		collision::triangle_result const&,
		collision::triangle_result const&
	) const
	{
		return false;
	}

private:
	float3 m_point;
};

portal_sector_system::portal_sector_system( portal_sector_structure_ptr structure ) :
	m_structure					( structure ),
	m_test_action				( false ),
	m_preventer					( 0 ),
	m_occlusion_bounds_buffer		( 0 ),
	m_occlusion_bounds				( 0, 0 ),
	m_occlusion_results_buffer		( 0 ),
	m_occlusion_results			( 0, 0 ),
	m_portals_geometry				( 0 )
{
	// claude@NOTE: legacy body diverged - legacy model_manager ctor inits view point/marker/D3D9 collision handles, not the canonical preventer/occlusion buffers; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5fc180]
}

portal_sector_system::~portal_sector_system( )
{
	// claude@NOTE: legacy body diverged - legacy model_manager dtor releases D3D9 VB/IB and heap sector/portal arrays; only the collision teardown maps; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5fc080]
}

// claude@NOTE: ported from dx9/model_manager.cpp create_frustum_from_portal,
// adapted from the legacy n-point polygon + convex_volume to the canonical
// fixed 4 points + math::frustum: the poly>6 aabb-simplify pass is moot for a
// fixed quad; the far plane comes in as a parameter instead of being rebuilt
// from the view-projection rows; plane order follows the math::frustum matrix
// ctor (sides 0-3, far 4, near 5 - legacy wrote near,far,sides) so
// planes( )[4] stays the far plane on both creation paths; the winding-fix
// classify uses the canonical inward-looking plane convention (the legacy
// volume planes looked outward, and its uninitialized plane p is a
// transcription artifact of the near plane); the out-of-range poly[4] edge of
// the legacy side loop wraps to poly[0]. Verify against 0x5fb460 at matcher
// phase.
math::frustum create_frustum_from_four_points(
	float3 const& view_pos,
	float3 const (&points)[4],
	math::plane const& far_plane
)
{
	// FUNCTION BODY[0x5fb460]
	float3	poly[4];
	std::copy( &points[0], &points[4], &poly[0] );

	// check plane orientation relative to viewer and reverse if needed
	if ( math::create_plane_precise( poly[0], poly[1], poly[2] ).classify( view_pos ) > 0 )
		std::reverse( &poly[0], &poly[4] );

	math::plane	clip_planes[math::cuboid::plane_count];

	// side clip planes through the view point and consecutive poly edges
	for ( u32 i = 0; i < 4; ++i )
		clip_planes[i]	= math::create_plane_precise( view_pos, poly[i], poly[( i + 1 ) % 4] );

	// far clip plane
	clip_planes[4]	= far_plane;

	// near clip plane
	clip_planes[5]	= math::create_plane_precise( poly[0], poly[1], poly[2] );

	math::cuboid const result( clip_planes );
	return static_cast< math::frustum const& >( result );
}

// claude@NOTE: ported from dx9/model_manager.cpp clip_polygon_against_volume
// (the swap-src/dest per-plane Sutherland-Hodgman clip), adapted from the
// polygon-returning legacy to the canonical in-place float3(&)[4] + bool:
// iterates the frustum's fixed 6 planes (the legacy i<src.size() loop bound
// is a transcription artifact of the plane loop); the inside test flips to
// classify>0 for the canonical inward-looking planes (legacy volumes looked
// outward); the legacy dest.back()=...;push_back(float3()) pair is the
// X-Ray last()/inc() idiom, i.e. a push_back of the intersection point; the
// clipped polygon is written back over the 4 points - extra vertices are
// dropped and a short result pads with its last vertex. Verify against
// 0x5fa5d0 at matcher phase.
bool cull_points_by_frustum( math::frustum const& f, float3 (&points)[4] )
{
	// FUNCTION BODY[0x5fa5d0]
	enum { max_points = 48 };
	typedef fixed_vector< float3, max_points >	polygon;

	polygon	src;
	polygon	dest( &points[0], &points[4] );

	for ( u32 i = 0; i < math::cuboid::plane_count; ++i )
	{
		// cache plane and swap lists
		math::plane const& p = f.planes( )[i].plane;
		std::swap( src, dest );
		dest.clear( );

		// classify all points relative to plane #i
		float	cls[max_points];
		for ( u32 j = 0; j < src.size( ); j++ )
			cls[j] = p.classify( src[j] );

		// clip everything against this plane
		cls[src.size( )] = cls[0];
		src.push_back( src[0] );
		float3 d; float denum, t;

		for ( u32 j = 0; j < src.size( ) - 1; ++j )
		{
			if ( src[j].is_similar( src[j + 1], math::epsilon_7 ) )
				continue;

			bool intersect = cls[j] * cls[j + 1] < 0;

			if ( cls[j] > 0 )
			{
				dest.push_back( src[j] );
			}

			if ( intersect )
			{
				// segment intersects plane
				d		= src[j + 1] - src[j];
				denum	= p.normal.dot_product( d );

				if ( denum != 0 )
				{
					t	= -cls[j] / denum;
					dest.push_back( src[j] + d * t );
				}
			}
		}

		// here we end up with complete polygon in 'dest' which is inside plane #i
		if ( dest.size( ) < 3 )
		{
			dest.clear( );
			break;
		}
	}

	if ( dest.size( ) < 3 )
		return false;

	for ( u32 i = 0; i < 4; ++i )
		points[i] = dest[i < dest.size( ) ? i : dest.size( ) - 1];

	return true;
}

// claude@NOTE: the two switch-guarded statements below are dropped by the optimizer
// because both callees are still empty: sector_double_query_preventer::render
// (sector_double_query_preventer.cpp, 0x5fc/13 stmts) and draw_portals (below,
// 0x153/14 stmts). Bodying either restores its guard.
void portal_sector_system::render( system_renderer& renderer, float3 const& view_pos, float4x4 const& )
{
	u32 const sector_id				= m_structure->get_sector_id( *g_allocator, view_pos );
	if ( sector_id )
		renderer.draw_aabb( m_structure->get_sectors( )[sector_id].get_aabb( ), math::color( 0xff0000ff ) );

	if ( s_draw_draw_frustum_images_value )
		m_preventer->render( renderer );

	if ( s_draw_portals_value )
		draw_portals( renderer, sector_id );
}

void portal_sector_system::test_action( )
{
	m_test_action			= true;
}

void portal_sector_system::draw_quads( system_renderer& )
{
	// claude@NOTE: no legacy ancestor - absent from the held dx9/model_manager.cpp reference; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5fc300]
}

void portal_sector_system::draw_portals( system_renderer&, u32 const )
{
	// claude@NOTE: no legacy ancestor - absent from the held dx9/model_manager.cpp reference; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5fc3f0]
}

// claude@NOTE: ported from dx9/model_manager.cpp
// select_visuals(sector*, float4x4, vector<render_visual*>&) (the float3
// overload is #if1-dead): the legacy m_visuals guard/selection swap and the
// add_visual_to_selection recursion over render_visuals are replaced by the
// canonical flow - locate the start sector (get_sector_id, replacing the
// legacy update/detect_sector), reset the preventer (the ++m_marker analog),
// recurse the portal graph (traverse -> process_sector), then gather
// render_surface_instances via perform_frustum_culling_and_sectors_test (the
// add_visual_to_selection analog); the legacy start-sector null check has no
// id-space analog (get_sector_id returns 0 on not-found). Verify against
// 0x5fbd80 at matcher phase.
void portal_sector_system::select_models(
	collision::space_partitioning_tree* tree,
	float3 const& view_pos,
	float4x4 const& mat_vp,
	vector<render_surface_instance*>& selection
)
{
	// FUNCTION BODY[0x5fbd80]
	math::frustum const frustum( mat_vp );

	u32 const sector_id				= m_structure->get_sector_id( *g_allocator, view_pos );

	m_preventer->clear( );
	process_sector( sector_id, u32(-1), view_pos, frustum );

	perform_frustum_culling_and_sectors_test( tree, sector_id, frustum, selection, view_pos, mat_vp );
}

// claude@NOTE: ported from the sector half of dx9/model_manager.cpp
// model_manager::traverse(sector*, convex_volume): the legacy
// marker/m_selection/view_volumes bookkeeping maps to
// sector_double_query_preventer::add_frustum, and the portal loop walks the
// canonical id arrays, skipping the portal we entered through
// (input_portal_id) instead of the legacy per-portal marker stamp; the
// per-portal loop body lives in process_portal_by_frustum_intersection
// below. Verify against 0x5f9110 at matcher phase.
void portal_sector_system::process_sector( u32 sector_id, u32 input_portal_id, float3 const& view_pos, math::frustum const& frustum )
{
	// FUNCTION BODY[0x5f9110]
	// register traversal process
	m_preventer->add_frustum( frustum, sector_id );

	spatial_sector const& s			= m_structure->get_sectors( )[sector_id];
	u32 const* const portal_ids		= s.get_portals( );
	u32 const portals_count			= s.get_portals_count( );

	for ( u32 i = 0; i < portals_count; ++i )
	{
		if ( portal_ids[i] == input_portal_id )
			continue;

		process_portal_by_frustum_intersection( portal_ids[i], frustum, sector_id, view_pos );
	}
}

// claude@NOTE: ported from the per-portal loop body of dx9/model_manager.cpp
// model_manager::traverse (:240-312): sphere early-out (canonical portals
// keep no sphere member - rebuilt from the 4 points' aabb per legacy
// portal::create; the legacy !f.test(sphere) truthiness is the intended
// ==intersection_outside); far-side sector resolve (get_sector_back(view_point)
// == plane-classify against the sectors pair; the legacy dual_render branch
// has no canonical member) with the self skip; the start-sector skip and the
// portal marker stamp collapse into is_possible_points_for_frustum on the
// preventer; then clip, build the new frustum (far plane reused from the
// current frustum's planes( )[4]) and recurse. Verify against 0x5fb8b0 at
// matcher phase.
void portal_sector_system::process_portal_by_frustum_intersection(
	u32 portal_id,
	math::frustum const& frustum,
	u32 sector_id,
	float3 const& view_pos
)
{
	// FUNCTION BODY[0x5fb8b0]
	portal const& p					= m_structure->get_portals( )[portal_id];

	// early-out sphere
	// claude@NOTE: legacy spelled this `math::aabb bb; bb.invalidate( );`;
	// the canonical math::aabb default ctor is private (friend-only), so the
	// repo-wide create_invalid_aabb( ) idiom is used instead.
	math::aabb bounds = math::create_invalid_aabb( );
	for ( u32 i = 0; i < 4; ++i )
		bounds.modify( p.get_points( )[i] );

	if ( frustum.test( bounds.sphere( ) ) == math::intersection_outside )
		return;

	// select the far-side sector
	u32 const next_sector_id		= p.get_plane( ).classify( view_pos ) > 0 ? p.get_sectors( )[1] : p.get_sectors( )[0];
	if ( next_sector_id == sector_id )
		return;

	// clip by frustum
	float3	points[4];
	std::copy( &p.get_points( )[0], &p.get_points( )[4], &points[0] );

	if ( !cull_points_by_frustum( frustum, points ) )
		return;

	if ( !m_preventer->is_possible_points_for_frustum( points, next_sector_id ) )
		return;

	// create _new_ frustum and recurse
	math::frustum const clip		= create_frustum_from_four_points( view_pos, points, frustum.planes( )[4].plane );
	process_sector( next_sector_id, portal_id, view_pos, clip );
}

void portal_sector_system::process_sector(
	u32,
	u32,
	buffer_vector<aab_rect> const&,
	float3 const&,
	math::plane const&,
	float4x4 const&,
	float4x4 const&,
	aab_rect const&
)
{
	// claude@NOTE: no legacy ancestor - the screen-space scissor traversal exists in dx9/model_manager.cpp only as commented-out code; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5f9090]
}

bool portal_screen_rect_to_four_points(
	aab_rect const&,
	math::plane const&,
	float4x4 const&,
	aab_rect const&,
	float3 (&)[4],
	aab_rect&
)
{
	// claude@NOTE: no legacy ancestor - only the commented-out scissor block in dx9/model_manager.cpp (struct scissor is declaration-only); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5f9bd0]
	return false;
}

void portal_sector_system::process_portal_in_screen_space(
	u32,
	u32,
	buffer_vector<aab_rect> const&,
	float3 const&,
	math::plane const&,
	float4x4 const&,
	float4x4 const&,
	aab_rect const&
)
{
	// claude@NOTE: no legacy ancestor - only the commented-out scissor block in dx9/model_manager.cpp; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5fb630]
}

void portal_sector_system::perform_frustum_culling_and_sectors_test(
	collision::space_partitioning_tree*,
	u32,
	math::frustum const&,
	vector<render_surface_instance*>&,
	float3 const&,
	float4x4 const&
)
{
	// claude@NOTE: legacy body diverged - legacy add_visual_to_selection frustum-tests the render_visual hierarchy; canonical queries the space_partitioning_tree with sector cross-check; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5fbae0]
}

void portal_sector_system::get_portals_occlusion_bounds( float4* )
{
	// claude@NOTE: no legacy ancestor - no GPU portal occlusion-query system in the legacy corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5f9010]
}

void portal_sector_system::initialize_portals_occlusion_bounds_and_results( )
{
	// claude@NOTE: no legacy ancestor - no GPU portal occlusion-query system in the legacy corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5f9940]
}

void portal_sector_system::update_portals_occlusion_culling( pcbyte )
{
	// claude@NOTE: no legacy ancestor - no GPU portal occlusion-query system in the legacy corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5f9040]
}

void portal_sector_system::calculate_portal_rects_in_screen_space(
	float4x4 const&,
	float,
	buffer_vector<aab_rect>&,
	buffer_vector<float>&
)
{
	// claude@NOTE: no legacy ancestor - only the commented-out scissor block in dx9/model_manager.cpp; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5f9180]
}

void portal_sector_system::sort_portals_and_calculate_rects_in_screen_space(
	float4x4 const&,
	float,
	buffer_vector<aab_rect>&
)
{
	// claude@NOTE: no legacy ancestor - only the commented-out scissor block in dx9/model_manager.cpp; no portal sort anywhere in the corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5f9b60]
}

u32 get_aabb_furthest_vertex_id( float3 )
{
	// claude@NOTE: no legacy ancestor - no counterpart in the legacy corpus; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5f8f90]
	return 0;
}

void portal_sector_system::make_frustum_images( float3 const& )
{
	// claude@NOTE: no legacy ancestor - frustum-image debug set is new-in-target; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5fb340]
}

} // namespace culling
} // namespace render
} // namespace vostok
