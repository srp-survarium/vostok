// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/animation_analysis_result_cook.h>

#include <vostok/game_core/animation_analysis_result_cook_user_data.h>
#include <vostok/game_core/animation_analyzer.h>
#include <vostok/game_core/animation_analysis_result.h>

namespace survarium {

animation_analysis_result_cook::animation_analysis_result_cook( )
	: resources::translate_query_cook( resources::animation_analysis_result_class, reuse_false, use_current_thread_id )
{
}

void animation_analysis_result_cook::translate_query( resources::query_result_for_cook& parent )
{
	// claude@MATCH: line layout matches the target so LOG_ERROR's __LINE__
	// (ud=L24 .. LOG=L27) is byte-identical - do NOT collapse these lines.

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
}

void animation_analysis_result_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );
}

} // namespace survarium
