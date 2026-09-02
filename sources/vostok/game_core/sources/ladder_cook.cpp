// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/ladder_cook.h>

#include <vostok/game_core/ladder.h>
#include <vostok/game_core/landing_point.h>

namespace survarium {

ladder_cook::ladder_cook( ) : resources::translate_query_cook( resources::ladder_class, reuse_false, use_current_thread_id )
{
}

ladder_cook::~ladder_cook( )
{
}
// claude@NOTE: STRUCTURE MATCH (30 stmts in order). Residual is the inline-vs-call COMDAT
// wall on STL/container helpers: target CALLs vostok::vectora<request>::vectora (4 ICF-folded
// copies in the target index, ZERO in our base -> we inline the whole _Impl_vector ctor +
// g_allocator load, ~0x35 bytes bigger), and CALLs requests.begin()/requests.size()
// out-of-line in the query_resources arg list (target: call buffer_vector<request>::size +
// begin; base: inlines the (end-begin)/8 pointer math). Decided by whether those template
// helpers are emitted out-of-line globally - not steerable from this caller. The LOG_* size
// deltas are the format-string append inline-fold (logging module).
void ladder_cook::translate_query( resources::query_result_for_cook& parent )
{
	variant<32>* user_data = parent.user_data( );
	ASSERT( UNKNOWN_EXPRESSION_T( user_data ) );

	configs::binary_config_value config;
	bool result = user_data->try_get( config );
	ASSERT( UNKNOWN_EXPRESSION_T( result ) );
	VOSTOK_UNREFERENCED_PARAMETER( result );
	vectora<resources::request> requests( g_allocator ); // sushi@NOTE: Either LTCG for `g_allocator`, or this is wrapped into something

	if ( !config.value_exists( "main_animation" ) )
	{
		LOG_ERROR( "there's no animation for this ladder, object can't be created" );
		parent.finish_query( result_error );
		return;
	}

	requests.push_back( resources::create_request( config["main_animation"], resources::animation_class ) ); // sushi@TODO: I don't know how to do that on a single line without naming variable

	ASSERT( UNKNOWN_EXPRESSION_T( config.value_exists( "landing_points" ) ) );

	configs::binary_config_value const& points			= config["landing_points"];
	configs::binary_config_value const* it_point		= points.begin( );
	configs::binary_config_value const* it_point_end	= points.end( );

	for ( ; it_point != it_point_end ; ++it_point )
	{
		configs::binary_config_value const& point = *it_point;

		ASSERT( UNKNOWN_EXPRESSION_T( point.value_exists( "start_animation" ) && point.value_exists( "end_animation" ) ) );

		pcstr start_animation = point["start_animation"];
		if ( !strings::equal( start_animation, "" ) )
			requests.push_back( resources::create_request( start_animation, resources::animation_class ) ); // target doesn't store those on stack

		pcstr end_animation = point["end_animation"];
		if ( !strings::equal( end_animation, "" ) )
			requests.push_back( resources::create_request( end_animation, resources::animation_class ) );
	}

	if ( requests.size( ) == 1 )
	{
		LOG_ERROR( "there are no animations for landing points of this ladder, object will be useless, hence won't be created" );
		parent.finish_query( result_error );
		return;
	}

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind( &ladder_cook::on_animations_loaded, this, _1, config ),
		g_allocator,
		0,
		&parent
	);
}
// claude@NOTE: STRUCTURE MATCH (33 stmts in order). Residuals are inline-vs-call walls plus a
// 4-byte frame-slot cascade (base sub esp,4D0h vs target 4CCh shifts every [ebp-N]): target
// INLINES survarium::landing_point::landing_point (its inline-defined ctor is emitted
// out-of-line + CALLed in OUR base - 1 symbol, ZERO in target), and the new_ladder VOSTOK_NEW
// tracks two destructible temporaries in the EH state var (or [ebp-260h],1 / or ...,2) where
// our build tracks one ("missing or 2"). begin/size/c_ptr and operator[] also fold
// inline-vs-call. Decided by global template/inline emission - not steerable from this caller.
void ladder_cook::on_animations_loaded( resources::queries_result& data, configs::binary_config_value const& config )
{
	resources::query_result_for_cook* const	parent = data.get_parent_query();
	ASSERT( UNKNOWN_EXPRESSION );
	if ( !data.is_successful() )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		parent->finish_query							( result_error );
		return;
	}

	u32					resource_index		= 0;
	float4x4 const&		ladder_transform	= math::create_rotation( config["rotation"] ) * math::create_translation( config["position"] );
	math::plane const& 	ladder_plane		= math::create_plane_normalized( ladder_transform.k.xyz( ), ladder_transform.c.xyz( ) );
	ladder*				new_ladder			= VOSTOK_NEW_IMPL( g_allocator, ladder )( data[resource_index++].get_managed_resource( ), ladder_plane ); // Missing `or 2` from target
	new_ladder->load( config );
	configs::binary_config_value const&	points			= config["landing_points"];
	configs::binary_config_value const* it_point		= points.begin( );
	configs::binary_config_value const* it_point_end	= points.end( );

	for ( ; it_point != it_point_end ; ++it_point )
	{
		configs::binary_config_value const& point = *it_point;
		float4x4 const&	point_tansform = math::create_rotation( point["rotation"] ) * math::create_translation( point["position"] ) * ladder_transform;
		landing_point* new_point = VOSTOK_NEW_IMPL( g_allocator, landing_point )( point_tansform.c.xyz( ), point_tansform.get_angles_xyz( ) ); // `landing_point` constructor inlined in target

		pcstr start_animation = point["start_animation"];
		if ( !strings::equal( start_animation, "" ) )
			new_point->set_start_animation( data[resource_index++].get_managed_resource( ) );

		pcstr end_animation = point["end_animation"];
		if ( !strings::equal( end_animation, "" ) )
			new_point->set_end_animation( data[resource_index++].get_managed_resource( ) );

		if ( !new_point->get_start_animation( ).c_ptr( ) && !new_point->get_end_animation( ).c_ptr( ) )
		{
			LOG_WARNING( "landing point has no start/end animation, it's useless, hence won't be created" );
			DEBUG_BREAK( );
			VOSTOK_DELETE_IMPL( g_allocator, new_point );
			continue;
		}

		new_ladder->add_landing_point( new_point );
	}

	parent->set_unmanaged_resource( new_ladder, resources::nocache_memory, sizeof( ladder ) );
	parent->finish_query( result_success );
}

void ladder_cook::delete_resource( resources::resource_base* resource )
{
	ladder* ladder_res = static_cast<ladder*>( resource );

	while ( landing_point* point = ladder_res->pop_landing_point( ) )
		VOSTOK_DELETE_IMPL( g_allocator, point );

	VOSTOK_DELETE_IMPL( g_allocator, ladder_res );
}

} // namespace survarium
