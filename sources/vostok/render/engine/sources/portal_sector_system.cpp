#include "pch.h"
#include "portal_sector_system.h"

#include <vostok/collision/common_types.h>
#include <vostok/collision/space_partitioning_tree.h>
#include <vostok/buffer_vector.h>
#include <vostok/math_frustum.h>
#include <vostok/math_plane.h>
#include <vostok/math_sphere.h>

#include <wildmagic/sdk/include/Wm4ContBox2.h>

#include <numeric>

#include <vostok/console_command.h>
#include <vostok/math_color.h>
#include <vostok/render/culling/possible_sectors_holder.h>
#include <vostok/render/engine/vertex_colored.h>

#include "render_model_instance_impl.h"
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

math::frustum create_frustum_from_four_points(
	float3 const& view_pos,
	float3 const (&points)[4],
	math::plane const& far_plane
)
{
	math::plane const plane = math::create_plane( points[0], points[1], points[2] );
	float const distance = plane.classify( view_pos );
	if ( distance > 0.f )
	{
		math::plane frustrum_planes[math::cuboid::plane_count];

		frustrum_planes[0] = math::create_plane( view_pos, points[1], points[0] );

		frustrum_planes[1] = math::create_plane( view_pos, points[3], points[2] );

		frustrum_planes[2] = math::create_plane( view_pos, points[2], points[1] );

		frustrum_planes[3] = math::create_plane( view_pos, points[0], points[3] );

		frustrum_planes[4] = far_plane;


		frustrum_planes[5].normal = -plane.normal;
		frustrum_planes[5].d = -plane.d;

		math::frustum result( frustrum_planes );
		return result;
	}
	else
	{
		math::plane frustrum_planes[math::cuboid::plane_count];
		frustrum_planes[0] = math::create_plane( view_pos, points[0], points[1] );

		frustrum_planes[1] = math::create_plane( view_pos, points[2], points[3] );

		frustrum_planes[2] = math::create_plane( view_pos, points[1], points[2] );

		frustrum_planes[3] = math::create_plane( view_pos, points[3], points[0] );
		frustrum_planes[4] = far_plane;


		frustrum_planes[5] = plane;
		math::frustum result( frustrum_planes );

		return result;
	}
}

