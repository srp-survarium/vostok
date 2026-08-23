////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animation_space_graph_cook.h"
#include "animation_space_graph.h"
#include "animation_space_vertex.h"
#include <vostok/animation/animation_player.h>
#include <vostok/resources.h>
#include <vostok/resources_query_result.h>
#include <vostok/configs_binary_config.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

extern float const agent_radius;

 animation_space_graph_cook::animation_space_graph_cook( ai::navigation::world& navigation_world ) :
	resources::translate_query_cook( resources::animation_space_graph_class, reuse_true, use_current_thread_id ),
	m_navigation_world( navigation_world )
{
}

void animation_space_graph_cook::translate_request_path( pcstr request, fs_new::virtual_path_string& new_request ) const
{
	new_request.assignf( "resources/npc/human/animation_space_graph/%s.space_graph", request );
}

void animation_space_graph_cook::translate_query( resources::query_result_for_cook& parent )
{
	resources::query_resource(
		parent.get_requested_path(),
		resources::binary_config_class_impl,
		boost::bind( &animation_space_graph_cook::on_options_received, this, _1 ),
		g_allocator,
		NULL,
		&parent
	);
}

void animation_space_graph_cook::delete_resource( resources::resource_base* resource )
{
	animation_space_graph* graph = static_cast< animation_space_graph* >( resource );

	animation_space_vertex const* it = graph->get_animations( );
	animation_space_vertex const* const end = it + graph->get_animations_count( );
	for ( ; it != end; ++it )
		it->~animation_space_vertex( );

	graph->~animation_space_graph( );
	VOSTOK_FREE_IMPL( g_allocator, graph );
}





u32 get_animation_vertices_count( configs::binary_config_value const& groups_config )
{
	u32 vertices_count = 0;
	configs::binary_config_value const* it = groups_config.begin( );
	configs::binary_config_value const* const it_end = groups_config.end( );
	for ( ; it != it_end; ++it )
		vertices_count += ( *it )[ "vertices" ].size( );

	return vertices_count;
}




std::pair< u32, u32 > get_animation_mixes_count( configs::binary_config_value const& groups_config )
{
	std::pair< u32, u32 > mixes_count( 0, 0 );
	configs::binary_config_value const* it = groups_config.begin( );
	configs::binary_config_value const* const it_end = groups_config.end( );
	for ( ; it != it_end; ++it )
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

	u32 const vertices_count = get_animation_vertices_count( groups_config );
	buffer_vector< resources::request >	requests		( ALLOCA( sizeof( resources::request ) * vertices_count ), vertices_count );

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

void animation_space_graph_cook::generate_graph_edges( animation_space_graph* graph )
{
	u32 const mixes_count = graph->get_mixes_count( );
	buffer_vector< u32 > offsets( ALLOCA( mixes_count * sizeof( u32 ) ), mixes_count );

	std::pair< animation_space_vertex const*, animation_space_vertex const* > const* it_begin = graph->get_mixes( );
	std::pair< animation_space_vertex const*, animation_space_vertex const* > const* const it_end = it_begin + mixes_count;
	u32 current_offset = 0;

	for ( ; it_begin != it_end; ++it_begin )
	{
		offsets.push_back( current_offset );
		current_offset += it_begin->first->intervals_count + 1;
	}

	animation_space_edge* it_edges = const_cast< animation_space_edge* >( graph->get_edges( ) );
	animation::animation_player player;
	for ( u32 i = 0; i < current_offset; ++i )
	{
		u32 const* it = std::lower_bound( offsets.begin( ), offsets.end( ), i );
		u32 offset;
		if ( *it == i )
			offset = i;
		else if ( it == offsets.begin( ) )
			offset = 0;
		else
			offset = *--it;

		std::pair< animation_space_vertex const*, animation_space_vertex const* > const& mix = graph->get_mixes( )[ it - offsets.begin( ) ];

		float const left_weight = ( i - offset ) *
			( 1.f / mix.first->intervals_count );

		new ( it_edges++ ) animation_space_edge(
			animation_space_graph::get_movement( player, mix.first, mix.second, left_weight ),
			&mix,
			left_weight,
			( mix.second->length * ( 1.f - left_weight ) + mix.first->length * left_weight ) / mix.first->length
		);
	}
}






void animation_space_graph_cook::on_animations_loaded( resources::queries_result& data, configs::binary_config_ptr config )
{
	resources::query_result_for_cook* const parent = data.get_parent_query( );

	if ( !data.is_successful( ) )
	{
		parent->finish_query( result_error );
		return;
	}

	configs::binary_config_value const& groups =
		(*config)["animation_space_graph"]["groups"];

	u32 const animations_count = data.size( );

	std::pair< u32, u32 > mixes_count = get_animation_mixes_count( groups );




	animation_space_graph* graph = new ( MALLOC(
		sizeof( animation_space_graph ) +
		animations_count * sizeof( animation_space_vertex ) +
		mixes_count.first * sizeof( std::pair< animation_space_vertex const*, animation_space_vertex const* > ) +
		mixes_count.second * sizeof( animation_space_edge ),
		"animation space graph"
	) ) animation_space_graph( m_navigation_world, agent_radius, animations_count, mixes_count.first, mixes_count.second );

	animation_space_vertex* it_animations = const_cast< animation_space_vertex* >( graph->get_animations( ) );
	for ( u32 i = 0; i < animations_count; ++i, ++it_animations )
	{
		resources::query_result_for_user const& query = data[i];
		new ( it_animations ) animation_space_vertex( query.get_managed_resource( ), query.get_requested_path( ) );
	}

	std::pair< animation_space_vertex const*, animation_space_vertex const* >* it_edges =
		const_cast< std::pair< animation_space_vertex const*, animation_space_vertex const* >* >( graph->get_mixes( ) );

	configs::binary_config_value const* it_groups = groups.begin( );
	configs::binary_config_value const* it_end_groups = groups.end( );
	for ( ; it_groups != it_end_groups; ++it_groups )
	{
		u32 const group_id = (*it_groups)["group_id"];
		u32 const intervals_count = (*it_groups)["intervals_count"];

		configs::binary_config_value const& vertices = (*it_groups)["vertices"];

		configs::binary_config_value const* it_vertices = vertices.begin( );
		configs::binary_config_value const* it_end_vertices = vertices.end( );
		for ( ; it_vertices != it_end_vertices; ++it_vertices )
		{
			animation_space_vertex* vertex = const_cast< animation_space_vertex* >( graph->get_animation_by_path( *it_vertices ) );
			vertex->group_id = group_id;
			vertex->intervals_count = intervals_count;
		}

		if ( (*it_groups).value_exists( "mixable" ) )
		{
			configs::binary_config_value const& mixable = (*it_groups)["mixable"];

			configs::binary_config_value const* it_mix = mixable.begin( );
			configs::binary_config_value const* it_end_mix = mixable.end( );
			for ( ; it_mix != it_end_mix; ++it_mix )
			{
				pcstr second_path = vertices[ u32( (*it_mix)["second"] ) ];
				animation_space_vertex const* const first_mixable = graph->get_animation_by_path( vertices[ u32( (*it_mix)["first"] ) ] );

				new ( it_edges++ ) std::pair< animation_space_vertex const*, animation_space_vertex const* >(
					first_mixable,
					graph->get_animation_by_path( second_path )
				);
			}
		}
	}

	generate_graph_edges( graph );

	parent->set_unmanaged_resource( graph, resources::nocache_memory, sizeof( animation_space_graph ) );
	parent->finish_query( result_success );
}

} // namespace survarium
