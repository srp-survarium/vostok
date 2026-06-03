////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bi_spline_skeleton_animation_baked_cook.h"

#include "bi_spline_skeleton_animation_baked.h"

namespace vostok {
namespace animation {

// STATE[UNCHECKED]
 bi_spline_skeleton_animation_baked_cook::bi_spline_skeleton_animation_baked_cook( )
	: resources::inplace_unmanaged_cook(
		resources::bi_spline_skeleton_animation_baked_class,
		reuse_true,
		use_resource_manager_thread_id,
		use_resource_manager_thread_id,
		resources::cook_base::internal_flag_does_inplace_cook // sushi@TODO: Understand what this flag does exactly
	)
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x121e85>|0x015|+0x048:'27'
	// ******
}

// STATE[UNCHECKED]
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
	mutable_buffer out_buffer( UNMANAGED_ALLOC( u8, bytes_to_allocate ), bytes_to_allocate );
	return out_buffer;

	/* sushi@NOTE: Other similar implentations call this. Should we? particle_world_cooker::allocate_resource
		if ( !out_buffer )
		in_query.set_out_of_memory	(resources::unmanaged_memory, bytes_to_allocate);
	*/

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x121ee0>|0x000|+0x009:'42'
	// <0x121ee9>|0x009|+0x036:'43'
	// ******
}

// STATE[100%|DONE]
void bi_spline_skeleton_animation_baked_cook::deallocate_resource( void* buffer )
{
	UNMANAGED_FREE( buffer );

	// FUNCTION BODY
	// <0x121f30>|0x000|+0x01b:'48'
	// ******
}

// STATE[STUB]
void bi_spline_skeleton_animation_baked_cook::create_resource(
	resources::query_result_for_cook&		in_out_query,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
	/*
	create_baked_animation_in_place(

	in_out_query.set_unmanaged_resource( new_particle_world, resources::nocache_memory, sizeof(particle_world) );
	in_out_query.finish_query( result_success );
	*/
	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x121f5b>|0x00b|+0x04c:'61'
	// <0>
	// <1>
	// <0x121fa7>|0x057|+0x024:'64'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x121fcb>|0x07b|+0x024:'70'
	// <0>
	// <0x121fef>|0x09f|+0x00b:'72'
	// ******
}

// STATE[UNCHECKED]
void bi_spline_skeleton_animation_baked_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	resource->~unmanaged_resource( );

	// FUNCTION BODY
	// ******
}

// STATE[100%|PARTIAL]: sushi@NOTE: Might still be related to an unimplemented function
bi_spline_skeleton_animation_baked_cook::resource_delegate bi_spline_skeleton_animation_baked_cook::get_create_resource_inplace_in_inline_fat_delegate( )
{
	return resource_delegate( this, &bi_spline_skeleton_animation_baked_cook::create_resource );

	// FUNCTION BODY
	// ******
}

// STATE[MISSING]: sushi@NOTE. The implementation is missing. If you look at vtable, you would see that exactly the same function is used for `inline_fat_delegate`.
bi_spline_skeleton_animation_baked_cook::resource_delegate bi_spline_skeleton_animation_baked_cook::get_create_resource_inplace_in_creation_data_delegate( )
{
	return resource_delegate( this, &bi_spline_skeleton_animation_baked_cook::create_resource );
}

/* sushi@TODO: Same as impl_cook
// STATE[UNCHECKED]
//  `dynamic atexit destructor for 's_bi_spline_skeleton_animation_baked_cook'' <0x7deb70>
static bi_spline_skeleton_animation_baked_cook s_bi_spline_skeleton_animation_baked_cook;
*/
} // namespace animation
} // namespace vostok