bool cull_points_by_frustum( math::frustum const& f, float3 (&io_points)[4] )
{
	typedef buffer_vector< Wm4::Vector2< float > > wm_vertices_2d_buffer_type;
	typedef buffer_vector< float3 > vertices_buffer_type;
	typedef Wm4::Vector2< float > wm_vertex_2d;

	float3 const normal = math::normalize( ( io_points[1] - io_points[0] ) ^ ( io_points[2] - io_points[0] ) );


	vertices_buffer_type temp( ALLOCA( 16 * sizeof( float3 ) ), 16 );
	vertices_buffer_type pos( ALLOCA( 16 * sizeof( float3 ) ), 16 );
	std::copy( io_points, io_points + 4, std::back_inserter( temp ) );


	for ( u32 plane_id = 0; plane_id < math::cuboid::plane_count; ++plane_id )
	{
		pos.clear( );

		u32 const temp_count = temp.size( );
		for ( u32 i = 0; i < temp_count; ++i )
		{
			float const distance = f.planes( )[plane_id].plane.classify( temp[i] );
			if ( math::is_zero( distance ) || distance > 0.f )
				pos.push_back( temp[i] );
			float3 intersection_position;
			if ( f.planes( )[plane_id].plane.intersect_segment( temp[i], temp[( i + 1 ) % temp_count], intersection_position ) && ( pos.empty( ) || !pos.back( ).is_similar( intersection_position ) ) )
				pos.push_back( intersection_position );
		}
		if ( pos.size( ) < 3 )
			return false;
		temp.clear( );
		std::copy( pos.begin( ), pos.end( ), std::back_inserter( temp ) );
	}

	if ( pos.size( ) < 3 )
		return false;



	float longest_edge_length = ( pos[1] - pos[0] ).squared_length( );
	u32 longest_edge_id = 0;
	for ( u32 i = 1; i < pos.size( ); ++i )
	{
		float const edge_length = ( pos[( i + 1 ) % pos.size( )] - pos[i] ).squared_length( );
		if ( edge_length > longest_edge_length )
		{
			longest_edge_length = edge_length;
			longest_edge_id = i;
		}
	}
	if ( math::is_zero( longest_edge_length ) )
		return false;
	float3 const x_axis = ( pos[( longest_edge_id + 1 ) % pos.size( )] - pos[longest_edge_id] ).normalize( );
	float3 const y_axis = ( normal ^ x_axis ).normalize( );
	float4x4 local_to_world;
	local_to_world.identity( );
	local_to_world.i.xyz( ) = x_axis;
	local_to_world.j.xyz( ) = y_axis;
	local_to_world.k.xyz( ) = normal;
	local_to_world.c.xyz( ) = pos[0];
	float4x4 world_to_local;
	world_to_local.try_invert( local_to_world );




	wm_vertices_2d_buffer_type wm_vertices_2d( ALLOCA( pos.size( ) * sizeof( wm_vertex_2d ) ), pos.size( ) );

	for ( u32 i = 0; i < pos.size( ); ++i )
	{
		float3 const local_position = world_to_local.transform_position( pos[i] );
		wm_vertices_2d.push_back( wm_vertex_2d( local_position.x, local_position.y ) );
	}
	Wm4::Box2< float > min_box = Wm4::ContMinBox( wm_vertices_2d.size( ), &wm_vertices_2d.front( ), 0.f, Wm4::Query::QT_REAL, false );
	wm_vertex_2d min_box_points[4];
	min_box.ComputeVertices( min_box_points );
	io_points[0] = local_to_world.transform_position( float3( min_box_points[0].X( ), min_box_points[0].Y( ), 0.f ) );

	io_points[1] = local_to_world.transform_position( float3( min_box_points[1].X( ), min_box_points[1].Y( ), 0.f ) );
	io_points[2] = local_to_world.transform_position( float3( min_box_points[2].X( ), min_box_points[2].Y( ), 0.f ) );
	io_points[3] = local_to_world.transform_position( float3( min_box_points[3].X( ), min_box_points[3].Y( ), 0.f ) );
	return !math::is_zero(
		( ( io_points[1] - io_points[0] ) ^ ( io_points[2] - io_points[0] ) ).length( ) * 0.5f +
		( ( io_points[2] - io_points[0] ) ^ ( io_points[3] - io_points[0] ) ).length( ) * 0.5f
	);
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

	quads_type::const_iterator const quads_end = m_quads.end( );
	for ( quads_type::const_iterator i = m_quads.begin( ); i != quads_end; ++i )
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
		if ( math::try_invert4x4(
			mat_vp,
			inverted_vp
		) )
		{
			aab_rects_buffer_type portal_rects( ALLOCA( sizeof( aab_rect ) * m_structure->get_portals( ).size( ) ), m_structure->get_portals( ).size( ) );
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

void portal_sector_system::process_sector( u32 sector_id, u32 input_portal_id, float3 const& view_pos, math::frustum const& frustum )
{
	spatial_sector const& s = m_structure->get_sectors( )[sector_id];
	u32 const* const portals_end = s.get_portals( ) + s.get_portals_count( );
	for ( u32 const* i = s.get_portals( ); i != portals_end; ++i )
	{
		if ( *i == input_portal_id || !m_structure->get_portals( )[*i].is_visible( ) )
			continue;

		process_portal_by_frustum_intersection( *i, frustum, sector_id, view_pos );
	}
}

void portal_sector_system::process_portal_by_frustum_intersection(
	u32 portal_id,
	math::frustum const& frustum,
	u32 sector_id,
	float3 const& view_pos
)
{
	portal const& p = m_structure->get_portals( )[portal_id];
	if ( p.get_sectors( )[p.get_plane( ).classify( -frustum.planes( )[4].plane.normal ) > 0.f] == sector_id )
		return;

	float3 points[4];
	std::copy( &p.get_points( )[0], &p.get_points( )[4], &points[0] );
	if ( !cull_points_by_frustum( frustum, points ) )
		return;
	float3 const edge0 = points[2] - points[1], edge1 = points[1] - points[0];
	if ( math::is_zero( edge0.length( ) * edge1.length( ), math::epsilon_3 ) )
		return;
	u32 const next_sector_id = p.get_sectors( )[0] != sector_id ? p.get_sectors( )[0] : p.get_sectors( )[1];
	if ( !m_preventer->is_possible_points_for_frustum( points, next_sector_id ) )
		return;


	math::frustum const clip = create_frustum_from_four_points( view_pos, points, frustum.planes( )[4].plane );
	m_preventer->add_frustum( clip, next_sector_id );
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
	float const limited_rect_square = ( limited_rect.max.x - limited_rect.min.x ) * ( limited_rect.max.y - limited_rect.min.y );
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
	{
		std::copy( &ws_near_rect[0], &ws_near_rect[4], &io_points[0] );
	}
	return true;
}

void portal_sector_system::process_portal_in_screen_space(
	u32 portal_id,
	u32 sector_id,
	aab_rects_buffer_type const& portals_rects,
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
	collision::space_partitioning_tree* tree,
	u32 active_sector_id,
	math::frustum const& f,
	vector<render_surface_instance*>& visible_surfaces,
	float3 const& view_pos,
	float4x4 const& mat_vp
)
{
	collision::objects_type query_result( *g_allocator );
	tree->cuboid_query( u32( -1 ), f, query_result );

	vector<render_surface_instance*> surfaces;
	collision::object const* const* query_result_end = query_result.end( );
	for ( collision::object const** it = query_result.begin( ); it != query_result_end; ++it )
	{
		render_collision_object<render_model_instance_impl> const* const object =
			static_cast_checked<render_collision_object<render_model_instance_impl> const*>( *it );
		possible_sectors_holder const* const sectors = object->owner( )->get_sectors_holder( );

		if ( sectors &&
			!sectors->is_possible_sector( (u16)active_sector_id ) &&
			!m_preventer->is_visible_aabb( object->get_aabb( ), sectors->begin( ), sectors->end( ) ) )
			continue;

		surfaces.clear( );

		object->owner( )->get_surfaces( &mat_vp, &view_pos, surfaces, true, u8( -1 ), 3 );

		render_surface_instance* const* const surfaces_end = surfaces.end( );
		for ( render_surface_instance* const* it = surfaces.begin( ); it != surfaces_end; ++it )
			visible_surfaces.push_back( *it );
	}
}

void portal_sector_system::get_portals_occlusion_bounds( float4* bounds )
{
	std::copy( m_occlusion_bounds.begin( ), m_occlusion_bounds.end( ), bounds );
}

void portal_sector_system::initialize_portals_occlusion_bounds_and_results( )
{
	portals_type::const_iterator const portals_end = m_structure->get_portals( ).end( );
	for ( portals_type::const_iterator i = m_structure->get_portals( ).begin( ); i != portals_end; ++i )
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
	aab_rects_buffer_type& rects,
	float_buffer_type& distances
)
{
	aab_rect portal_rect;

	rects.clear( );
	distances.clear( );
	portals_type::const_iterator const portals_end = m_structure->get_portals( ).end( );
	for ( portals_type::const_iterator it = m_structure->get_portals( ).begin( ); it != portals_end; ++it )
	{
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
				0.f, math::min( math::min( cs_f4[0].z, cs_f4[1].z ), math::min( cs_f4[2].z, cs_f4[3].z ) )
			) : math::float_max
		);
	}
}

void portal_sector_system::sort_portals_and_calculate_rects_in_screen_space(
	float4x4 const& mat_vp,
	float min_z,
	aab_rects_buffer_type& rects
)
{
	float_buffer_type distances( ALLOCA( sizeof( float ) * m_structure->get_portals( ).size( ) ), m_structure->get_portals( ).size( ) );
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
	sectors_type::const_iterator const sectors_end = m_structure->get_sectors( ).end( );
	float3* output = furthest_vertices;
	for ( sectors_type::const_iterator i = m_structure->get_sectors( ).begin( ); i != sectors_end; ++i, ++output )
	{
		*output = i->get_aabb( ).vertex( furthest_vertex_id );
	}
	m_preventer->make_frustum_images( furthest_vertices );
}

} // namespace culling
} // namespace render
} // namespace vostok
