// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "bi_spline_skeleton_animation_baked_cook.h"

#include "bi_spline_skeleton_animation_baked.h"

namespace vostok {
namespace animation {

 bi_spline_skeleton_animation_baked_cook::bi_spline_skeleton_animation_baked_cook( )
	: resources::inplace_unmanaged_cook(
		resources::bi_spline_skeleton_animation_baked_class,
		reuse_true,
		use_resource_manager_thread_id,
		use_resource_manager_thread_id
	)
{
	resources::register_cook( this );
}

mutable_buffer bi_spline_skeleton_animation_baked_cook::allocate_resource(
	resources::query_result_for_cook&		in_query,
	u32										file_size,
	u32&									out_offset_to_file,
	bool									file_exist
)
{
	VOSTOK_UNREFERENCED_PARAMETERS(&in_query, file_exist);
	out_offset_to_file = sizeof( bi_spline_skeleton_animation_baked );
	u32 bytes_to_allocate = sizeof( bi_spline_skeleton_animation_baked ) + file_size;
	return mutable_buffer( UNMANAGED_ALLOC( char, bytes_to_allocate ), bytes_to_allocate );
}

void bi_spline_skeleton_animation_baked_cook::deallocate_resource( void* buffer )
{
	UNMANAGED_FREE( buffer );
}

void bi_spline_skeleton_animation_baked_cook::create_resource(
	resources::query_result_for_cook&		in_out_query,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
	create_resource_inplace( in_out_query, in_out_unmanaged_resource_buffer );
}

inline void bi_spline_skeleton_animation_baked_cook::create_resource_inplace(
	resources::query_result_for_cook&		in_out_query,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
	create_baked_animation_in_place(
		static_cast< pbyte >( in_out_unmanaged_resource_buffer.c_ptr() ) +
			sizeof( bi_spline_skeleton_animation_baked ),
		in_out_query.get_raw_file_size()
	);

	bi_spline_skeleton_animation_baked* const animation =
		new ( in_out_unmanaged_resource_buffer.c_ptr() )
			bi_spline_skeleton_animation_baked;

	in_out_query.set_unmanaged_resource(
		animation,
		resources::managed_memory,
		in_out_unmanaged_resource_buffer.size()
	);
	in_out_query.finish_query( result_success );
}

void bi_spline_skeleton_animation_baked_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	resource->~unmanaged_resource( );
}

bi_spline_skeleton_animation_baked_cook::resource_delegate bi_spline_skeleton_animation_baked_cook::get_create_resource_inplace_in_inline_fat_delegate( )
{
	return resource_delegate( this, &bi_spline_skeleton_animation_baked_cook::create_resource_inplace );
}

bi_spline_skeleton_animation_baked_cook::resource_delegate bi_spline_skeleton_animation_baked_cook::get_create_resource_inplace_in_creation_data_delegate( )
{
	return resource_delegate( this, &bi_spline_skeleton_animation_baked_cook::create_resource_inplace );
}

static bi_spline_skeleton_animation_baked_cook s_bi_spline_skeleton_animation_baked_cook;

} // namespace animation
} // namespace vostok
