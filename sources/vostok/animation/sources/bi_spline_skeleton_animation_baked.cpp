////////////////////////////////////////////////////////////////////////////
//	Created 	: 25.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bi_spline_skeleton_animation_baked.h"

namespace vostok {
namespace animation {

// STATE[STUB]
void create_baked_animation_in_place( void* const raw_buffer, const u32 buffer_size )
{
	// LOCALS
	// mutable_buffer 					buffer
	// bi_spline_event_channel_baked* const event_channels
	// ******

	// TYPEDEFS
	// typedef
	// 	platform_pointer_selector< bi_spline_channel_animation_baked, 1 >::helper
	// 	bi_spline_channel_animation_baked_ptr;

	// ******

	mutable_buffer buffer( raw_buffer, buffer_size );
	buffer += 4;

	// FUNCTION BODY[0x73b3f0]: 26
	// <0x73b3f7>|0x007|+0x00b:'119'
	// <0x73b402>|0x012|+0x004:'120'
	// <0x73b406>|0x016|+0x00e:'121'
	// <0>
	// <1>
	// <0x73b414>|0x024|+0x018:'124'
	// <0>
	// <1>
	// <0x73b42c>|0x03c|+0x015:'127'
	// <0>
	// <1>
	// <2>
	// <0x73b441>|0x051|+0x043:'131'
	// <0x73b484>|0x094|-0x034:'131'
	// <0x73b450>|0x060|+0x007:'132'
	// <0x73b457>|0x067|+0x002:'133'
	// <0x73b459>|0x069|+0x010:'134'
	// <0>
	// <0x73b469>|0x079|+0x021:'136'
	// <0>
	// <1>
	// <2>
	// <0x73b48a>|0x09a|+0x016:'140'
	// <0x73b4a0>|0x0b0|+0x003:'141'
	// <0x73b4a3>|0x0b3|+0x016:'142'
	// <0x73b4b9>|0x0c9|+0x042:'143'
	// <0>
	// ******
}

} // namespace animation
} // namespace vostok
