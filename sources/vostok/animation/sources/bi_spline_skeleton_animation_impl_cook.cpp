////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bi_spline_skeleton_animation_impl_cook.h"
#include "bi_spline_skeleton_animation_baked.h"

namespace vostok {
namespace animation {

typedef resources::resource_ptr< bi_spline_skeleton_animation_baked, resources::unmanaged_intrusive_base >
	bi_spline_skeleton_animation_baked_ptr;

// sushi@NOTE: Incorrect flags most likely
 bi_spline_skeleton_animation_impl_cook::bi_spline_skeleton_animation_impl_cook( )
	: resources::translate_query_cook( resources::bi_spline_skeleton_animation_class, reuse_true, use_resource_manager_thread_id, 1 << 3 /* resources::flag_last */ )
{
	resources::register_cook( this );
}

// claude@NOTE: success-path structure matches; the failure-branch finish_query
// folds into the is_successful() check (TRGT_ONLY) - the gold build emits a
// richer 2-result error selection there (picks the failed sub-query, error code
// from resource null-ness) that this simple results[0].get_error_type() form does
// not reproduce; exact selection helper unknown.
void bi_spline_skeleton_animation_impl_cook::on_resources_ready(
	resources::queries_result&				results,
	resources::query_result_for_cook* const	parent_query
)
{
	if ( !results.is_successful( ) )
	{
		parent_query->finish_query		( results[0].get_error_type( ), results.assert_on_fail( ) );
		return;
	}

	bi_spline_skeleton_animation_baked_ptr animation	= static_cast_resource_ptr< bi_spline_skeleton_animation_baked_ptr >( results[0].get_unmanaged_resource( ) );

	configs::binary_config_ptr bones_names				= static_cast_resource_ptr< configs::binary_config_ptr >( results[1].get_unmanaged_resource( ) );

	animation->m_bones_names			= bones_names;

	parent_query->set_unmanaged_resource( animation.c_ptr( ), resources::nocache_memory, sizeof( bi_spline_skeleton_animation_baked ) );
	parent_query->finish_query			( result_success );
}

// claude@NOTE: structure matches the 99%-paired single_animation_cook::translate_query
// sibling; the requests[2]/query_resources split and the get_requested_path() inline
// (declared out-of-line, gold inlines two member reads) are /Od scheduling + inline-vs-call
// residuals shared with that sibling, not source-steerable here.
void bi_spline_skeleton_animation_impl_cook::translate_query( resources::query_result_for_cook& parent )
{
	pcstr const requested_path			= parent.get_requested_path();

	pcstr animation_path				= 0;
	STR_JOINA							( animation_path, requested_path, ".b-spline" );

	pcstr bones_names_path				= 0;
	STR_JOINA							( bones_names_path, requested_path, ".bones_names" );

	resources::request requests[2]		= {
		{ animation_path,	resources::bi_spline_skeleton_animation_baked_class	},
		{ bones_names_path,	resources::binary_config_class_impl					},
	};

	resources::query_resources			(
		requests,
		array_size( requests ),
		boost::bind( &bi_spline_skeleton_animation_impl_cook::on_resources_ready, _1, &parent ),
		resources::helper_allocator(),
		0,
		&parent
	);
}

void bi_spline_skeleton_animation_impl_cook::delete_resource( resources::resource_base* resource_to_delete )
{
	resource_to_delete->~resource_base( );
}

/* sushi@TODO: I have no idea how to make it included
//  `dynamic atexit destructor for 's_bi_spline_skeleton_animation_impl_cook'' <0x7deac0>
static bi_spline_skeleton_animation_impl_cook s_bi_spline_skeleton_animation_impl_cook;
*/

} // namespace animation
} // namespace vostok
