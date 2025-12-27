////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bi_spline_skeleton_animation_baked_cook.h"

namespace vostok {
namespace animation {

// STATE[STUB]
 bi_spline_skeleton_animation_baked_cook::bi_spline_skeleton_animation_baked_cook( )
	: resources::inplace_unmanaged_cook( resources::bi_spline_skeleton_animation_baked_class, reuse_true ) // sushi@TODO
{
	// FUNCTION BODY
	// <0x121e85>|0x015|+0x048:'27'
	// ******
}

// STATE[STUB]
mutable_buffer bi_spline_skeleton_animation_baked_cook::allocate_resource(
	resources::query_result_for_cook&		in_query,
	u32										file_size,
	u32&									out_offset_to_file,
	bool									file_exist
)
{
	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x121ee0>|0x000|+0x009:'42'
	// <0x121ee9>|0x009|+0x036:'43'
	// ******
}

// STATE[STUB]
void bi_spline_skeleton_animation_baked_cook::deallocate_resource( void* buffer )
{
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
/*
// STATE[STUB]
void* bi_spline_skeleton_animation_baked_cook::`scalar deleting destructor'( u32 arg_0 )
{
	return NULL;

	// FUNCTION BODY
	// <0x607a0>|0x000|+0x000:'76'	{
	// <0>
	// <0x607a0>|0x000|      :'78'	}
	// ******
}
*/
// STATE[STUB]
fastdelegate::FastDelegate< void( resources::query_result_for_cook&, mutable_buffer ) > bi_spline_skeleton_animation_baked_cook::get_create_resource_inplace_in_inline_fat_delegate( )
{
	// FUNCTION BODY
	// <0x122010>|0x000|      :'81'	{
	// ******
}
/*
// STATE[STUB]
void `dynamic atexit destructor for 's_bi_spline_skeleton_animation_baked_cook''( )
{
	// FUNCTION BODY
	// <0x7deb70>|0x000|      :'110'	{
	// ******
}
*/
} // namespace animation
} // namespace vostok
