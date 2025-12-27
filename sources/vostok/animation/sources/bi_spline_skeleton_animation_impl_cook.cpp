////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bi_spline_skeleton_animation_impl_cook.h"

namespace vostok {
namespace animation {

// STATE[STUB]
 bi_spline_skeleton_animation_impl_cook::bi_spline_skeleton_animation_impl_cook( )
	: resources::translate_query_cook( resources::bi_spline_skeleton_animation_class, reuse_true, use_resource_manager_thread_id, 1 << 3 /* resources::flag_last */ )
{
	// FUNCTION BODY
	// <0x121a90>|0x010|+0x050:'33'
	// ******
}

// STATE[STUB]
void bi_spline_skeleton_animation_impl_cook::on_resources_ready(
	resources::queries_result&		results,
	resources::query_result_for_cook* const	parent_query
)
{
	// FUNCTION BODY
	// <0x121af0>|0x000|+0x027:'38'	{
	// <0>
	// <0x121b17>|0x027|-0x023:'40'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x121af4>|0x004|+0x07a:'45'
	// <0x121b6e>|0x07e|+0x040:'45'
	// <0>
	// <1>
	// <0x121bae>|0x0be|+0x041:'48'
	// <0>
	// <0x121bef>|0x0ff|+0x010:'50'
	// <0>
	// <0x121bff>|0x10f|+0x020:'52'
	// <0x121c1f>|0x12f|-0x0b9:'53'
	// <0x121b66>|0x076|+0x0c3:'54'
	// <0x121c29>|0x139|      :'54'	}
	// ******
}

// STATE[STUB]
void bi_spline_skeleton_animation_impl_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// resources::request[2] 			requests
	// strings::detail::tuples 			STR_JOINA_tuples_unique_identifier
	// strings::detail::tuples 			STR_JOINA_tuples_unique_identifier
	// ******

	// FUNCTION BODY
	// <0x121c73>|0x003|+0x019:'58'
	// <0>
	// <1>
	// <0x121c8c>|0x01c|+0x02c:'61'
	// <0>
	// <1>
	// <0x121cb8>|0x048|+0x02e:'64'
	// <0>
	// <1>
	// <2>
	// <0x121ce6>|0x076|+0x003:'68'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x121ce9>|0x079|+0x061:'77'
	// ******
}

// STATE[STUB]
void bi_spline_skeleton_animation_impl_cook::delete_resource( resources::resource_base* resource_to_delete )
{
	// CALL SITE INFO
	// <0x607b0> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY
	// <0x607a0>|0x000|+0x000:'81'	{
	// <0>
	// <1>
	// <0x607a0>|0x000|      :'84'	}
	// ******
}
/*
// STATE[STUB]
void `dynamic atexit destructor for 's_bi_spline_skeleton_animation_impl_cook''( )
{
	// FUNCTION BODY
	// <0x7deac0>|0x000|      :'89'	{
	// ******
}
*/
} // namespace animation
} // namespace vostok
