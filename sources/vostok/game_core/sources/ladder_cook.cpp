////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/ladder_cook.h>

#include <vostok/game_core/ladder.h>
#include <vostok/game_core/landing_point.h>

namespace survarium {

// STATE[65.21%|DONE]: LTCG for `translate_query_cook`.
ladder_cook::ladder_cook( ) : resources::translate_query_cook( resources::ladder_class, reuse_false, use_current_thread_id )
{
}

// STATE[100%|DONE]
ladder_cook::~ladder_cook( )
{
}

// STATE[71.21%|PARTIAL]
void ladder_cook::translate_query( resources::query_result_for_cook& parent )
{
	variant<32>* user_data = parent.user_data( );
	ASSERT( UNKNOWN_EXPRESSION_T( user_data ) );

	configs::binary_config_value config;
	bool result = user_data->try_get( config );
	ASSERT( UNKNOWN_EXPRESSION_T( result ) );

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

	resources::query_resources			(
		requests.begin( ),
		requests.size( ),
		boost::bind( &ladder_cook::on_animations_loaded, this, _1, config ),
		g_allocator,
		0,
		&parent
	);

	// FUNCTION BODY
	// <0x70162b>|0x01b|+0x00b:'25'
	// <0x701636>|0x026|+0x00c:'26'
	// <0>
	// <0x701642>|0x032|+0x008:'28'
	// <0x70164a>|0x03a|+0x00e:'29'
	// <0x701658>|0x048|+0x00c:'30'
	// <0>
	// <0x701664>|0x054|+0x008:'32'	vectora<resources::request> requests
	// <0>
	// <0x70166c>|0x05c|+0x018:'34'	if ( config.value_exists( "main_animation" )
	// <0>
	// <0x701684>|0x074|+0x080:'36'
	// <0x701704>|0x0f4|+0x00c:'37'
	// <0x701710>|0x100|+0x00d:'38'		return;
	// <0>
	// <1>
	// <0x70171d>|0x10d|+0x02d:'41'	requests.push_back( resources::create_request( config["main_animation"], resources::animation_class ) );
	// <0>
	// <0x70174a>|0x13a|+0x00c:'43'	ASSERT( UNKNOWN_EXPRESSION_T( config.value_exists( "landing_points" ) ) );
	// <0>
	// <0x701756>|0x146|+0x010:'45'
	// <0x701766>|0x156|+0x00b:'46'
	// <0x701771>|0x161|+0x00b:'47'
	// <0>
	// <0x70177c>|0x16c|+0x017:'49' for ( ; it_point != it_point_end ; ++it_point )
	// <0>
	// <0x701793>|0x183|+0x006|[1]:'51'	configs::binary_config_value const& point = *it_point;
	// <0>
	// <0x701799>|0x189|+0x00c:'53'		ASSERT( UNKNOWN_EXPRESSION_T( config.value_exists( "start_animation" ) ) );
	// <0>
	// <0x7017a5>|0x195|+0x015:'55'		pcstr start_animation = point["start_animation"];
	// <0x7017ba>|0x1aa|+0x014:'56'		if ( !strings::equal( start_animation, "" ) )
	// <0x7017ce>|0x1be|+0x024:'57'		{ resources::request r = { start_animation, resources::animation_class }; requests.push_back( r ); } // sushi@TODO
	// <0>
	// <0x7017f2>|0x1e2|+0x015:'59'
	// <0x701807>|0x1f7|+0x014:'60'
	// <0x70181b>|0x20b|+0x027:'61'
	// <0x701842>|0x232|+0x005:'62'	}
	// <0>
	// <0x701847>|0x237|+0x011:'64'
	// <0>
	// <0x701858>|0x248|+0x089:'66'
	// <0x7018e1>|0x2d1|+0x00c:'67'
	// <0x7018ed>|0x2dd|+0x00d:'68'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x7018fa>|0x2ea|+0x0bc:'78'
	// ******
}

// STATE[72.07%|PARTIAL]
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
			new_point->set_start_animation( static_cast_resource_ptr< resources::managed_resource_ptr >( data[resource_index++].get_managed_resource( ) ) );

		pcstr end_animation = point["end_animation"];
		if ( !strings::equal( end_animation, "" ) )
			new_point->set_end_animation( static_cast_resource_ptr< resources::managed_resource_ptr >( data[resource_index++].get_managed_resource( ) ) );

		if ( !new_point->get_start_animation( ) && !new_point->get_end_animation( ) )	// This is done somehow differently
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

