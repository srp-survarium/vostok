// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "bi_spline_skeleton_animation_baked_cook.h"

#include <vostok/resources_cook_classes.h>
#include "bi_spline_skeleton_animation_baked.h"
#include <vostok/linkage_helper.h>

VOSTOK_DECLARE_LINKAGE_ID(animation_bi_spline_animation_baked_cook)

namespace vostok {
namespace animation {

class bi_spline_skeleton_animation_baked_cook : public resources::inplace_unmanaged_cook {
public:
	bi_spline_skeleton_animation_baked_cook( )
		: resources::inplace_unmanaged_cook(
			resources::bi_spline_skeleton_animation_baked_class,
			reuse_true,
			use_resource_manager_thread_id,
			use_resource_manager_thread_id
		)
	{
		resources::register_cook( this );
	}

private:
	virtual mutable_buffer allocate_resource(
		resources::query_result_for_cook&		in_query,
		u32										file_size,
		u32&									out_offset_to_file,
		bool									file_exist
	) override
	{
		VOSTOK_UNREFERENCED_PARAMETERS(&in_query, file_exist);
		u32 bytes_to_allocate = sizeof( bi_spline_skeleton_animation_baked ) + file_size;
		out_offset_to_file = sizeof( bi_spline_skeleton_animation_baked );
		return mutable_buffer( UNMANAGED_ALLOC( char, bytes_to_allocate ), bytes_to_allocate );
	}

	virtual void deallocate_resource( void* buffer ) override
	{
		UNMANAGED_FREE( buffer );
	}

	virtual void create_resource(
		resources::query_result_for_cook&		in_out_query,
		mutable_buffer							in_out_unmanaged_resource_buffer
	) override
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

	virtual void destroy_resource( resources::unmanaged_resource* resource ) override
	{
		resource->~unmanaged_resource( );
	}

	virtual create_resource_inplace_delegate_type get_create_resource_inplace_in_inline_fat_delegate( ) override
	{
		return create_resource_inplace_delegate_type( this, &bi_spline_skeleton_animation_baked_cook::create_resource_inplace );
	}

	virtual create_resource_inplace_delegate_type get_create_resource_inplace_in_creation_data_delegate( ) override
	{
		return create_resource_inplace_delegate_type( this, &bi_spline_skeleton_animation_baked_cook::create_resource_inplace );
	}

private:
	void create_resource_inplace(
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
}; // class bi_spline_skeleton_animation_baked_cook

STATIC_SIZE_ASSERT(bi_spline_skeleton_animation_baked_cook, 0x20);

static bi_spline_skeleton_animation_baked_cook s_bi_spline_skeleton_animation_baked_cook;

} // namespace animation
} // namespace vostok
