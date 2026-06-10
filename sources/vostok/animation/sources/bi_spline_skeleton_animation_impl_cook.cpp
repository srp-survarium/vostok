////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bi_spline_skeleton_animation_impl_cook.h"

namespace vostok {
namespace animation {

// sushi@NOTE: Incorrect flags most likely
 bi_spline_skeleton_animation_impl_cook::bi_spline_skeleton_animation_impl_cook( )
	: resources::translate_query_cook( resources::bi_spline_skeleton_animation_class, reuse_true, use_resource_manager_thread_id, 1 << 3 /* resources::flag_last */ )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x121a90>|0x010|+0x050:'33'
	// ******
}

// STATE[STUB]
void bi_spline_skeleton_animation_impl_cook::on_resources_ready(
	resources::queries_result&				results,
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

	// parent.get_requested_path( );
	pcstr const req_path = parent.get_requested_path();

	pcstr temp_name_todo_1 = 0;
	STR_JOINA( temp_name_todo_1, req_path, ".b-spline" );

	pcstr temp_name_todo_2 = 0;
	STR_JOINA( temp_name_todo_2, req_path, ".bones_names" );

	resources::request const requests[] = {
		{ temp_name_todo_1,		resources::bi_spline_skeleton_animation_baked_class },
		{ temp_name_todo_2,		resources::binary_config_class_impl					},
	};

	resources::query_resources			(
		requests,
		array_size( requests ),
		boost::bind( &bi_spline_skeleton_animation_impl_cook::on_resources_ready, _1, &parent ),
		resources::helper_allocator(),
		0,
		&parent
	);

	// FUNCTION BODY
	// <0x121c73>|0x003|+0x019:'58'
	// <0>
	// <1>
	// <0x121c8c>|0x01c|+0x02c:'61'	STR_JOINA( temp_name_todo_1, parent.get_requested_path( ), ".b-spline" );
	// <0>
	// <1>
	// <0x121cb8>|0x048|+0x02e:'64'	STR_JOINA( temp_name_todo_2, parent.get_requested_path( ), ".bones_names" );
	// <0>
	// <1>
	// <2>
	// <0x121ce6>|0x076|+0x003:'68'	resources::request const requests[]
	// <0>
	// <7>							resources::query_resources			(
	// <0x121ce9>|0x079|+0x061:'77'
	// ******
}

void bi_spline_skeleton_animation_impl_cook::delete_resource( resources::resource_base* resource_to_delete )
{
	resource_to_delete->~resource_base( );

	// FUNCTION BODY
	// <0x607a0>|0x000|+0x000:'81'	{
	// <0>
	// <1>
	// <0x607a0>|0x000|      :'84'	}
	// ******
}

/* sushi@TODO: I have no idea how to make it included
//  `dynamic atexit destructor for 's_bi_spline_skeleton_animation_impl_cook'' <0x7deac0>
static bi_spline_skeleton_animation_impl_cook s_bi_spline_skeleton_animation_impl_cook;
*/

} // namespace animation
} // namespace vostok
