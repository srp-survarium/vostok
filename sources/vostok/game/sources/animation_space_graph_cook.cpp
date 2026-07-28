////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animation_space_graph_cook.h"
#include "animation_space_graph.h"
#include "animation_space_vertex.h"
#include <vostok/resources.h>
#include <vostok/resources_query_result.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

 animation_space_graph_cook::animation_space_graph_cook( ai::navigation::world& navigation_world ) :
	resources::translate_query_cook( resources::animation_space_graph_class, reuse_true, use_current_thread_id ),
	m_navigation_world( navigation_world )
{
}

void animation_space_graph_cook::translate_request_path( pcstr request, fs_new::virtual_path_string& new_request ) const
{
	new_request.assignf( "resources/npc/human/animation_space_graph/%s.space_graph", request );
}

// claude@NOTE: target evaluates parent.get_requested_path() AFTER the boost::function
// ctor and builds the request inline at the call (right-to-left arg eval); a local
// `request requests[1]` array inits the path early. Call shape + args match; the
// request-build evaluation order is the byte residual.
void animation_space_graph_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::request requests[1] = {
		{ parent.get_requested_path(), resources::binary_config_class_impl },
	};

	resources::query_resources(
		requests,
		1,
		boost::bind( &animation_space_graph_cook::on_options_received, this, _1 ),
		g_allocator,
		NULL,
		&parent
	);
}

void animation_space_graph_cook::delete_resource( resources::resource_base* resource )
{
	animation_space_graph* graph = static_cast< animation_space_graph* >( resource );

	animation_space_vertex const* it = graph->get_animations( ), * const end = it + graph->get_animations_count( );
	for ( ; it != end; ++it )
		it->~animation_space_vertex( );

	graph->~animation_space_graph( );
	VOSTOK_FREE_IMPL( g_allocator, graph );
}

// claude@NOTE: target records 0 named locals (sum + iterators are temps), so the
// per-group vertices fold is likely a std::accumulate; the natural hand loop below
// keeps the accumulator + iterator as named locals. Structure (sum of each group's
// "vertices" child count) is faithful; the 0-local fold is the byte residual.
u32 get_animation_vertices_count( configs::binary_config_value const& groups_config )
{
	u32 vertices_count = 0;
	for ( configs::binary_config_value const* it = groups_config.begin( ); it != groups_config.end( ); ++it )
		vertices_count += ( *it )[ "vertices" ].size( );

	return vertices_count;
}

// claude@NOTE: same 0-named-local fold shape as get_animation_vertices_count (likely a
// std::accumulate). Per group: first += mixable.size(); second += (intervals_count+1) *
// mixable.size(). Structure faithful; the 0-local fold is the byte residual.
std::pair< u32, u32 > get_animation_mixes_count( configs::binary_config_value const& groups_config )
{
	std::pair< u32, u32 > mixes_count( 0, 0 );
	for ( configs::binary_config_value const* it = groups_config.begin( ); it != groups_config.end( ); ++it )
	{
		const u32 intervals_count = ( *it )[ "intervals_count" ];
		if ( ( *it ).value_exists( "mixable" ) )
		{
			const u32 mixable_count = ( *it )[ "mixable" ].size( );
			mixes_count.first	+= mixable_count;
			mixes_count.second	+= ( intervals_count + 1 ) * mixable_count;
		}
	}

	return mixes_count;
}

void animation_space_graph_cook::on_options_received( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent	= data.get_parent_query();
	if ( data.size() != 1 )
	{
		parent->finish_query							( resources::query_result_for_user::error_type_cook_failed );
		return;
	}

	configs::binary_config_ptr config					= static_cast_resource_ptr<configs::binary_config_ptr const>( data[0].get_unmanaged_resource() );
	configs::binary_config_value const& groups_config	= (*config)["animation_space_graph"]["groups"];

	buffer_vector< resources::request >	requests		( ALLOCA( sizeof( resources::request ) * get_animation_vertices_count( groups_config ) ), get_animation_vertices_count( groups_config ) );

	configs::binary_config_value const* it_groups		= groups_config.begin();
	configs::binary_config_value const* it_end_groups	= groups_config.end();
	for ( ; it_groups != it_end_groups; ++it_groups )
	{
		configs::binary_config_value const& vertices_config	= (*it_groups)["vertices"];

		configs::binary_config_value const* it_vertices		= vertices_config.begin();
		configs::binary_config_value const* it_end_vertices	= vertices_config.end();
		for ( ; it_vertices != it_end_vertices; ++it_vertices )
		{
			resources::request								request;
			request.path									= *it_vertices;
			request.id										= resources::animation_class;
			requests.push_back								( request );
		}
	}

	resources::query_resources	(
		&requests.front(),
		requests.size(),
		boost::bind( &animation_space_graph_cook::on_animations_loaded, this, _1, config ),
		g_allocator,
		NULL,
		parent
	);
}

// STATE[STUB]
// claude@NOTE: walks the mixable pairs building animation_space_edge entries via an
// animation_player + animation_space_graph::get_movement() per pair (lexeme wall) over a
// buffer_vector<u32> offsets table. Depends on get_movement; parked behind the lexeme wall.
void animation_space_graph_cook::generate_graph_edges( animation_space_graph* graph )
{
}

// STATE[STUB]
// claude@NOTE: parses the loaded animation config, placement-news the vertex / mix /
// edge arrays behind the graph object (animation_space_vertex ctor per animation), then
// calls generate_graph_edges. Needs the graph-layout placement allocation + vertex ctor +
// generate_graph_edges (lexeme wall).
void animation_space_graph_cook::on_animations_loaded( resources::queries_result& data, configs::binary_config_ptr config )
{
}

} // namespace survarium
