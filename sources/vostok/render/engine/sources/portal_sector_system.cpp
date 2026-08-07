#include "pch.h"
#include "portal_sector_system.h"

#include <vostok/collision/common_types.h>
#include <vostok/fixed_vector.h>
#include <vostok/math_frustum.h>
#include <vostok/math_plane.h>
#include <vostok/math_sphere.h>

#include <numeric>

#include <vostok/console_command.h>
#include <vostok/math_color.h>
#include <vostok/render/engine/vertex_colored.h>

#include "sector_double_query_preventer.h"
#include "statistics.h"
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
	m_occlusion_bounds_buffer		( ALLOC( float4, structure->get_portals( ).size( ) ) ),
	m_occlusion_bounds				( m_occlusion_bounds_buffer, structure->get_portals( ).size( ) ),
	m_occlusion_results_buffer		( ALLOC( u8, structure->get_portals( ).size( ) ) ),
	m_occlusion_results			( m_occlusion_results_buffer, structure->get_portals( ).size( ), structure->get_portals( ).size( ) ),
	m_portals_geometry				( 0 )
{
	initialize_portals_occlusion_bounds_and_results( );
	m_preventer = NEW( sector_double_query_preventer )(
		m_structure->get_sectors( ).begin( ),
		m_structure->get_sectors( ).size( )
	);
}

