////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/animation_analysis_result_cook.h>

#include <vostok/game_core/animation_analysis_result_cook_user_data.h>
#include <vostok/game_core/animation_analyzer.h>
#include <vostok/game_core/animation_analysis_result.h>

namespace survarium {

// STATE[100%|DONE]
animation_analysis_result_cook::animation_analysis_result_cook( )
	: resources::translate_query_cook( resources::animation_analysis_result_class, reuse_false, use_current_thread_id )
{
}

// STATE[96.49%|PARTIAL]: LOG_ERROR boost::function inline-vs-call residual
void animation_analysis_result_cook::translate_query( resources::query_result_for_cook& parent )
{
	// claude@MATCH: line layout matches the target so LOG_ERROR's __LINE__ (ud=L24 .. LOG=L27) is byte-identical.
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

	// STRUCTURE DIFF[target 0x7534b0 | base 0x567de0]: target 12 / base 12 stmts
	// .. same ..
	// 0x043 <0x78> | 0x043 <0x75> | LOG_ERROR( "Failed to get user data for animation_analysis_result_cook" );   SIZE
	// .. same ..
	// ; aligned 11, size-diffs 1, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is __LOG boost::function copy-construct inline-vs-call inside LOG_ERROR (logging internals; string len + __LINE__ now matched), non-steerable. trail: animation_analysis_result_cook_translate_query.md
}

// STATE[31.00%|PARTIAL]: shared cook-base delete_helper inline-vs-call wall
void animation_analysis_result_cook::delete_resource( resources::resource_base* resource )
{
	VOSTOK_DELETE_IMPL( g_allocator, resource );

	// STRUCTURE DIFF[target 0x753480 | base 0x567db0]: target 1 / base 1 stmts
	// 0x009 <0x17> | 0x00a <0x16> | VOSTOK_DELETE_IMPL( g_allocator, resource );   SIZE
	// ; aligned 0, size-diffs 1, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the shared cook-base delete_helper wall: target inlines delete_helper down to delete_helper_impl while base keeps the wrapper + strip_pointer call, non-steerable. trail: animation_analysis_result_cook_delete_resource.md
}

} // namespace survarium