	// FUNCTION BODY
	// <0x700fab>|0x01b|+0x00b:'83'
	// <0x700fb6>|0x026|+0x012:'84'
	// <0x700fc8>|0x038|+0x00f:'85'
	// <0>
	// <0x700fd7>|0x047|+0x012:'87'
	// <0x700fe9>|0x059|+0x00c:'88'
	// <0x700ff5>|0x065|+0x005:'89'
	// <0>
	// <1>
	// <0x700ffa>|0x06a|+0x007:'92'
	// <0x701001>|0x071|+0x058:'93'
	// <0x701059>|0x0c9|+0x02e:'94'
	// <0x701087>|0x0f7|+0x100:'95'		ladder*				new_ladder			= VOSTOK_NEW_IMPL( g_allocator, ladder )( data[resource_index++].get_managed_resource( ), ladder_plane );
	// <0x701187>|0x1f7|+0x01b:'96'
	// <0x7011a2>|0x212|+0x010:'97'
	// <0x7011b2>|0x222|+0x00b:'98'
	// <0x7011bd>|0x22d|+0x00b:'99'		configs::binary_config_value const* it_point_end	= points.end( );
	// <0>
	// <0x7011c8>|0x238|+0x017:'101'	for ( ; it_point != it_point_end ; ++it_point )
	// <0>								{
	// <0x7011df>|0x24f|+0x006|[1]:'103'	configs::binary_config_value const& point = *it_point;
	// <0x7011e5>|0x255|+0x072:'104'		float4x4 const&	point_tansform = math::create_rotation( point["rotation"] ) * math::create_translation( point["position"] ) * ladder_transform;
	// <0x701257>|0x2c7|+0x102:'105'		landing_point* new_point = VOSTOK_NEW_IMPL( g_allocator, landing_point )( point_tansform.c.xyz( ), point_tansform.get_angles_xyz( ) );
	// <0>
	// <0x701359>|0x3c9|+0x015:'107'		pcstr start_animation = point["start_animation"];
	// <0x70136e>|0x3de|+0x014:'108'		if ( !strings::equal( start_animation, "" ) )
	// <0x701382>|0x3f2|+0x07e:'109'			new_point->set_start_animation( data[resource_index++].get_managed_resource( ) );
	// <0>
	// <0x701400>|0x470|+0x018:'111'		pcstr end_animation = point["end_animation"];
	// <0x701418>|0x488|+0x017:'112'		if ( !strings::equal( end_animation, "" ) )
	// <0x70142f>|0x49f|+0x07e:'113'			new_point->set_end_animation( data[resource_index++].get_managed_resource( ) );
	// <0>
	// <0x7014ad>|0x51d|+0x050:'115'		if ( !new_point->get_start_animation( ).c_ptr( ) && !new_point->get_end_animation( ).c_ptr( ) )
	// <0>									{
	// <0x7014fd>|0x56d|+0x089:'117'			LOG_WARNING( "landing point has no start/end animation, it's useless, hence won't be created" );
	// <0x701586>|0x5f6|+0x001:'118'			DEBUG_BREAK( );
	// <0x701587>|0x5f7|+0x032:'119'			VOSTOK_DELETE_IMPL( g_allocator, new_point );
	// <0x7015b9>|0x629|+0x005:'120'			continue;
	// <0>									}
	// <1>
	// <0x7015be>|0x62e|+0x00c:'123'		new_ladder->add_landing_point( new_point );
	// <0x7015ca>|0x63a|+0x005:'124'	}
	// <0>
	// <0x7015cf>|0x63f|+0x01e:'126'	parent->set_unmanaged_resource( new_ladder, resources::nocache_memory, sizeof( ladder ) );
	// <0x7015ed>|0x65d|+0x00c:'127'	parent->finish_query( result_success );
	// ******
}

// STATE[100%|DONE]
void ladder_cook::delete_resource( resources::resource_base* resource )
{
	ladder* ladder_res = static_cast<ladder*>( resource );

	while ( landing_point* point = ladder_res->pop_landing_point( ) )
		VOSTOK_DELETE_IMPL( g_allocator, point );

	VOSTOK_DELETE_IMPL( g_allocator, ladder_res );

	// FUNCTION BODY
	// <0x700f29>|0x009|+0x006:'132'
	// <0>
	// <0x700f2f>|0x00f|+0x011|[1]:'134'
	// <0x700f40>|0x020|+0x019:'135'
	// <0>
	// <0x700f59>|0x039|+0x026:'137'
	// ******
}

} // namespace survarium
