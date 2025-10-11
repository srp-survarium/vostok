////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "ik_processor.h"

namespace survarium {

// STATE[STUB]
// survarium::ik_processor::ik_processor()
ik_processor::ik_processor( )
{
}

// STATE[STUB]
// void survarium::ik_processor::activate(vostok::animation::skeleton const&)
void ik_processor::activate( vostok::animation::skeleton const& skeleton )
{
	// FUNCTION BODY
	// <0x58eb87>|0x000|0x000:'21'
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::get_bone_matrix_in_object_space_impl(vostok::animation::skeleton_bone const&, vostok::math::float4x4 const*, vostok::animation::skeleton_bone const*)
vostok::math::float4x4 get_bone_matrix_in_object_space_impl( vostok::animation::skeleton_bone const& bone, vostok::math::float4x4 const* matrices, vostok::animation::skeleton_bone const* first_non_root_bone )
{
	// LOCALS
	// u32 							matrix_index
	// ******

	return vostok::math::float4x4();
	// FUNCTION BODY
	// <0x58ebdb>|0x000|0x000:'26'
	// <0x58ebec>|0x011|0x011:'27'
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::get_bone_matrix_in_object_space(vostok::animation::skeleton_bone const&, vostok::animation::skeleton const&, vostok::math::float4x4 const*)
vostok::math::float4x4 get_bone_matrix_in_object_space( vostok::animation::skeleton_bone const& bone, vostok::animation::skeleton const& skeleton, vostok::math::float4x4 const* matrices )
{
	// LOCALS
	// vostok::animation::skeleton_bone const* first_non_root_bone
	// ******

	return vostok::math::float4x4();
	// FUNCTION BODY
	// <0x58ec97>|0x000|0x000:'32'
	// <0x58ecb1>|0x01a|0x01a:'33'
	// ******
}

} // namespace survarium
