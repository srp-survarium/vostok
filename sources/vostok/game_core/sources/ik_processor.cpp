////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/ik_processor.h>

#include <vostok/animation/api.h>
#include <vostok/animation/skeleton.h>

namespace survarium {

// STATE[STUB]
// survarium::ik_processor::ik_processor()
ik_processor::ik_processor( )
{
	// FUNCTION BODY
	// <0x58eba0>|0x000|+0x022:'16'	{
	// <0x58ebc2>|0x022|      :'17'	}
	// ******
}

// STATE[STUB]
// void survarium::ik_processor::activate(vostok::animation::skeleton const&)
void ik_processor::activate( animation::skeleton const& skeleton )
{
	// FUNCTION BODY
	// <0x58eb87>|0x007|+0x008:'21'
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::get_bone_matrix_in_object_space_impl(vostok::animation::skeleton_bone const&, vostok::math::float4x4 const*, vostok::animation::skeleton_bone const*)
float4x4 get_bone_matrix_in_object_space_impl( animation::skeleton_bone const& bone, float4x4 const* matrices, animation::skeleton_bone const* first_non_root_bone )
{
	// LOCALS
	// u32 							matrix_index
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY
	// <0x58ebdb>|0x00b|+0x011:'26'
	// <0x58ebec>|0x01c|+0x08f:'27'
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::get_bone_matrix_in_object_space(vostok::animation::skeleton_bone const&, vostok::animation::skeleton const&, vostok::math::float4x4 const*)
float4x4 get_bone_matrix_in_object_space( animation::skeleton_bone const& bone, animation::skeleton const& skeleton, float4x4 const* matrices )
{
	// LOCALS
	// animation::skeleton_bone const* first_non_root_bone
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY
	// <0x58ec97>|0x007|+0x01a:'32'
	// <0x58ecb1>|0x021|+0x01b:'33'
	// ******
}

} // namespace survarium
