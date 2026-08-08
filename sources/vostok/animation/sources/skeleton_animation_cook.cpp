////////////////////////////////////////////////////////////////////////////
//	Created		: 20.07.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "skeleton_animation_cook.h"
#include "bi_spline_skeleton_animation_baked.h"

using vostok::animation::skeleton_animation_cook;
using vostok::animation::bi_spline_skeleton_animation_baked_ptr;

skeleton_animation_cook::skeleton_animation_cook		( ) :
	super(
		resources::animation_class,
		reuse_true,
		use_resource_manager_thread_id
	)
{
	register_cook	( this );
}

void skeleton_animation_cook::on_cubic_spline_animation_cooked	(
	vostok::resources::queries_result&		data,
	bi_spline_skeleton_animation_baked_ptr	bi_spline_animation
)
{
	resources::managed_resource_ptr managed_resource = data[ 0 ].get_managed_resource( );
	data.get_parent_query( )->set_managed_resource( managed_resource );
	data.get_parent_query( )->finish_query( result_success );
}

void skeleton_animation_cook::on_bi_spline_animation_arrived( vostok::resources::queries_result& result )
{
	if ( !result.is_successful( ) )
	{
		result.get_parent_query( )->finish_query( result_error );
		return;
	}

	bi_spline_skeleton_animation_baked_ptr bi_spline_animation = static_cast_resource_ptr< bi_spline_skeleton_animation_baked_ptr >( result[ 0 ].get_unmanaged_resource( ) );
	resources::query_create_resource(
		"",
		const_buffer( (pvoid)bi_spline_animation.c_ptr( ), sizeof( bi_spline_animation ) ),
		resources::cubic_spline_skeleton_animation_class,
		boost::bind( &skeleton_animation_cook::on_cubic_spline_animation_cooked, _1, bi_spline_animation ),
		resources::helper_allocator( ),
		0,
		result.get_parent_query( )
	);
}

void skeleton_animation_cook::translate_query		( vostok::resources::query_result_for_cook& parent )
{
	resources::query_resource(
		parent.get_requested_path( ),
		resources::bi_spline_skeleton_animation_class,
		boost::bind( &skeleton_animation_cook::on_bi_spline_animation_arrived, _1 ),
		resources::helper_allocator( ),
		0,
		&parent
	);
}

void skeleton_animation_cook::delete_resource		( vostok::resources::resource_base* dying_resource )
{
	VOSTOK_UNREFERENCED_PARAMETER	( dying_resource );
	NODEFAULT					( );
}
