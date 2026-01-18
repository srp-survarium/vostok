////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/animation_analysis_result_cook.h>

#include <vostok/game_core/animation_analysis_result_cook_user_data.h>
#include <vostok/game_core/animation_analyzer.h>
#include <vostok/game_core/animation_analysis_result.h>

namespace survarium {

// STATE[65.21%|DONE]
animation_analysis_result_cook::animation_analysis_result_cook( )
	: resources::translate_query_cook( resources::animation_analysis_result_class, reuse_false, use_current_thread_id )
{
}

// STATE[87.15%|PARTIAL]: Logging issues
void animation_analysis_result_cook::translate_query( resources::query_result_for_cook& parent )
{
	animation_analysis_result_cook_user_data ud;
	if ( !parent.user_data( ) || !parent.user_data( )->try_get( ud ) )
	{
		LOG_ERROR( "Failed to get user data for animation_analysis_result_cook" );
		return;
	}

	animation_analyzer analyzer( ud.legs, ud.legs_count, *ud.skeleton );
	animation_analysis_result* result = VOSTOK_NEW_IMPL( g_allocator, animation_analysis_result )( ud.legs_count );

	parent.set_unmanaged_resource( result, resources::nocache_memory, sizeof( animation_analysis_result ) );
	parent.finish_query( result_success, assert_on_fail_true );

	// FUNCTION BODY
	// <0x7634c1>|0x011|+0x008:'24'
	// <0x7634c9>|0x019|+0x02a:'25'
	// <0>
	// <0x7634f3>|0x043|+0x078:'27'
	// <0x76356b>|0x0bb|+0x00d:'28'
	// <0>
	// <0x763578>|0x0c8|+0x014:'30'	animation_analyzer analyzer( ud.legs, ud.legs_count, ud.skeleton );
	// <0>
	// <0x76358c>|0x0dc|+0x055:'32'	animation_analysis_result* result = VOSTOK_NEW_IMPL( g_allocator, animation_analysis_result )( ud.legs_count );
	// <0>
	// <0x7635e1>|0x131|+0x01e:'34'
	// <0x7635ff>|0x14f|+0x00c:'35'
	// ******
}

// STATE[31.00%|DONE]
void animation_analysis_result_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( *g_allocator, resource );

	// FUNCTION BODY
	// <0x763489>|0x009|+0x017:'40'
	// ******
}

} // namespace survarium
