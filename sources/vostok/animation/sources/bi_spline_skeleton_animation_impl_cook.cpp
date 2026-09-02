// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "bi_spline_skeleton_animation_impl_cook.h"
#include "bi_spline_skeleton_animation_baked.h"

namespace vostok {
namespace animation {

typedef resources::resource_ptr< bi_spline_skeleton_animation_baked, resources::unmanaged_intrusive_base >
	bi_spline_skeleton_animation_baked_ptr;

 bi_spline_skeleton_animation_impl_cook::bi_spline_skeleton_animation_impl_cook( )
	: resources::translate_query_cook( resources::bi_spline_skeleton_animation_class, reuse_true, use_resource_manager_thread_id )
{
	resources::register_cook( this );
}

void bi_spline_skeleton_animation_impl_cook::on_resources_ready(
	resources::queries_result&				results,
	resources::query_result_for_cook* const	parent_query
)
{
	if ( !results.is_successful( ) )
	{
		parent_query->finish_query		(
			results[ results[0].is_successful( ) ? 1 : 0 ].get_error_type( ),
			parent_query->assert_on_fail( )
		);
		return;
	}

	bi_spline_skeleton_animation_baked_ptr animation	= static_cast_resource_ptr< bi_spline_skeleton_animation_baked_ptr >( results[0].get_unmanaged_resource( ) );

	configs::binary_config_ptr bones_names				= static_cast_resource_ptr< configs::binary_config_ptr >( results[1].get_unmanaged_resource( ) );

	animation->m_bones_names			= bones_names;

	parent_query->set_unmanaged_resource( animation.c_ptr( ), resources::nocache_memory, sizeof( bi_spline_skeleton_animation_baked ) );
	parent_query->finish_query			( result_success );
}

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

static bi_spline_skeleton_animation_impl_cook s_bi_spline_skeleton_animation_impl_cook;

} // namespace animation
} // namespace vostok