portal_sector_system::~portal_sector_system( )
{
	m_occlusion_results.clear( );
	FREE( m_occlusion_results_buffer );
	m_occlusion_bounds.clear( );
	FREE( m_occlusion_bounds_buffer );
	DELETE( m_preventer );
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

void portal_sector_system::draw_quads( system_renderer& renderer )
{
	u16 const quad_indices[]	= {
		0, 1,
		1, 2,
		2, 3,
		3, 0
	};

	vector<quad>::const_iterator const quads_end = m_quads.end( );
	for ( vector<quad>::const_iterator i = m_quads.begin( ); i != quads_end; ++i )
	{
		vertex_colored const vertices[] = {
			vertex_colored( i->vertices[0], math::color( 0xff00ffff ) ),
			vertex_colored( i->vertices[1], math::color( 0xff00ffff ) ),
			vertex_colored( i->vertices[2], math::color( 0xff00ffff ) ),
			vertex_colored( i->vertices[3], math::color( 0xff00ffff ) ),
		};

		renderer.draw_lines( vertices, vertices + array_size( vertices ), quad_indices, quad_indices + array_size( quad_indices ), false );
	}
}

void portal_sector_system::draw_portals( system_renderer& renderer, u32 const active_sector_id )
{
	u16 const frustum_indices[]		= {
		0, 1, 2,
		0, 2, 3,
		0, 2, 1,
		0, 3, 2
	};

	spatial_sector const& s			= m_structure->get_sectors( )[active_sector_id];
	u32 const* const portals_end	= s.get_portals( ) + s.get_portals_count( );
	for ( u32 const* i = s.get_portals( ); i != portals_end; ++i )
	{
		portal const& p				= m_structure->get_portals( )[*i];
		if ( !p.is_visible( ) )
			continue;

		vertex_colored const vertices[] = {
			vertex_colored( p.get_points( )[0], math::color( 0x6464c864 ) ),
			vertex_colored( p.get_points( )[1], math::color( 0x6464c864 ) ),
			vertex_colored( p.get_points( )[2], math::color( 0x6464c864 ) ),
			vertex_colored( p.get_points( )[3], math::color( 0x6464c864 ) ),
		};
		renderer.draw_triangles	(
			vertices, vertices + array_size( vertices ),
			frustum_indices, frustum_indices + array_size( frustum_indices ),
			false
		);
	}

	draw_quads( renderer );
}

void portal_sector_system::select_models(
	collision::space_partitioning_tree* tree,
	float3 const& view_pos,
	float4x4 const& mat_vp,
	vector<render_surface_instance*>& selection
)
{
	m_preventer->clear( );
	selection.clear( );
	m_quads.clear( );


	math::frustum f( mat_vp );
	u32 const active_sector_id = m_structure->get_sector_id( *g_allocator, view_pos );
	m_preventer->add_frustum( f, active_sector_id );
	m_structure->update_portals_visability( f, m_occlusion_results.begin( ) );
	if ( !s_use_screeen_space_portals_intersection_value )
	{


		process_sector( active_sector_id, u32( -1 ), view_pos, f );
	}
	else
	{
		float4x4 inverted_vp;
		if ( math::try_invert4x4( mat_vp, inverted_vp ) )
		{
			buffer_vector<aab_rect> portal_rects( ALLOCA( sizeof( aab_rect ) * m_structure->get_portals( ).size( ) ), m_structure->get_portals( ).size( ) );
			float const min_z = mat_vp.transform_position( view_pos + f.planes( )[4].plane.normal * .01f ).z * 2.f;
			sort_portals_and_calculate_rects_in_screen_space( mat_vp, min_z, portal_rects );
			aab_rect limiting_rect;
			m_preventer->add_ss_aab_rect( limiting_rect, active_sector_id );
			process_sector( active_sector_id, u32( -1 ), portal_rects, view_pos, f.planes( )[4].plane, mat_vp, inverted_vp, limiting_rect );
		}
		else
		{
			DEBUG_BREAK( );
			process_sector( active_sector_id, u32( -1 ), view_pos, f );
		}
	}

	perform_frustum_culling_and_sectors_test( tree, active_sector_id, f, selection, view_pos, mat_vp );
	if ( m_test_action )
	{
		m_test_action = false;
		make_frustum_images( -f.planes( )[4].plane.normal );
	}
	statistics::ref( ).visibility_stat_group.frustums_count.value = m_preventer->frustums_count( );
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
	u32 sector_id,
	u32 input_portal_id,
	buffer_vector<aab_rect> const& portals_rects,
	float3 const& view_pos,
	math::plane const& far_plane,
	float4x4 const& mat_vp,
	float4x4 const& inv_mat_vp,
	aab_rect const& rect
)
{
	spatial_sector const& s = m_structure->get_sectors( )[sector_id];
	u32 const* const portals_end = s.get_portals( ) + s.get_portals_count( );
	for ( u32 const* i = s.get_portals( ); i != portals_end; ++i )
	{
		if ( *i == input_portal_id || !m_structure->get_portals( )[*i].is_visible( ) )
			continue;

		process_portal_in_screen_space( *i, sector_id, portals_rects, view_pos, far_plane, mat_vp, inv_mat_vp, rect );
	}
}

bool portal_screen_rect_to_four_points(
	aab_rect const& portal_rect,
	math::plane const& portal_plane,
	float4x4 const& inv_mat_vp,
	aab_rect const& limiting_rect,
	float3 (&io_points)[4],
	aab_rect& limited_rect
)
{
	if ( !portal_rect.intersects( limiting_rect ) )
		return false;

	limited_rect = get_intersection_rect( portal_rect, limiting_rect );
	float const limited_rect_square =
		( limited_rect.max.x - limited_rect.min.x ) * ( limited_rect.max.y - limited_rect.min.y );
	if ( math::is_zero( limited_rect_square ) )
		return false;

	float3 ws_near_rect[4] = {
		inv_mat_vp.transform( float3( limited_rect.min.x, limited_rect.min.y, 0 ) ),
		inv_mat_vp.transform( float3( limited_rect.min.x, limited_rect.max.y, 0 ) ),
		inv_mat_vp.transform( float3( limited_rect.max.x, limited_rect.max.y, 0 ) ),
		inv_mat_vp.transform( float3( limited_rect.max.x, limited_rect.min.y, 0 ) ),
	};

	float3 ws_far_rect[4] = {
		inv_mat_vp.transform( float3( limited_rect.min.x, limited_rect.min.y, 1 ) ),
		inv_mat_vp.transform( float3( limited_rect.min.x, limited_rect.max.y, 1 ) ),
		inv_mat_vp.transform( float3( limited_rect.max.x, limited_rect.max.y, 1 ) ),
		inv_mat_vp.transform( float3( limited_rect.max.x, limited_rect.min.y, 1 ) ),
	};

	if ( !portal_plane.intersect_segment( ws_near_rect[0], ws_far_rect[0], io_points[0] ) ||
		!portal_plane.intersect_segment( ws_near_rect[1], ws_far_rect[1], io_points[1] ) ||
		!portal_plane.intersect_segment( ws_near_rect[2], ws_far_rect[2], io_points[2] ) ||
		!portal_plane.intersect_segment( ws_near_rect[3], ws_far_rect[3], io_points[3] ) )
		std::copy( &ws_near_rect[0], &ws_near_rect[4], &io_points[0] );

	return true;
}

void portal_sector_system::process_portal_in_screen_space(
	u32 portal_id,
	u32 sector_id,
	buffer_vector<aab_rect> const& portals_rects,
	float3 const& view_pos,
	math::plane const& far_plane,
	float4x4 const& mat_vp,
	float4x4 const& inv_mat_vp,
	aab_rect const& limiting_rect
)
{
	portal const& p = m_structure->get_portals( )[portal_id];
	if ( ( p.get_plane( ).classify( view_pos ) > 0 ? p.get_sectors( )[1] : p.get_sectors( )[0] ) == sector_id )
		return;

	float3 points[4];
	std::copy( &p.get_points( )[0], &p.get_points( )[4], &points[0] );
	aab_rect limited_portal_rect = portals_rects[portal_id];
	if ( !limiting_rect.contains( portals_rects[portal_id] ) &&
		!portal_screen_rect_to_four_points( portals_rects[portal_id], far_plane, inv_mat_vp, limiting_rect, points, limited_portal_rect ) )
		return;

	u32 const next_sector_id = p.get_sectors( )[0] != sector_id ? p.get_sectors( )[0] : p.get_sectors( )[1];
	if ( !m_preventer->is_possible_ss_aab_rect( limited_portal_rect, next_sector_id ) )
		return;

	if ( points[0].is_similar( points[1] ) ||
		points[1].is_similar( points[2] ) ||
		points[2].is_similar( points[3] ) ||
		points[3].is_similar( points[0] ) )
		return;

	m_quads.push_back( quad( ) );
	std::copy( &points[0], &points[4], &m_quads.back( ).vertices[0] );
	math::frustum const portal_frustum = create_frustum_from_four_points( view_pos, points, far_plane );
	m_preventer->add_frustum( portal_frustum, next_sector_id );
	m_preventer->add_ss_aab_rect( limited_portal_rect, next_sector_id );
	process_sector( next_sector_id, portal_id, portals_rects, view_pos, far_plane, mat_vp, inv_mat_vp, limited_portal_rect );
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

void portal_sector_system::get_portals_occlusion_bounds( float4* bounds )
{
	std::copy( m_occlusion_bounds.begin( ), m_occlusion_bounds.end( ), bounds );
}

void portal_sector_system::initialize_portals_occlusion_bounds_and_results( )
{
	portal const* const portals_end = m_structure->get_portals( ).end( );
	for ( portal const* i = m_structure->get_portals( ).begin( ); i != portals_end; ++i )
	{
		float3 const center = std::accumulate( &i->get_points( )[0], &i->get_points( )[4], float3( 0, 0, 0 ) ) / 4.f;



		float const radius = math::max( math::max( math::squared_length( i->get_points( )[0] - center ), math::squared_length( i->get_points( )[1] - center ) ), math::max( math::squared_length( i->get_points( )[2] - center ), math::squared_length( i->get_points( )[3] - center ) ) );
		m_occlusion_bounds.push_back( float4( center, math::sqrt( radius ) ) );
	}
	std::fill( m_occlusion_results.begin( ), m_occlusion_results.end( ), u8( -1 ) );
}

void portal_sector_system::update_portals_occlusion_culling( pcbyte occlusion_results )
{
	if ( s_portals_occlusion_culling_value )
		std::copy(
			occlusion_results,
			occlusion_results + m_occlusion_results.size( ),
			m_occlusion_results.begin( )
		);
	else
		std::fill( m_occlusion_results.begin( ), m_occlusion_results.end( ), u8( -1 ) );
}

void portal_sector_system::calculate_portal_rects_in_screen_space(
	float4x4 const& mat_vp,
	float min_z,
	buffer_vector<aab_rect>& rects,
	buffer_vector<float>& distances
)
{
	rects.clear( );
	distances.clear( );
	portal const* const portals_end = m_structure->get_portals( ).end( );
	for ( portal const* it = m_structure->get_portals( ).begin( ); it != portals_end; ++it )
	{
		aab_rect portal_rect;
		if ( !it->is_visible( ) )
		{
			portal_rect.min = portal_rect.max = float2( 0.f, 0.f );
			rects.push_back( portal_rect );
			distances.push_back( math::float_max );
			continue;
		}

		u32 const portal_id = u32( it - m_structure->get_portals( ).begin( ) );
		float4 const cs_f4[4] = {
			mat_vp.transform( float4( it->get_points( )[0], 1.f ) ),
			mat_vp.transform( float4( it->get_points( )[1], 1.f ) ),
			mat_vp.transform( float4( it->get_points( )[2], 1.f ) ),
			mat_vp.transform( float4( it->get_points( )[3], 1.f ) ),
		};
		float3 const hs_f3[4] = {
			cs_f4[0].xyz( ) / math::max( math::epsilon_3, cs_f4[0].w ),
			cs_f4[1].xyz( ) / math::max( math::epsilon_3, cs_f4[1].w ),
			cs_f4[2].xyz( ) / math::max( math::epsilon_3, cs_f4[2].w ),
			cs_f4[3].xyz( ) / math::max( math::epsilon_3, cs_f4[3].w ),
		};

		if ( cs_f4[0].z < min_z && cs_f4[1].z < min_z && cs_f4[2].z < min_z && cs_f4[3].z < min_z )
		{
			m_structure->set_portal_visible( portal_id, false );
			portal_rect.min = portal_rect.max = float2( 0.f, 0.f );
			rects.push_back( portal_rect );
			distances.push_back( math::float_max );
			continue;
		}

		portal_rect.min = portal_rect.max = float2( hs_f3[0].x, hs_f3[0].y );
		portal_rect.modify( hs_f3[1] );
		portal_rect.modify( hs_f3[2] );
		portal_rect.modify( hs_f3[3] );
		rects.push_back( portal_rect );
		bool const visible = portal_rect.intersects( aab_rect( ) );
		m_structure->set_portal_visible( portal_id, visible );
		distances.push_back(
			visible ? math::max(
				0.f,
				math::min(
					math::min( cs_f4[0].z, cs_f4[1].z ),
					math::min( cs_f4[2].z, cs_f4[3].z )
				)
			) : math::float_max
		);
	}
}

void portal_sector_system::sort_portals_and_calculate_rects_in_screen_space(
	float4x4 const& mat_vp,
	float min_z,
	buffer_vector<aab_rect>& rects
)
{
	buffer_vector<float> distances( ALLOCA( sizeof( float ) * m_structure->get_portals( ).size( ) ), m_structure->get_portals( ).size( ) );
	calculate_portal_rects_in_screen_space( mat_vp, min_z, rects, distances );
	m_structure->sort_portal_ids( distances.begin( ) );
}

u32 get_aabb_furthest_vertex_id( float3 const view_dir )
{
	if ( view_dir.x >= 0.f )
	{
		if ( view_dir.y >= 0.f )
			return view_dir.z >= 0.f ? 7 : 6;
		return view_dir.z >= 0.f ? 5 : 4;
	}
	if ( view_dir.y >= 0.f )
		return view_dir.z >= 0.f ? 2 : 3;
	return view_dir.z >= 0.f ? 1 : 0;
}

void portal_sector_system::make_frustum_images( float3 const& view_dir )
{
	u32 const furthest_vertex_id = get_aabb_furthest_vertex_id( view_dir );
	float3* const furthest_vertices = static_cast<float3*>( ALLOCA( sizeof( float3 ) * m_structure->get_sectors( ).size( ) ) );
	spatial_sector const* const sectors_end = m_structure->get_sectors( ).end( );
	float3* output = furthest_vertices;
	for ( spatial_sector const* i = m_structure->get_sectors( ).begin( ); i != sectors_end; ++i, ++output )
		*output = i->get_aabb( ).vertex( furthest_vertex_id );
	m_preventer->make_frustum_images( furthest_vertices );
}

} // namespace culling
} // namespace render
} // namespace vostok
